/* Class AbTop */

#include "AbTop.h"
#include <qpopmenu.h>
#include <qkeycode.h>
#include <kapp.h>
#include <ktopwidget.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmsgbox.h>
#include <kstatusbar.h>
#include "Board.h"
#include "BoardWidget.h"

AbTop::AbTop()
{
  timerState = noGame;
  depth = 2;
  level_id = easy_id;
  iplay_id = red_id;
  iplay = cRed;
  moveNo = 0;
  showMoveLong = true;
  stop = false;

  setupMenu();

  char tmp[100];
  sprintf(tmp,"%s 000", klocale->translate("Move"));
  status = new KStatusBar(this);
  status->insertItem(tmp, 1);
  status->insertItem( klocale->translate("Press F2 for a new game"), 2);
  setStatusBar(status);
  updateStatus();

  timer = new QTimer;
  connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );

  board = new Board();

  connect( board, SIGNAL(searchBreak()), this, SLOT(searchBreak()) );

  CHECK_PTR(board);
  boardWidget = new BoardWidget(*board,this);

  connect( boardWidget, SIGNAL(moveChoosen(Move&)), 
	   this, SLOT(moveChoosen(Move&)) );

  setView(boardWidget);
  boardWidget->show();

  easy();
  play_red();

  show();
  setMinimumSize(150,200);
  resize(260,280);
}

/* let the program be responsive even in a long search... */
void AbTop::searchBreak()
{
  kapp->processEvents();
}

void AbTop::setupMenu()
{
  KMenuBar* menu;
  QPopupMenu *file, *_help;

  file = new QPopupMenu;
  CHECK_PTR( file );
  file->insertItem( klocale->translate("New Game"), 
		   this, SLOT(newGame()), Key_F2 );
  file->insertItem( klocale->translate("Stop Search"),
		   this, SLOT(stopSearch()), Key_S );
  file->insertSeparator();
  //  file->insertItem( "Stop", this, SLOT(stopGame()) );
  //  file->insertItem( "Continue", this, SLOT(continueGame()) );
  file->insertItem( klocale->translate("Take back"),
		   this, SLOT(takeBack()), Key_B );
  file->insertItem( klocale->translate("Hint"),
		   this, SLOT(suggestion()), Key_H );
  file->insertSeparator();
  file->insertItem( klocale->translate("Quit"), this, SLOT(quit()) );

  _level = new QPopupMenu;
  CHECK_PTR( _level );
  easy_id = _level->insertItem( klocale->translate("Easy"),
			       this, SLOT(easy()) );
  normal_id = _level->insertItem( klocale->translate("Normal"),
				 this, SLOT(normal()) );
  hard_id = _level->insertItem( klocale->translate("Hard"),
			       this, SLOT(hard()) );
  challange_id = _level->insertItem( klocale->translate("Challange"),
				    this, SLOT(challange()) );
  _level->setCheckable( TRUE );

  _iplay = new QPopupMenu;
  CHECK_PTR( _iplay );
  red_id = _iplay->insertItem( klocale->translate("Red"),
			      this, SLOT(play_red()) );
  yellow_id = _iplay->insertItem( klocale->translate("Yellow"),
				 this, SLOT(play_yellow()) );
  both_id = _iplay->insertItem( klocale->translate("Both"), 
			       this, SLOT(play_both()) );
  _iplay->setCheckable( TRUE );

  _options = new QPopupMenu;
  CHECK_PTR( _options );
  _options->insertItem( klocale->translate("Level"), _level );
  _options->insertItem( klocale->translate("Computer plays"), _iplay );
  _options->insertSeparator();
  slow_id = _options->insertItem( klocale->translate("Move slow"),
				  this, SLOT(changeShowMove()) );
  _options->setCheckable( TRUE );

  _options->setItemChecked(slow_id, showMoveLong);

  _help = new QPopupMenu();
  CHECK_PTR( _help );
  _help->insertItem( klocale->translate("About"), this, SLOT(about()) );
  _help->insertItem( klocale->translate("Rules"), this, SLOT(help()) );

  menu  = new KMenuBar(this);
  CHECK_PTR( menu );
  menu->insertItem( klocale->translate("File"), file);
  menu->insertItem( klocale->translate("Options"), _options);
  menu->insertSeparator();
  menu->insertItem( klocale->translate("Help"), _help);
  menu->show();

  setMenu(menu);
}

void AbTop::updateStatus()
{
  char tmp[50];

  if (timerState == noGame)
    strcpy(tmp,"");
  else
    sprintf(tmp,"%s %d", klocale->translate("Move"), moveNo/2 + 1);
  status->changeItem(tmp,1);

  if (timerState == noGame)
    strcpy(tmp,klocale->translate("Press F2 for a new game"));
  else {
    if (timerState == gameOver)
      sprintf(tmp,"%s %s !", (board->actColor() == Board::color2) ? 
	      klocale->translate("Red"):klocale->translate("Yellow"),
	      klocale->translate("won"));
    else
      sprintf(tmp,"%s - %s", (board->actColor() == Board::color1) ? 
	      klocale->translate("Red"):klocale->translate("Yellow"),
	      iPlayNow() ? klocale->translate("I am thinking...") : 
	                   klocale->translate("It's your turn !"));
  }
  status->changeItem(tmp,2);
}

