#include "defines.h"
#include "dialogs.h"
#include "gtboard.h"
#include "ggboard.h"

#include <stdio.h>
#include <stdlib.h>

#include <qapp.h>
#include <qfont.h>
#include <qlabel.h>
#include <qdatetm.h>

#include <kapp.h>
#include <Kconfig.h>
#include <kkeyconf.h>
#include <kmsgbox.h>

#include "main.moc"

KTetris::KTetris( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QTime t = QTime::currentTime();
    srand( t.hour()+t.minute()+t.second() );

	net_obj = NULL; /* no net object */

	installEventFilter(this);

	setCaption( kapp->getCaption() );
	
	board = new GTBoard(this);
//	board = new GGBoard(this);
	board->installEventFilter(this);
	
	connect( this, SIGNAL(initMultiGame(NetObject *)),
			 board, SLOT(initMultiGame(NetObject *)) );
	connect( board, SIGNAL(showOpponents()), SLOT(showOpponents()) );
	connect( board, SIGNAL(updateOpponents()), SLOT(updateOpponents()) );

	/* menu */
	popup = new QPopupMenu;
	tog_id = popup->insertItem( "Hide menuBar", this, SLOT(toggleMenu()) );
	popup->insertSeparator();
	popup->insertItem( "Restart game",  board, SLOT(start()) );
	popup->insertItem( "Pause game",  board, SLOT(pause()) );
	popup->insertItem( "MultiPlayer Game" , this, SLOT(multiGame()) );
	popup->insertSeparator();
	popup->insertItem( "High scores", board, SLOT(showHighScores()) );
	popup->insertSeparator();
	popup->insertItem( "Quit", this, SLOT(quit()) );

	QPopupMenu *options = new QPopupMenu;
	options->insertItem( "Keys", this, SLOT(configKeys()) );
	
	QPopupMenu *help = new QPopupMenu;
	help->insertItem( "Help", this, SLOT(help()) );
 	help->insertSeparator();
  	help->insertItem( "About...", this, SLOT(about()) ); ;

	menu = new QMenuBar(this);
	menu->insertItem( "File", popup );
	menu->insertItem( "Options", options );
	menu->insertSeparator();
	menu->insertItem( "Help", help );
	
	/* keys management */
	kKeys->addKey("Quit", "CTRL+Q");
	kKeys->addKey("New game", "F2");
	kKeys->addKey("Pause", "P");
	kKeys->addKey("Help", "F1");
	kKeys->addKey("High scores", "H");
	kKeys->addKey("Move left", "Left");
	kKeys->addKey("Move right", "Right");
	kKeys->addKey("Drop down", "Down");
	kKeys->addKey("One line down", "Shift");
	kKeys->addKey("Rotate left", "Up");
	kKeys->addKey("Rotate right", "Return");
	kKeys->addKey("Close dialog", "Return");
	kKeys->addKey("Ok dialog", "Return");
	kKeys->addKey("Cancel dialog", "Escape");   

	/* keys connections */
	kKeys->registerWidget("main", this);
	kKeys->connectFunction("main", "Quit", this, SLOT(quit()));
	kKeys->connectFunction("main", "New game", board, SLOT(start()));
	kKeys->connectFunction("main", "Pause", board, SLOT(pause()));
	kKeys->connectFunction("main", "Help", this, SLOT(help()));
	kKeys->connectFunction("main", "High scores", 
						   board, SLOT(showHighScores()));
	/* the following connections are disactivated */
	kKeys->connectFunction("main", "Move left",
						   board, SLOT(pMoveLeft()), FALSE);
	kKeys->connectFunction("main", "Move right",
						   board, SLOT(pMoveRight()), FALSE);
	kKeys->connectFunction("main", "Drop down",
						   board, SLOT(pDropDown()), FALSE);
	kKeys->connectFunction("main", "One line down",
						   board, SLOT(pOneLineDown()), FALSE);
	kKeys->connectFunction("main", "Rotate left",
						   board, SLOT(pRotateLeft()), FALSE);
	kKeys->connectFunction("main", "Rotate right",
						   board, SLOT(pRotateRight()), FALSE);
														  
	lScore = new QLabel("Score", this);
    showScore = new QLCDNumber(5,this);
    showScore->display( 0 );
	
	lLines = new QLabel("Lines removed", this);
	showLines = new QLCDNumber(5,this);
    showLines->display( 0 );

    lLevel = new QLabel("Level", this);
    showLevel = new QLCDNumber(2,this);
    showLevel->display( 0 );

	lNext = new QLabel("Next tile", this);
    showNext = new ShowNextPiece(this);

	labPrevName = new QLabel("", this);
	labPrevName->hide();
	prevHeight = new KProgress(0, 22, 0, KProgress::Vertical, this);
	prevHeight->setBackgroundColor(lightGray);
	prevHeight->setTextEnabled(FALSE);
	prevHeight->hide();

	labNextName = new QLabel("", this);
	labNextName->hide();
	nextHeight = new KProgress(0, 22, 0, KProgress::Vertical, this);
	nextHeight->setBackgroundColor(lightGray);
	nextHeight->setTextEnabled(FALSE);
	nextHeight->hide();

	lScore->installEventFilter(this); showScore->installEventFilter(this);
	lLines->installEventFilter(this); showLines->installEventFilter(this);
	lLevel->installEventFilter(this); showLevel->installEventFilter(this);
	lNext->installEventFilter(this); showNext->installEventFilter(this);
	labPrevName->installEventFilter(this);
	prevHeight->installEventFilter(this);
	labNextName->installEventFilter(this);
	nextHeight->installEventFilter(this);
	
    connect( board,     SIGNAL(drawNextSquareSignal(int,int,QColor*)),
			 showNext,  SLOT(drawNextSquare(int,int,QColor*)) );
    connect( showNext,  SIGNAL(update()),
			 board,     SLOT(updateNext()) );
    connect( board,     SIGNAL(updateScoreSignal(int)),
			 showScore, SLOT(display(int)) );
    connect( board,     SIGNAL(updateLevelSignal(int)),
			 showLevel, SLOT(display(int)));
    connect( board,     SIGNAL(updateRemovedSignal(int)),
			 showLines, SLOT(display(int)));
	
	board->revealNextPiece(TRUE);

	/* read the menu visible/invisible config */
	kconf = kapp->getConfig();
	kconf->setGroup("");
	if ( !kconf->hasKey(OP_MENUBAR_VIS) )
		kconf->writeEntry(OP_MENUBAR_VIS, 1);
	if ( kconf->readNumEntry(OP_MENUBAR_VIS)!=1 ) menu->show();
	else menu->hide();
	toggleMenu();
}

