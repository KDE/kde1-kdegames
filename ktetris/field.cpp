#include "field.h"

#include <kkeyconf.h>
#include "gtboard.h"
#include "dialogs.h"

#include "field.moc"

Field::Field(QWidget *parent, const char *name)
: QWidget(parent, name)
{
	net_obj = 0; /* no net object */
	
	installEventFilter(this);
	
	board = new GTBoard(this);
	//	board = new GGBoard(this);
	board->installEventFilter(this);
	
	connect( this, SIGNAL(initMultiGame(NetObject *)),
			board, SLOT(initMultiGame(NetObject *)) );
	connect( board, SIGNAL(showOpponents()), SLOT(showOpponents()) );
	connect( board, SIGNAL(updateOpponents()), SLOT(updateOpponents()) );

	/* keys connections */
	kKeys->addKey(klocale->translate("Quit"), "CTRL+Q");
	kKeys->addKey(klocale->translate("New game"), "F2");
	kKeys->addKey(klocale->translate("Pause game"), "P");
	kKeys->addKey(klocale->translate("Help"), "F1");
	kKeys->addKey(klocale->translate("High scores"), "H");
	kKeys->addKey(klocale->translate("Move left"), "Left");
	kKeys->addKey(klocale->translate("Move right"), "Right");
	kKeys->addKey(klocale->translate("Drop down"), "Down");
	kKeys->addKey(klocale->translate("One line down"), "Shift");
	kKeys->addKey(klocale->translate("Rotate left"), "Up");
	kKeys->addKey(klocale->translate("Rotate right"), "Return");
	kKeys->addKey(klocale->translate("Close dialog"), "Return");
	kKeys->addKey(klocale->translate("Ok dialog"), "Return");
	kKeys->addKey(klocale->translate("Cancel dialog"), "Escape");
	                                                                   
	kKeys->registerWidget(K_MAIN, this);
	kKeys->connectFunction(K_MAIN, klocale->translate("Quit"), parent, SLOT(quit()));
	kKeys->connectFunction(K_MAIN, klocale->translate("New game"), board, SLOT(start()));
	kKeys->connectFunction(K_MAIN, klocale->translate("Pause game"), board, SLOT(pause()));
	kKeys->connectFunction(K_MAIN, klocale->translate("Help"), parent, SLOT(help()));
	kKeys->connectFunction(K_MAIN, klocale->translate("High scores"), board, SLOT(showHighScores()));
	/* the following connections are disactivated */
	kKeys->connectFunction(K_MAIN, klocale->translate("Move left"), board, SLOT(pMoveLeft()), FALSE);
	kKeys->connectFunction(K_MAIN, klocale->translate("Move right"), board, SLOT(pMoveRight()), FALSE);
	kKeys->connectFunction(K_MAIN, klocale->translate("Drop down"), board, SLOT(pDropDown()), FALSE);
	kKeys->connectFunction(K_MAIN, klocale->translate("One line down"), board, SLOT(pOneLineDown()), FALSE);
	kKeys->connectFunction(K_MAIN, klocale->translate("Rotate left"), board, SLOT(pRotateLeft()), FALSE);
	kKeys->connectFunction(K_MAIN, klocale->translate("Rotate right"), board, SLOT(pRotateRight()), FALSE);
	
	lScore = new QLabel(klocale->translate("Score"), this);
	showScore = new QLCDNumber(5,this);
	showScore->display( 0 );
	
	lLines = new QLabel(klocale->translate("Lines removed"), this);
	showLines = new QLCDNumber(5,this);
	showLines->display( 0 );
	
	lLevel = new QLabel(klocale->translate("Level"), this);
	showLevel = new QLCDNumber(2,this);
	showLevel->display( 0 );
	
	lNext = new QLabel(klocale->translate("Next tile"), this);
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
	/* reinit the board */
	board->hideBoard(TRUE);
	
	netDialog *md = new netDialog(&net_obj, this);
	
	if ( md->exec() )
		emit initMultiGame(net_obj);
	
	delete md;
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
	pop->insertItem(klocale->translate("&Restart game"), board, SLOT(start()) );
	pop->insertItem(klocale->translate("&Pause game"), board, SLOT(pause()) );
	pop->insertItem(klocale->translate("&Multiplayer game"), this, SLOT(multiGame()) );
	pop->insertSeparator();
	pop->insertItem(klocale->translate("&High scores"), board, SLOT(showHighScores()) );
	pop->insertSeparator();
}
