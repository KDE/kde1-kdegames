#include "board.h"
#include "dialogs.h"

#include <stdio.h>
#include <unistd.h>

#include <qtimer.h>
#include <qkeycode.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qmsgbox.h>

#include <kapp.h>
#include <kkeyconf.h>
#include <kmsgbox.h>

#include "board.moc"


const int waitAfterLineTime = 500;

Board::Board( int type, QWidget *p, const char *name )
: QFrame( p, name ), GenericTetris(type)
{
    setFrameStyle( QFrame::Panel | QFrame::Sunken );

	multiGame = FALSE;
	paint = 0;
    timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()), SLOT(timeout()) );

    colors[0].setRgb(200,100,100);
    colors[1].setRgb(100,200,100);
    colors[2].setRgb(100,100,200);
    colors[3].setRgb(200,200,100);
    colors[4].setRgb(200,100,200);
    colors[5].setRgb(100,200,200);
    colors[6].setRgb(218,170,  0);
	colors[7].setRgb(100,100,100);

    xOffset          = -1;      /* -1 until a resizeEvent is received. */
    blockWidth       = 20;
    yOffset          = 30;
    blockHeight      = 20;
    noGame           = TRUE;
    isPaused         = FALSE;
    waitingAfterLine = FALSE;
    updateTimeoutTime();   /* Sets timeoutTime */

	msg = new QLabel(this);
	msg->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	msg->setAlignment(AlignCenter);

	pb = new QPushButton(this);
	connect( pb, SIGNAL(clicked()), SLOT(start()) );

	/* configuration & highscore initialisation */
	kconf = kapp->getConfig();
	isConfigWritable =
		(kapp->getConfigState()==KApplication::APPCONFIG_READWRITE);
	
	/* if the entries do not exist : create them */
	kconf->setGroup(HS_GRP);
	QString str1, str2;
	for (int i=0; i<NB_HS; i++) {
		str1.sprintf("%s%i", HS_NAME_KEY, i);
		if ( !kconf->hasKey(str1) )
		    kconf->writeEntry(str1, klocale->translate("Anonymous"));
		str2.sprintf("%s%i", HS_SCORE_KEY, i);    
		if ( !kconf->hasKey(str2) )
			kconf->writeEntry(str2, 0);
	}

	/* show the midbutton with "start game" */
	midbutton(FALSE);
}

void Board::midbutton(bool game_over)
{
	char spa[50];
	char sre[50];
	int sw, sh, sw2, sh2;
	
	if ( isPaused) {
		sprintf(spa, klocale->translate("Game paused"));
		QFontMetrics fm = msg->fontMetrics();
		sw = fm.width(spa) + 10;
		sh = fm.height() + 10;
		
		sprintf(sre, klocale->translate("Press to resume"));
		QFontMetrics fm2 = pb->fontMetrics();
		sw2 = fm2.width(sre) + 10;
		sh2 = fm2.height() + 10;
		
		msg->setGeometry( (BOARD_W - sw)/2, (BOARD_H - sh-sh2)/3, sw, sh );
		msg->setText(spa);
		msg->show();
		
		pb->setGeometry( (BOARD_W - sw2)/2, 2*(BOARD_H - sh-sh2)/3, sw2, sh2 );
		pb->setText(sre);
		pb->show();
	} else {
		msg->hide();
		if (game_over) {
			sprintf(spa, klocale->translate("Game over"));
			QFontMetrics fm = msg->fontMetrics();
			sw = fm.width(spa) + 15;     
			sh = fm.height() + 15;
			sprintf(sre,  klocale->translate("Press Return to replay"));
		} else {
			sw = 0;
			sh = 0;
			sprintf(sre, klocale->translate("Press to start game"));
		}
		
		QFontMetrics fm2 = pb->fontMetrics();
		sw2 = fm2.width(sre) + 10;
		sh2 = fm2.height() + 10;
		
		if (game_over) {
			msg->setGeometry( (BOARD_W - sw)/2, (BOARD_H - sh-sh2)/3, sw, sh );
			msg->setText(spa);     
			msg->show();
		}
		
		pb->setGeometry( (BOARD_W - sw2)/2, 2*(BOARD_H - sh-sh2)/3, sw2, sh2 );
		pb->setText(sre);
		pb->show();
	}
}
void Board::startGame()
{
	noGame = FALSE; 
	setPieceMovingKeys(TRUE);
	setBackgroundColor(black); 
	msg->hide();
    pb->hide();
	
	if ( multiGame && isPaused ) {
		updateOpponents();
		isPaused = FALSE;
	}
	
    if ( isPaused ) {
		timer->start(timeoutTime);
        isPaused = FALSE;    
	} else    
		GenericTetris::startGame();
    /* Note that the timer is started by updateLevel! */
}

