#include "field.h"

#include <kkeyconf.h>
#include "gtboard.h"
#include "dialogs.h"

#include "field.moc"

Field::Field(QWidget *parent, const char *name)
: QWidget(parent, name)
{
	net_obj = 0;
	
	installEventFilter(this);
	
	board = new GTBoard(this);
	//	board = new GGBoard(this);
	board->installEventFilter(this);
	
	connect( board, SIGNAL(showOpponents()), SLOT(showOpponents()) );
	connect( board, SIGNAL(updateOpponents()), SLOT(updateOpponents()) );

	/* keys connections */
	kKeys->addKey(i18n("Quit"), "CTRL+Q");
	kKeys->addKey(i18n("New game"), "F2");
	kKeys->addKey(i18n("Pause game"), "P");
	kKeys->addKey(i18n("Help"), "F1");
	kKeys->addKey(i18n("High scores"), "H");
	kKeys->addKey(i18n("Move left"), "Left");
	kKeys->addKey(i18n("Move right"), "Right");
	kKeys->addKey(i18n("Drop down"), "Down");
	kKeys->addKey(i18n("One line down"), "Shift");
	kKeys->addKey(i18n("Rotate left"), "Up");
	kKeys->addKey(i18n("Rotate right"), "Return");
	kKeys->addKey(i18n("Close dialog"), "Return");
	kKeys->addKey(i18n("Ok dialog"), "Return");
	kKeys->addKey(i18n("Cancel dialog"), "Escape");
	                                                                   
	kKeys->registerWidget(K_MAIN, this);
	kKeys->connectFunction(K_MAIN, i18n("Quit"), parent, SLOT(quit()));
	kKeys->connectFunction(K_MAIN, i18n("New game"), board, SLOT(start()));
	kKeys->connectFunction(K_MAIN, i18n("Pause game"), board, SLOT(pause()));
	kKeys->connectFunction(K_MAIN, i18n("High scores"), board, SLOT(showHighScores()));
	/* the following connections are disactivated */
	kKeys->connectFunction(K_MAIN, i18n("Move left"), board, SLOT(pMoveLeft()), FALSE);
	kKeys->connectFunction(K_MAIN, i18n("Move right"), board, SLOT(pMoveRight()), FALSE);
	kKeys->connectFunction(K_MAIN, i18n("Drop down"), board, SLOT(pDropDown()), FALSE);
	kKeys->connectFunction(K_MAIN, i18n("One line down"), board, SLOT(pOneLineDown()), FALSE);
	kKeys->connectFunction(K_MAIN, i18n("Rotate left"), board, SLOT(pRotateLeft()), FALSE);
	kKeys->connectFunction(K_MAIN, i18n("Rotate right"), board, SLOT(pRotateRight()), FALSE);
	
	lScore = new QLabel(i18n("Score"), this);
	showScore = new QLCDNumber(6,this);
	showScore->display( 0 );
	
	lLines = new QLabel(i18n("Lines removed"), this);
	showLines = new QLCDNumber(3,this);
	showLines->display( 0 );
	
	lLevel = new QLabel(i18n("Level"), this);
	showLevel = new QLCDNumber(2,this);
	showLevel->display( 0 );
	
	lNext = new QLabel(i18n("Next tile"), this);
	showNext = new ShowNextPiece(this);
	
	labPrevName = new QLabel("", this);
	labPrevName->hide();
	prevHeight = new KProgress(0, 22, 0, KProgress::Vertical, this);
	prevHeight->setBackgroundColor(lightGray);
	prevHeight->setTextEnabled(FALSE);
	prevHeight->setBarColor(blue);
	prevHeight->hide();
	
	labNextName = new QLabel("", this);
	labNextName->hide();
	nextHeight = new KProgress(0, 22, 0, KProgress::Vertical, this);
	nextHeight->setBackgroundColor(lightGray);
	nextHeight->setTextEnabled(FALSE);
	nextHeight->setBarColor(blue);
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
	
	board->setGeometry( SIDE_W + LCD_W + MID_W, 0, BOARD_W, BOARD_H );
	lScore->setGeometry( SIDE_W, TOP_H, LABEL_W, LABEL_H );
	showScore->setGeometry( SIDE_W, TOP_H + LABEL_H, LCD_W, LCD_H );	
	lLines->setGeometry( SIDE_W, TOP_H + LABEL_H + LCD_H + MID_H, LABEL_W, LABEL_H );
	showLines->setGeometry( SIDE_W, TOP_H + 2*LABEL_H + LCD_H + MID_H, LCD_W, LCD_H );
	lLevel->setGeometry( SIDE_W, TOP_H + 2*LABEL_H + 2*LCD_H + 2*MID_H, LABEL_W, LABEL_H );
	showLevel->setGeometry( SIDE_W, TOP_H + 3*LABEL_H + 2*LCD_H + 2*MID_H, LCD_W, LCD_H );
	lNext->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W, TOP_H, NTILE_W, LABEL_H );
	showNext->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W, TOP_H + LABEL_H, NTILE_W, NTILE_H );
	labPrevName->setGeometry( SIDE_W, TOP_H + BOARD_H + BASE_H - LCD_H - LABEL_H - MID_H, LABEL_W, LABEL_H );
	prevHeight->setGeometry( SIDE_W + LCD_W + (MID_W-BAR_W)/2, 0, BAR_W, BOARD_H);
	labNextName->setGeometry( SIDE_W + 2*MID_W + LCD_W + BOARD_W, TOP_H + BOARD_H + BASE_H - LCD_H - LABEL_H - MID_H, LABEL_W, LABEL_H );
	nextHeight->setGeometry( SIDE_W + LCD_W + MID_W + BOARD_W +(MID_W-BAR_W)/2, 0, BAR_W, BOARD_H);
	
	setFixedSize( 2*SIDE_W + 2*MID_W + BOARD_W + LCD_W + NTILE_W,
				 TOP_H + BOARD_H + BASE_H );
}


Field::~Field()
{
	if (net_obj) delete net_obj;
}


QSize Field::sizeHint() const
{
	return QSize(board->width(), board->height());
}


bool Field::eventFilter(QObject *, QEvent *e)
{
	if ( e->type()!=Event_MouseButtonPress ) return FALSE;
	
	QMouseEvent *em = (QMouseEvent*) e;
	if ( em->button()!=RightButton ) return FALSE;
	
	popup->popup(QCursor::pos());
	return TRUE;
}


void Field::multiGame()
{
	if (net_obj) {
		delete net_obj;
		net_obj = 0;
	}
	
	NetDialog md(this);
	if ( md.exec() ) {
		net_obj = md.netObject();
		board->initMultiGame(net_obj);
	}
}


void Field::showOpponents()
{
	labPrevName->show();
	labNextName->show();
	prevHeight->show();
	nextHeight->show();
}


void Field::updateOpponents()
{
	labPrevName->setText( (const char *)net_obj->getPrevName() );
	prevHeight->setValue( net_obj->getPrevHeight() );
	labNextName->setText( (const char *)net_obj->getNextName() );
	nextHeight->setValue( net_obj->getNextHeight() );
}


void Field::fillPopup(QPopupMenu *pop)
{
	popup = pop;
	pop->insertSeparator();
	pop->insertItem(i18n("&Restart game"), board, SLOT(start()) );
	pop->insertItem(i18n("&Pause game"), board, SLOT(pause()) );
	pop->insertItem(i18n("&Multiplayer game"), this, SLOT(multiGame()) );
	pop->insertSeparator();
	pop->insertItem(i18n("&High scores"), board, SLOT(showHighScores()) );
	pop->insertSeparator();
}