void KTetris::quit()
{
	kconf->setGroup("");
	kconf->writeEntry(OP_MENUBAR_VIS, menu->isVisible());
	kapp->quit();
}

void KTetris::toggleMenu()
{
	if ( menu->isVisible() ) {
		popup->changeItem("Show menuBar", tog_id);
		menu->hide();
	} else {
		popup->changeItem("Hide menuBar", tog_id);
		menu->show();
	}
	
	int mh = NO_MENU_H;    
	if ( menu->isVisible() ) mh += menu->height();
	
	setFixedSize( 2*SIDE_W + 2*MID_W + BOARD_W + LCD_W + NTILE_W,
				  mh + TOP_H + BOARD_H + BASE_H );
}

void KTetris::resizeEvent(QResizeEvent *)
{
	int mh = NO_MENU_H;
	if ( menu->isVisible() ) mh = menu->height();
	
	board->setGeometry( SIDE_W + LCD_W + MID_W, mh, BOARD_W, BOARD_H );

	lScore->setGeometry( SIDE_W, mh + TOP_H, LABEL_W, LABEL_H );
    showScore->setGeometry( SIDE_W, mh + TOP_H + LABEL_H, LCD_W, LCD_H );	
	
	lLines->setGeometry( SIDE_W, mh + TOP_H + LABEL_H + LCD_H + MID_H,
						 LABEL_W, LABEL_H );
	showLines->setGeometry( SIDE_W, mh + TOP_H + 2*LABEL_H + LCD_H + MID_H,
						    LCD_W, LCD_H );
	lLevel->setGeometry( SIDE_W, mh + TOP_H + 2*LABEL_H + 2*LCD_H + 2*MID_H,
						 LABEL_W, LABEL_H );
	showLevel->setGeometry( SIDE_W, mh + TOP_H + 3*LABEL_H + 2*LCD_H + 2*MID_H,
						    LCD_W, LCD_H );
	lNext->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W,
						    mh + TOP_H, NTILE_W, LABEL_H );
	showNext->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W,
						   mh + TOP_H + LABEL_H, NTILE_W, NTILE_H );
	labPrevName->setGeometry( SIDE_W,
					mh + TOP_H + BOARD_H + BASE_H - LCD_H - LABEL_H - MID_H,
					LABEL_W, LABEL_H );
	prevHeight->setGeometry( SIDE_W + LCD_W + (MID_W-BAR_W)/2, mh,
							 BAR_W, BOARD_H);
	labNextName->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W,
			   mh + TOP_H + BOARD_H + BASE_H - LCD_H - LABEL_H - MID_H,
			   LABEL_W, LABEL_H );
	nextHeight->setGeometry( SIDE_W + LCD_W + MID_W + BOARD_W +(MID_W-BAR_W)/2,
							 mh, BAR_W, BOARD_H);
}