void Board::setPieceMovingKeys( bool activate )
{
	kKeys->toggleFunction(K_MAIN, klocale->translate("Move left"), activate);
	kKeys->toggleFunction(K_MAIN, klocale->translate("Move right"), activate);
	kKeys->toggleFunction(K_MAIN, klocale->translate("Drop down"), activate);
	kKeys->toggleFunction(K_MAIN, klocale->translate("One line down"), activate);
	kKeys->toggleFunction(K_MAIN, klocale->translate("Rotate left"), activate);
	kKeys->toggleFunction(K_MAIN, klocale->translate("Rotate right"), activate);
}

void Board::pause()
{
	/* game not active */
    if ( noGame )
        return;
	
	/* already in pause : resume game (reactivate the piece-moving keys */
	if ( isPaused ) {
		setPieceMovingKeys(TRUE);
		startGame();
		return;
	}

	/* disactivate all piece-moving keys */
	setPieceMovingKeys(FALSE);
	
	hideBoard(TRUE);
}

void Board::hideBoard( bool pause )
{
	isPaused = pause;
	timer->stop();
	setBackgroundColor(lightGray);
	GenericTetris::hideBoard();
	midbutton(FALSE);
}

void Board::drawSquare(int x,int y,int value)
{
    if (xOffset == -1)    /* Before first resizeEvent? */
        return;

    const int X = xOffset  + x*blockWidth;
    const int Y = yOffset  + (y - 1)*blockHeight;

    bool localPainter = paint == 0;
    QPainter *p;
    if ( localPainter ) {
		p = new QPainter;    
		p->begin( this );
    } else
		p = paint;

    drawTetrisButton( p, X, Y, blockWidth, blockHeight,
					  value == 0 ? 0 : &colors[value-1] );

    if ( localPainter ) {
		p->end();
		delete p;
    }
}

void Board::drawNextSquare( int x, int y, int value )
{
    if ( value == 0 )
        emit drawNextSquareSignal (x, y, 0 );
    else
        emit drawNextSquareSignal( x, y, &colors[value-1] );
}

void Board::updateRemoved( int noOfLines )
{
	removedLines = removedLines + noOfLines - oldFullLines;
	oldFullLines = noOfLines;

    if ( !multiGame && noOfLines>0 ) {
        timer->stop();
        timer->start( waitAfterLineTime );
        waitingAfterLine = TRUE;
    }
    emit updateRemovedSignal( noOfLines );
}

void Board::updateScore( int newScore )
{
    emit updateScoreSignal( newScore );
}

void Board::updateLevel( int newLevel )
{
	if ( !multiGame ) {
		timer->stop();
		updateTimeoutTime();
		timer->start( timeoutTime );
	}
	
	emit updateLevelSignal( newLevel );
}

void Board::pieceDropped(int)
{
    if ( !multiGame && waitingAfterLine )
		/* give player a break if a line has been removed */
        return;
    newPiece();
}

void Board::gameOver()
{
    timer->stop();
    noGame = TRUE;
	setPieceMovingKeys(FALSE);

	if ( !multiGame ) 
		setHighScore(getScore());
	else {
		QString msg = klocale->translate("You suck !");
		net_obj->gameOver(msg);
	}
	
	midbutton(TRUE);
}

