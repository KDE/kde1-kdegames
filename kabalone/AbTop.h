/* Class AbTop: the toplevel widget of KAbalone
 * 
 * Josef Weidendorfer, 9/97
*/

#ifndef _ABTOP_H_
#define _ABTOP_H_

// #define STDC_HEADERS
#include <qtimer.h>
#include <kapp.h>
#include <kstatusbar.h>
#include <ktopwidget.h>
#include "Board.h"
#include "BoardWidget.h"

class AbTop: public KTopLevelWidget
{
  Q_OBJECT;

public:
  AbTop();

  /* timer states */
  enum { noGame, gameOver, notStarted, moveShown,
	 showMove = 100, showSugg=200
  };

public slots:
  void timerDone();
  void newGame();
  void stopGame();
  void continueGame();
  void takeBack();
  void suggestion();
  void easy();
  void stopSearch();
  void normal();
  void hard();
  void challange();
  void changeShowMove();
  void help();
  void about();
  void quit();
  void searchBreak();
  void moveChoosen(Move&);
  void play_red();
  void play_yellow();
  void play_both();

private:
  void updateStatus();
  void userMove();
  void playGame();
  void setupMenu();
  void setLevel(int,int);
  void setIplay(int,int);
  bool iPlayNow();

  Move actMove;
  Board* board;
  BoardWidget *boardWidget;
  QTimer *timer;
  KStatusBar *status;
  int timerState;
  int depth, moveNo;
  bool showMoveLong, stop;

  int iplay;
  enum { none, cYellow, cRed, cBoth };

  QPopupMenu *_options, *_level, *_iplay;
  int easy_id, normal_id, hard_id, challange_id, slow_id, level_id;
  int yellow_id, red_id, both_id, iplay_id;
};

#endif /* _ABTOP_H_ */