bool KTetris::eventFilter(QObject *, QEvent *e)
{
	if ( e->type()!=Event_MouseButtonPress ) return FALSE;

	QMouseEvent *em = (QMouseEvent*) e;
	if ( em->button()!=RightButton ) return FALSE;
	
	popup->popup(QCursor::pos());
	return TRUE;
}

void KTetris::about()
{
	QString str;
	str.sprintf( "%s (%s) \n\nby Nicolas HADACEK  (hadacek@kde.org)\n tetris kernel \
by Eirik ENG.\nhttp://www.via.ecp.fr/~hadacek/KDE/ktetris.html",
				 SNAME, SDATE );
	
	KMsgBox::message( 0, kapp->getCaption(), (const char *)str,
					  KMsgBox::INFORMATION, "Close" ); 
}

void KTetris::help()
{
	kapp->invokeHTMLHelp("ktetris.html",NULL);
}

void KTetris::multiGame()
{
	/* reinit the board */
	board->hideBoard(TRUE);

	netDialog *md = new netDialog(&net_obj, this);
	
	if ( md->exec() )
		emit initMultiGame(net_obj);
		
	delete md;
}

void KTetris::showOpponents()
{
	labPrevName->show();
	labNextName->show();
	prevHeight->show();
	nextHeight->show();
}

void KTetris::updateOpponents()
{
	labPrevName->setText( (const char *)net_obj->getPrevName() );
	prevHeight->setValue( net_obj->getPrevHeight() );
	labNextName->setText( (const char *)net_obj->getNextName() );
	nextHeight->setValue( net_obj->getNextHeight() );
}

void KTetris::configKeys()
{
	kKeys->configureKeys(this);
}



ShowNextPiece::ShowNextPiece( QWidget *parent, const char *name )
     : QFrame( parent, name )
{
    initMetaObject();
    setFrameStyle( QFrame::Panel | QFrame::Sunken );
    xOffset = -1;     /* -1 until first resizeEvent. */
}

void ShowNextPiece::resizeEvent( QResizeEvent *e )
{
    QSize sz = e->size();
    blockWidth  = (sz.width()  - 3)/5;
    blockHeight = (sz.height() - 3)/6;
    xOffset     = (sz.width()  - 3)/5;
    yOffset     = (sz.height() - 3)/6;
}


void ShowNextPiece::paintEvent( QPaintEvent * )
{
    QPainter p;

    p.begin( this );
    drawFrame( &p );
    p.end();
    emit update();
}


void ShowNextPiece::drawNextSquare(int x, int y,QColor *color)
{
    if (xOffset == -1)		/* Before first resizeEvent? */
        return;

    QPainter paint;
    paint.begin(this);
    drawTetrisButton( &paint, xOffset+x*blockWidth, yOffset+y*blockHeight,
					  blockWidth, blockHeight, color );
    paint.end();
}


/* MAIN */

int main( int argc, char **argv )
{
    KApplication a(argc, argv, "ktetris");
    KTetris *tetris = new KTetris;
    a.setMainWidget(tetris);
    tetris->show();
    return a.exec();
}