void AbTop::timerDone()
{
  int interval = 400;

  switch(timerState) {
  case noGame:
  case notStarted:
    return;
  case showMove:
  case showMove+2:
  case showSugg:
  case showSugg+2:
  case showSugg+4:
    board->showMove(actMove, 2);
    interval = 200;
    break;
  case showMove+1:
  case showMove+3:
  case showSugg+1:
  case showSugg+3:
    board->showMove(actMove, 3);
    break;
  case showSugg+5:
    interval = 800;
  case showMove+4:
    board->showMove(actMove, 4);
    break;
  case showMove+5:
    board->showMove(actMove, 0);
    timerState = moveShown;
    playGame();
    return;
  case showSugg+6:
    board->showMove(actMove, 0);
    timerState = notStarted;
    boardWidget->draw();
    return;
  }
  boardWidget->draw();
  timerState++;
  timer->start(interval,TRUE);
}

void AbTop::userMove()
{
    /* User has to move */
    static MoveList list;
    
    list.clear();
    board->generateMoves(list);
    boardWidget->choseMove(&list);
}

bool AbTop::iPlayNow()
{
  int c = board->actColor();
    
  /* color1 is red */
  return ((iplay == cBoth) || 
	  ((c == Board::color1) && (iplay == cRed) ) ||
	  ((c == Board::color2) && (iplay == cYellow) ));
}

void AbTop::playGame()
{
  if (timerState == moveShown) {
    board->playMove(actMove);
    boardWidget->draw();
    timerState = notStarted;
  }
  if (!board->isValid()) { 
    stop = true;
    timerState = gameOver;
  }
  updateStatus();
      
  if (!iPlayNow()) {
    userMove();
    return;
  }
  if (stop) return;
	
  kapp->processEvents();
  if (moveNo++ <4) {
    /* Chose a random move making the position better for actual color */

    int v = board->calcValue(), vv;
    do {
      actMove = board->randomMove();
      board->playMove(actMove);
      vv = board->calcValue();
      board->takeBack();
    } while( (board->actColor() == Board::color1) ? (vv<v) : (vv>v) );
  }
  else
    actMove = (board->bestMove());

  timerState = showMoveLong ? showMove : showMove+3;
  timerDone();
}

void AbTop::moveChoosen(Move& m)
{
  actMove = m;
  timerState = moveShown;
  playGame();
}

void AbTop::newGame()
{
  timerState = notStarted;
  timer->stop();	
  board->begin(Board::color1);
  boardWidget->draw();
  moveNo = 0;
  stop = false;	
  playGame();
}

void AbTop::stopGame()
{
  stop = true;
  board->stopSearch();
}

void AbTop::stopSearch()
{
  board->stopSearch();
}

void AbTop::quit()
{
  board->stopSearch();
  kapp->quit();
}

void AbTop::continueGame()
{
  if (timerState != noGame && timerState != gameOver) {
    stop = false;
    playGame();
  }
}

void AbTop::takeBack()
{
  if (timerState != notStarted) return;
  if (moveNo > 1) {
    board->takeBack();
    board->takeBack();
    boardWidget->draw();
    moveNo -=2;
    updateStatus();
    userMove();
  }
}

void AbTop::suggestion()
{
  MoveList list;
  Move& m = board->nextMove();

  if (timerState != notStarted) return;
  
  board->generateMoves(list);
  if (!list.isElement(m,0)) return;

  actMove = m;

  timerState = showSugg;
  timerDone();
}

void AbTop::setLevel(int id, int d)
{
  _level->setItemChecked(level_id, false);
  _level->setItemChecked(level_id = id, true);
  depth = d;
  board->setDepth(depth);
}

void AbTop::easy()
{
  setLevel(easy_id, 2);
}

void AbTop::normal()
{
  setLevel(normal_id, 3);
}

void AbTop::hard()
{
  setLevel(hard_id, 4);
}

void AbTop::challange()
{
  setLevel(challange_id, 5);
}

void AbTop::setIplay(int id, int v)
{
  _iplay->setItemChecked(iplay_id, false);
  _iplay->setItemChecked(iplay_id = id, true);
  iplay = v;
  continueGame();
}

void AbTop::play_red()
{
  setIplay(red_id, cRed);
}

void AbTop::play_yellow()
{
  setIplay(yellow_id, cYellow);
}

void AbTop::play_both()
{
  setIplay(both_id, cBoth);
}

void AbTop::changeShowMove()
{
  showMoveLong = showMoveLong ? false:true;
  _options->setItemChecked(slow_id, showMoveLong);
}

void AbTop::help()
{
  kapp->invokeHTMLHelp("kabalone.html", "");
}

void AbTop::about()
{
    QString tmp;

    tmp.sprintf("KAbalone V 1.01 \n\n"
		"(C) 1997 Josef Weidendorfer\n"
		"<weidendo@informatik.tu-muenchen.de>");

    KMsgBox::message(this, klocale->translate("About"),
		     tmp,KMsgBox::INFORMATION, "OK");
}


#include "AbTop.moc"