void Board::timeout()
{
	if ( !multiGame ) {
		if ( waitingAfterLine ) {
			timer->stop();
			waitingAfterLine = FALSE;
			newPiece();
			timer->start( timeoutTime );
		} else
			oneLineDown();
	} else {
		/* update own status */
		net_obj->ownStatus(Height-nClearLines, removedLines);
	
		/* call net object playTimeout */
		if ( !net_obj->playTimeout(serror) ) {
			timer->stop();
			net_obj->gameOver(serror);
		} else {
			/* if CLIENT_MODE restart timer (ajust wuth the server one) */
			if ( net_obj->isClient() )
				timer->start(net_obj->getTimeout());
			
			/* update names & position of opponents on screen */
			emit updateOpponents();
			
			/* check and place opponent gift :) */
			checkOpponentGift();
			
			removedLines = 0;
			
			if ( waitingAfterLine ) {
				waitingAfterLine = FALSE;
				newPiece();    
			} else
				oneLineDown();
		}
	}		
}

void Board::drawContents( QPainter *p )
{
	QRect r = contentsRect();

	if ( isPaused )
        return;
	
	paint = p;
	
    int x1,y1,x2,y2;
    x1 = (r.left() - xOffset) / blockWidth;
    if (x1 < 0)
        x1 = 0;
    if (x1 >= boardWidth())
        x1 = boardWidth() - 1;

    x2 = (r.right() - xOffset) / blockWidth;
    if (x2 < 0)
        x2 = 0;
    if (x2 >= boardWidth())
        x2 = boardWidth() - 1;

    y1 = (r.top() - yOffset) / blockHeight;
    if (y1 < 0)
        y1 = 0;
    if (y1 >= boardHeight())
        y1 = boardHeight() - 1;

    y2 = (r.bottom() - yOffset) / blockHeight;
    if (y2 < 0)
        y2 = 0;
    if (y2 >= boardHeight())
        y2 = boardHeight() - 1;

    updateBoard( x1, y1, x2, y2 );
	paint = 0;
    return;
}

void Board::resizeEvent(QResizeEvent *e)
{
    QSize sz = e->size();
    blockWidth  = (sz.width() - 3)/10;
    blockHeight = (sz.height() - 3)/22;
    xOffset     = 1;
    yOffset     = 1;
}

void Board::updateTimeoutTime()
{
    timeoutTime = 1000/(1 + getLevel());
}

void Board::options()
{
	Options::Options(this);
}

void Board::showHighScores()
{
	WHighScores *whs = new WHighScores(TRUE, 0, this);
	delete whs;
}

void Board::setHighScore(int score)
{
	if ( isConfigWritable ) {
		WHighScores *whs = new WHighScores(FALSE, score, this);
		delete whs;
		
		/* save the new score (in the file to be sure it won't be lost) */
		if (isConfigWritable)
			kconf->sync();
	} else
		KMsgBox::message(0, klocale->translate("Warning"), 
						 klocale->translate("Highscores file not writable !"),
						 KMsgBox::EXCLAMATION, klocale->translate("Close"));
}

void drawTetrisButton( QPainter *p, int x, int y, int w, int h,
					   const QColor *color )
{
	QColor fc;
	if ( color ) {
		QPointArray a;
		a.setPoints( 3,  x,y+h-1, x,y, x+w-1,y );
		p->setPen( color->light() );
		p->drawPolyline( a );
		a.setPoints( 3, x+1,y+h-1, x+w-1,y+h-1, x+w-1,y+1 );
		p->setPen( color->dark() );
		p->drawPolyline( a );
		x++; y++;
		w -= 2; h -= 2;
		fc = *color;
	} else
		fc = p->backgroundColor();
	
	p->fillRect( x, y, w, h, fc );
}

void Board::initMultiGame(NetObject *net_object)
{
	net_obj = net_object;

	if ( !net_obj->initGame(serror) )
		ERROR(serror)
	else {
		/* start game */
		multiGame = TRUE;
		emit showOpponents();
		startGame();
			
		/* start timer */
		timer->start(net_obj->getTimeout());
	}
}
