/* Yo Emacs, this is -*- C++ -*-
 *******************************************************************
 *******************************************************************
 *
 *
 * KREVERSI
 *
 *
 *******************************************************************
 *
 * A Reversi (or sometimes called Othello) game
 *
 *******************************************************************
 *
 * created 1997 by Mario Weilguni <mweilguni@sime.com>
 *
 *******************************************************************
 *
 * This file is part of the KDE project "KREVERSI"
 *
 * KREVERSI is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * KREVERSI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KREVERSI; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *******************************************************************
 */

#include <qdir.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <qaccel.h>
#include <qpopmenu.h>
#include <kmsgbox.h>
#include <kapp.h>
#include <kmsgbox.h>
#include <qpushbt.h>
#include <qlistbox.h>
#include <qdatetm.h>
#include <qlabel.h>
#include <qlined.h>
#include <kcolordlg.h>
#include <qregexp.h>
#include <qtimer.h>
#include "Score.h"
#include "app.h"
#include "about.h"
#include "playsound.h"
#include "ktablistbox.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

const int ID_FQUIT	= 104;

const int ID_GSTOP	= 200;
const int ID_GCONTINUE	= 201;
const int ID_GNEW	= 202;
const int ID_GUNDO	= 203;
const int ID_GSWITCH	= 204;
const int ID_GHIGHSCORES= 205;

const int ID_VZOOMIN	= 400;
const int ID_VZOOMOUT	= 401;
const int ID_VZOOMBASE	= 1000;
const int ID_VZOOM50	= 1050;
const int ID_VZOOM60	= 1060;
const int ID_VZOOM80	= 1080;
const int ID_VZOOM100	= 1100;
const int ID_VZOOM120	= 1120;
const int ID_VZOOM140	= 1140;
const int ID_VZOOM160	= 1160;
const int ID_VZOOM180	= 1180;
const int ID_VZOOM200	= 1200;

const int ID_OBASE	= 500;
const int ID_O2		= 502;
const int ID_O3		= 503;
const int ID_O4		= 504;
const int ID_O5		= 505;
const int ID_O6		= 506;
const int ID_O7		= 507;
const int ID_O8		= 508;
const int ID_OANIMATION = 509;
const int ID_OSPEED     = 510;
const int ID_COLOR	= 549;
const int ID_PIXMAP	= 550;
const int ID_OSOUND	= 580;
const int ID_OGSCALE	= 581;

const int ID_HHELP	= 901;
const int ID_HABOUT	= 902;
const int ID_HHINT	= 903;
const int ID_HRULES	= 904;
const int ID_HSTRATEGY	= 905;

const int SB_SCOREH	= 2;
const int SB_SCOREC	= 3;
const int SB_TURN       = 4;

extern QString PICDIR;

App::App() : KFixedTopWidget() {
  // create locale
  locale = kapp->getLocale();

  highscore.resize(0);
  readHighscore();
  setCaption("KReversi");

  // create reversi board
  b = new Board(this);
  createMenuBar();
  createToolBar();
  createStatusBar();

  b->setFixedSize(b->sizeHint());
  b->show();
  tb->show();
  sb->show();
  menu->show();
  setView(b);
  addToolBar(tb);
  setStatusBar(sb);
  setMenu(menu);

  connect(b, SIGNAL(score()), this, SLOT(slotScore()));
  connect(b, SIGNAL(gameWon(int)), this, SLOT(slotGameEnded(int)));
  connect(b, SIGNAL(turn(int)), this, SLOT(slotTurn(int)));
  connect(b, SIGNAL(strengthChanged(int)), this, SLOT(slotStrength(int)));
  connect(b, SIGNAL(statusChange(int)), this, SLOT(slotStatusChange(int)));
  connect(b, SIGNAL(illegalMove()), this, SLOT(slotIllegalMove()));

#define menuPosition
  connect(menu, SIGNAL(moved(menuPosition)),
	  this, SLOT(slotBarChanged()));
#undef menuPosition

#define BarPosition
  connect(tb, SIGNAL(moved(BarPosition)),
	  this, SLOT(slotBarChanged()));
#undef BarPosition

  KConfig *conf = kapp->getConfig();
  if(conf != NULL) {
    if(conf->readNumEntry("Background", -1) != -1) {
      int i = conf->readNumEntry("Background");
      if(i == 1) {
	int red, green, blue;
	QString s = conf->readEntry("BackgroundColor");
	sscanf((const char *)s, "%d %d %d", &red, &green, &blue);      
	b->setColor(QColor(red, green, blue));
      } else if(i == 2) {
	QString s = conf->readEntry("BackgroundPixmap");
	if(s.length() > 0) {
	  QPixmap bg((const char *)s);
	  if(bg.width())
	    b->setPixmap(bg);
	}
      }
    }
    
    if(conf->readNumEntry("Skill", -1) != -1)
      b->setStrength(conf->readNumEntry("Skill", -1));
    
    if(conf->readNumEntry("Zoom", -1) != -1) {      
      b->setZoom(conf->readNumEntry("Zoom", -1)); 
      b->setFixedSize(b->sizeHint());
      updateRects();
    }

    // set menubar position
    int mbpos = conf->readNumEntry("Menubar_Pos",
				   (int)(KMenuBar::Top));
    menu->setMenuBarPos((KMenuBar::menuPosition)mbpos);

    // set toolbar position
    int tbpos = conf->readNumEntry("Toolbar_1_Pos",
				   (int)(KToolBar::Top));
    tb->setBarPos((KToolBar::BarPosition)tbpos);
    
    if(conf->readNumEntry("AnimationSpeed", -1000) != -1000) 
      b->setAnimationSpeed(conf->readNumEntry("AnimationSpeed", 0)); 

    if(conf->readNumEntry("Grayscale", 0)) {
      b->loadChips("chips_mono.xpm");
      menu->setItemChecked(ID_OGSCALE, TRUE);
    }

#ifdef HAVE_MEDIATOOL
    if(conf->readNumEntry("Sound", 0) != 0) {
      initAudio();
      if(!audioOK()) {
	show();
	kapp->processEvents();
	KMsgBox::message(this, locale->translate("Error"),
			 locale->translate("A problem with the sound server "
			 "occured!\nCannot enable sound "
			 "support."), KMsgBox::STOP);	
      }
    } else
      doneAudio(); // just to be sure
#endif
  }
    
    updateRects();
}


App::~App() {
  if(tb)
    delete tb;
  if(menu)
    delete menu;
}

void App::createMenuBar() {
  menu = new KMenuBar(this);

  QPopupMenu *fm = new QPopupMenu;
  fm->insertItem(locale->translate("&Quit"), ID_FQUIT);

  QPopupMenu *gm = new QPopupMenu;
  gm->insertItem(locale->translate("Get &hint)"), ID_HHINT);
  gm->insertSeparator();  
  gm->insertItem(locale->translate("&Stop thinking"), ID_GSTOP);
  gm->insertItem(locale->translate("&Continue"), ID_GCONTINUE);
  gm->insertSeparator();
  gm->insertItem(locale->translate("&New game"), ID_GNEW);
  gm->insertSeparator();
  gm->insertItem(locale->translate("&Undo move"), ID_GUNDO);
  gm->insertItem(locale->translate("Switch si&des"), ID_GSWITCH);
  gm->insertSeparator();
  gm->insertItem(locale->translate("Hall Of &Fame..."), ID_GHIGHSCORES);
  
  QPopupMenu *om = new QPopupMenu;
  om->setCheckable(TRUE);
  QPopupMenu *om_s = new QPopupMenu;  
  om_s->setCheckable(TRUE);
  om_s->insertItem(locale->translate("Level 1 (Wimp)"), ID_O2);
  om_s->insertItem(locale->translate("Level 2 (Beginner)"), ID_O3);
  om_s->insertItem(locale->translate("Level 3 (Novice)"),  ID_O4);
  om_s->insertItem(locale->translate("Level 4 (Average)"),  ID_O5);
  om_s->insertItem(locale->translate("Level 5 (Good)"),  ID_O6);
  om_s->insertItem(locale->translate("Level 6 (Expert)"),  ID_O7);
  om_s->insertItem(locale->translate("Level 7 (Master)"),  ID_O8);
  om->insertItem(locale->translate("Skill"), om_s);
  om->insertSeparator();
  om->insertItem(locale->translate("&Shrink board"), ID_VZOOMOUT);
  om->insertItem(locale->translate("&Enlarge board"), ID_VZOOMIN);

  QPopupMenu *zm = new QPopupMenu;
  zm->setCheckable(TRUE);
  zm->insertItem(locale->translate("&Half size"), ID_VZOOM50);
  zm->insertItem(locale->translate("60%"), ID_VZOOM60);
  zm->insertItem(locale->translate("80%"), ID_VZOOM80);
  zm->insertItem(locale->translate("D&efault size"), ID_VZOOM100);
  zm->insertItem(locale->translate("120%"), ID_VZOOM120);
  zm->insertItem(locale->translate("140%"), ID_VZOOM140);
  zm->insertItem(locale->translate("160%"), ID_VZOOM160);
  zm->insertItem(locale->translate("180%"), ID_VZOOM180);
  zm->insertItem(locale->translate("&Double size"), ID_VZOOM200);
  om->insertItem(locale->translate("Set size"), zm);

  om->insertSeparator();
  om->insertItem(locale->translate("Select &background color..."), ID_COLOR);
  
  QPopupMenu *om_bg = new QPopupMenu;
  lookupBackgroundPixmaps();

  if(backgroundPixmaps.count() == 0)
    om_bg->insertItem(locale->translate("none"), ID_PIXMAP);
  else
    for(unsigned i = 0; i < backgroundPixmaps.count(); i++) {
      // since the filename may contain underscore, they
      // are replaced with spaces in the menu entry
      QString s(backgroundPixmaps.at(i)->baseName());
      s = s.replace(QRegExp("_"), " ");
      om_bg->insertItem((const char *)s, ID_PIXMAP + i);
    }
  
  om->insertItem(locale->translate("Select background image"), om_bg);
  om->insertItem(locale->translate("&Grayscale"), ID_OGSCALE);
  om->insertSeparator();
  om->insertItem(locale->translate("&Animations"), ID_OANIMATION);

  QPopupMenu *om_sp = new QPopupMenu;
  om_sp->setCheckable(TRUE);
  om_sp->insertItem(locale->translate("1 (fastest)"), ID_OSPEED+1);
  for(int i = ID_OSPEED+2; i < ID_OSPEED + 10; i++) {
    QString txt;
    txt.setNum(i - ID_OSPEED);
    om_sp->insertItem((const char *)txt, i);
  }
  om_sp->insertItem(locale->translate("10 (slowest)"), ID_OSPEED+10);
  om->insertItem(locale->translate("Animation speed"), om_sp, ID_OSPEED);
#ifdef HAVE_MEDIATOOL
  om->insertSeparator();
  om->insertItem(locale->translate("S&ound"), ID_OSOUND);
#endif

  QPopupMenu *hm = new QPopupMenu;
  hm->insertItem(locale->translate("&Help"), ID_HHELP);
  hm->insertItem(locale->translate("&Rules"), ID_HRULES);
  hm->insertItem(locale->translate("&Strategy hints"), ID_HSTRATEGY);
  hm->insertSeparator();
  hm->insertItem(locale->translate("&About..."), ID_HABOUT);

  menu->insertItem(locale->translate("&File"), fm);
  menu->insertItem(locale->translate("&Game"), gm);
  menu->insertItem(locale->translate("&Options"), om);
  menu->insertSeparator();
  menu->insertItem(locale->translate("&Help"), hm);
  connect(menu, SIGNAL(activated(int)), this, SLOT(processEvent(int)));

  menu->setAccel(CTRL+Key_Q, ID_FQUIT);
  menu->setAccel(Key_F1, ID_HHELP);
  menu->setAccel(Key_Escape, ID_GSTOP);
  menu->setAccel(CTRL+Key_N, ID_GNEW);
  menu->setAccel(CTRL+Key_U, ID_GUNDO);
  menu->setAccel(CTRL+Key_1, ID_O2);
  menu->setAccel(CTRL+Key_2, ID_O3);
  menu->setAccel(CTRL+Key_3, ID_O4);
  menu->setAccel(CTRL+Key_4, ID_O5);
  menu->setAccel(CTRL+Key_5, ID_O6);
  menu->setAccel(CTRL+Key_6, ID_O7);
  menu->setAccel(CTRL+Key_7, ID_O8);

  menu->setAccel(CTRL + Key_Minus, ID_VZOOMOUT);
  menu->setAccel(CTRL + Key_Plus, ID_VZOOMIN);

  // if no background pixmaps were found, disable
  // the "none" entry
  menu->setItemEnabled(ID_PIXMAP, backgroundPixmaps.count()!=0);
}


void App::createToolBar() {
  tb = new KToolBar(this);
  QPixmap p1((const char *)(PICDIR + "stop.xpm"));
  QPixmap p2((const char *)(PICDIR + "zoomin.xpm"));
  QPixmap p3((const char *)(PICDIR + "zoomout.xpm"));
  QPixmap p4((const char *)(PICDIR + "undo.xpm"));
  QPixmap p5((const char *)(PICDIR + "hint.xpm"));
  QPixmap p6((const char *)(PICDIR + "help.xpm"));
  tb->insertButton(p1, ID_GSTOP, TRUE, locale->translate("Stop thinking"));
  tb->insertButton(p4, ID_GUNDO, TRUE, locale->translate("Undo move"));
  tb->insertButton(p3, ID_VZOOMOUT, TRUE, locale->translate("Shrink board"));
  tb->insertButton(p2, ID_VZOOMIN, TRUE, locale->translate("Enlarge board"));  
  tb->insertButton(p5, ID_HHINT, TRUE, locale->translate("Get hint"));
  tb->insertButton(p6, ID_HHELP, TRUE, locale->translate("Get help"));
  connect(tb, SIGNAL(clicked(int)), this, SLOT(processEvent(int)));  
}


void App::createStatusBar() {
  sb = new KStatusBar(this);
  sb->insertItem(locale->translate("XXXXX's turn"), SB_TURN);
  sb->insertItem(locale->translate("You (XXXXX): 88"), SB_SCOREH);
  sb->insertItem(locale->translate("Computer (XXXXX): 88"), SB_SCOREC);
}


void App::lookupBackgroundPixmaps() {  
  QDir dir((const char *)(PICDIR + "background/"), "*.xpm");
  if(!dir.exists())
    return;

  const QFileInfoList *fl = dir.entryInfoList();

  // sanity check, maybe the directory is unreadable
  if(fl == NULL)
    return;

  QFileInfoListIterator it( *fl );
  QFileInfo *fi;

  while((fi = it.current())) {
    backgroundPixmaps.append(new QFileInfo(*fi));
    ++it;
  }
}

void App::processEvent(int itemid) {
  QString s;
  QColor c;

  switch(itemid) {
  case ID_FQUIT:
    b->interrupt();

    // interesting, without this kreversi seems to segfault
    delete this;
    kapp->quit();
    return;
    break;

  case ID_GNEW:
    b->newGame();
    break;

  case ID_GCONTINUE:
    b->doContinue();
    break;

  case ID_GSTOP:
    b->interrupt();
    break;

  case ID_GUNDO:
    b->undo();
    break;
    
  case ID_GSWITCH:
    b->switchSides();
    break;

  case ID_GHIGHSCORES:
    showHighscore();
    break;

  case ID_VZOOMIN:
    b->zoomIn();
    b->setFixedSize(b->sizeHint());
    updateRects();
    kapp->getConfig()->writeEntry("Zoom", b->getZoom());
    break;
    
  case ID_VZOOMOUT:
    b->zoomOut();
    b->setFixedSize(b->sizeHint());
    updateRects();
    kapp->getConfig()->writeEntry("Zoom", b->getZoom());
    break;

  case ID_VZOOM50:
  case ID_VZOOM60:
  case ID_VZOOM80:
  case ID_VZOOM100:
  case ID_VZOOM120:
  case ID_VZOOM140:
  case ID_VZOOM160:
  case ID_VZOOM180:
  case ID_VZOOM200:
    b->setZoom(itemid - ID_VZOOMBASE);
    b->setFixedSize(b->sizeHint());
    updateRects();
    kapp->getConfig()->writeEntry("Zoom", b->getZoom());
    break;

  case ID_OANIMATION:
    b->setAnimationSpeed(-b->animationSpeed());
    kapp->getConfig()->writeEntry("AnimationSpeed", b->animationSpeed());
    break;

  case ID_OGSCALE:
    {
      bool gs;
      
      if(b->chipsName() == "chips.xpm") {
	b->loadChips("chips_mono.xpm");
	gs = TRUE;
      } else {
	b->loadChips("chips.xpm");
	gs = FALSE;
      }
      menu->setItemChecked(ID_OGSCALE, gs);
      kapp->getConfig()->writeEntry("Grayscale", gs);
    }

  case ID_O2:
  case ID_O3:
  case ID_O4:
  case ID_O5:
  case ID_O6:
  case ID_O7:
  case ID_O8:
    b->setStrength(itemid - ID_OBASE);
    kapp->getConfig()->writeEntry("Skill", itemid - ID_OBASE);
    break;

  case ID_COLOR:
    {
      if(KColorDialog::getColor(c)) {
	b->setColor(c);
	kapp->getConfig()->writeEntry("Background", 1);
	s.sprintf("%d %d %d", c.red(), c.green(), c.blue());
	kapp->getConfig()->writeEntry("BackgroundColor", (const char *)s);
      }
    };
    break;

#ifdef HAVE_MEDIATOOL
  case ID_OSOUND:
    if(!audioOK()) {
      initAudio();
      if(!audioOK()) {
	KMsgBox::message(this, locale->translate("Error"), 
			 locale->translate("A problem with the sound server "
			 "occured!\nCannot enable sound "
			 "support."), KMsgBox::STOP);
	kapp->getConfig()->writeEntry("Sound", 0);
      } else 
	kapp->getConfig()->writeEntry("Sound", 1);  
    } else {
      doneAudio();
      kapp->getConfig()->writeEntry("Sound", 0);
    }
    break;
#endif

  case ID_HHINT:
    b->hint();
    break;

  case ID_HHELP:
    KApplication::getKApplication()->invokeHTMLHelp("", "");
    break;

  case ID_HRULES:
    KApplication::getKApplication()->invokeHTMLHelp("", "rules");
    break;

  case ID_HSTRATEGY:
    KApplication::getKApplication()->invokeHTMLHelp("", "strategy");
    break;

  case ID_HABOUT:
    {
      QDialog *dlg = new About(0);
      dlg->exec();
      delete dlg;
    }
  break;

  default:
    {
      if((itemid >= ID_PIXMAP) && (itemid < ID_PIXMAP + (int)backgroundPixmaps.count())) {
	QPixmap pm(backgroundPixmaps.at(itemid - ID_PIXMAP)->filePath());
	b->setPixmap(pm);
	kapp->getConfig()->writeEntry("Background", 2);
	s.sprintf("%d %d %d", c.red(), c.green(), c.blue());
	kapp->getConfig()->writeEntry("BackgroundPixmap",
				      backgroundPixmaps.at(itemid - ID_PIXMAP)->filePath());
      } else if((itemid >= ID_OSPEED) && (itemid <= ID_OSPEED + 10)) {
	b->setAnimationSpeed(itemid - ID_OSPEED);
	kapp->getConfig()->writeEntry("AnimationSpeed", b->animationSpeed());
      } else
	KMsgBox::message(this, locale->translate("Information"), locale->translate("not yet implemented"));
    }
  }
  enableItems();
}


void App::slotScore() {
  int black, white;
  QString s1, s2;

  b->getScore(black, white);
  if(b->humanIs() == Score::BLACK) {
    s1.sprintf(locale->translate("You (%s): %d"), 
	       locale->translate("blue"), black);
    s2.sprintf(locale->translate("Computer (%s): %d"), 
	       locale->translate("red"), white);
  } else {
    s2.sprintf(locale->translate("You (%s): %d"), 
	       locale->translate("red"), white);
    s1.sprintf(locale->translate("Computer (%s): %d"), 
	       locale->translate("blue"), black);
  }

  sb->changeItem((char *)(const char *)s1, SB_SCOREH);
  sb->changeItem((char *)(const char *)s2, SB_SCOREC);
  enableItems();
}


void App::enableItems() {
  int i;

  menu->setItemEnabled(ID_VZOOMIN, b->canZoomIn());
  menu->setItemEnabled(ID_VZOOMOUT, b->canZoomOut());
  menu->setItemEnabled(ID_GSTOP, (bool)(b->getState() == Board::THINKING));
  tb->setItemEnabled(ID_VZOOMIN, b->canZoomIn());
  tb->setItemEnabled(ID_VZOOMOUT, b->canZoomOut());
  tb->setItemEnabled(ID_GSTOP, (bool)(b->getState() == Board::THINKING));  
  menu->setItemEnabled(ID_GCONTINUE, b->interrupted());

  // check size entry
  for(i = ID_VZOOM50; i < ID_VZOOM200; i += 10)
    menu->setItemChecked(i, (b->getZoom() + ID_VZOOMBASE == i));

  // check animation speed
  menu->setItemChecked(ID_OANIMATION, (bool)(b->animationSpeed() > 0));
  for(i = ID_OSPEED+1; i <= ID_OSPEED + 10; i++)
    menu->setItemChecked(i, (bool)(b->animationSpeed() == i - ID_OSPEED));
  menu->setItemEnabled(ID_OSPEED, (bool)(b->animationSpeed() > 0));
  menu->setItemEnabled(ID_GUNDO, (bool)(b->getMoveNumber() != 0));
  menu->setItemEnabled(ID_GNEW, (bool)(b->getMoveNumber() != 0));
  tb->setItemEnabled(ID_GUNDO, (bool)(b->getMoveNumber() != 0));

  // check sound
#ifdef HAVE_MEDIATOOL
  menu->setItemChecked(ID_OSOUND, (bool)(audioOK()));
#endif
}


void App::slotGameEnded(int color) {
  QString s;
  int winner, loser;

  sb->changeItem(locale->translate("End of game"), SB_TURN);
    
  // get the score
  if(color == Score::BLACK) 
    b->getScore(winner, loser);
  else
    b->getScore(loser, winner);
  
  if(color == Score::NOBODY) {
    playSound("drawn.wav");
    s.sprintf(locale->translate("Game is drawn!\n\nYou     : %d\nComputer: %d"), winner, loser);
    KMsgBox::message(this, locale->translate("Game ended"), (const char *)s);
  } else if(b->humanIs() == color) {
    // calculate score
    int  st = b->getStrength();
    int sum = winner + loser;    
    float score= (float)winner / sum * 
                 (st - (ID_O2 - ID_OBASE) + 1) / (ID_O8 - ID_O2 + 1) * 
                 100.0;

    playSound("won.wav");
    s.sprintf(locale->translate("Congratulations, you have won!\n\nYou     : %d\nComputer: %d\nYour rating %4.1f%%"), 
	      winner, loser, score);
    KMsgBox::message(this, locale->translate("Game ended"), (const char *)s);

    // create highscore entry
    HighScore hs;
    QString name = getPlayerName();
    strncpy(hs.name, (const char *)name, sizeof(hs.name) - 1);
    hs.color = b->humanIs();
    hs.winner = winner;
    hs.loser = loser;
    hs.rating = score;   
    hs.date = time(NULL);

    int rank = insertHighscore(hs);
    if(rank != -1) {
      showHighscore(rank);
    }
  } else {
    playSound("lost.wav");
    s.sprintf(locale->translate("You have lost the game!\n\nYou     : %d\nComputer: %d"), 
	      loser, winner);
    KMsgBox::message(this, locale->translate("Game ended"), (const char *)s);
  }
}


void App::slotTurn(int color) {
  QString s;

  if(color == Score::WHITE)
    s.sprintf("%s %s", locale->translate("Red's"), locale->translate("turn"));
  else if(color == Score::BLACK)
    s.sprintf("%s %s", locale->translate("Blue's"), locale->translate("turn"));
  else
    s = "";
  sb->changeItem(s.data(), SB_TURN);
}


void App::slotStrength(int st) {
  for(int i = ID_O2; i <= ID_O8; i++)
    menu->setItemChecked(i, (bool)(i == ID_OBASE + st));
}


void App::slotStatusChange(int) {
  bool oldstate = (bool)(b->getState() == Board::THINKING);
  enableItems();
  bool newstate = (bool)(b->getState() == Board::THINKING);

  if(newstate != oldstate) {
    if(newstate) // we are thinking now
      kapp->setOverrideCursor(waitCursor);
    if(oldstate) // stopped thinking
      kapp->restoreOverrideCursor();
  }
}


void App::slotIllegalMove() {
  if(!audioOK())
    kapp->beep();
  else
    playSound("illegalmove.wav");
}


bool App::isBetter(HighScore &hs, HighScore &than) {
  if(hs.rating > than.rating)
    return TRUE;
  else if(hs.rating < than.rating)
    return FALSE;
  // better if less pieces are set
  else if(hs.winner + hs.loser < than.winner + than.loser)
    return TRUE;
  else
    return FALSE;
}


int App::insertHighscore(HighScore &hs) {
  int i;

  if(highscore.size() == 0) {
    highscore.resize(1);
    highscore[0] = hs;
    writeHighscore();
    return 0;
  } else {
    HighScore last = highscore[highscore.size() - 1];
    if(isBetter(hs, last) || (highscore.size() < HIGHSCORE_MAX)) {
      if(highscore.size() == HIGHSCORE_MAX)
	highscore[HIGHSCORE_MAX - 1] = hs;
      else {
	highscore.resize(highscore.size()+1);
	highscore[highscore.size() - 1] = hs;
      }

      // sort in new entry
      int bestsofar = highscore.size() - 1;
      for(i = highscore.size() - 1; i > 0; i--)
	if(highscore[i-1].rating < highscore[i].rating) {
	  // swap entries
	  HighScore temp = highscore[i-1];
	  highscore[i-1] = highscore[i];
	  highscore[i] = temp;
	  bestsofar = i - 1;
	}

      writeHighscore();
      return bestsofar;
    }
  }
  return -1;
}


void App::readHighscore() {
  int i;
  QString s, e, grp;
  KConfig *conf = kapp->getConfig();

  highscore.resize(0);
  i = 0;
  bool eol = FALSE;
  grp = conf->getGroup();
  conf->setGroup(QString("Hall of Fame"));
  while ((i < HIGHSCORE_MAX) && !eol) {
    s.sprintf("Highscore_%d", i);
    if(conf->hasKey(s)) {
      e = conf->readEntry(s);
      highscore.resize(i+1);

      HighScore hs;
      sscanf((const char *)e, "%s %d %d %d %f %ld", 
	     (char *)&hs.name, &hs.color, &hs.winner, 
	     &hs.loser, &hs.rating, &hs.date);
      highscore[i] = hs;
    } else
      eol = TRUE;
    i++;
  }

  // restore old group
  conf->setGroup(grp);
}


void App::writeHighscore() {
  int i;
  QString s, e, grp;
  KConfig *conf = kapp->getConfig();

  grp = conf->getGroup();
  conf->setGroup(QString("Hall of Fame"));
  for(i = 0; i < (int)highscore.size(); i++) {
    s.sprintf("Highscore_%d", i);
    HighScore hs = highscore[i];
    e.sprintf("%s %d %d %d %f %ld", 
	      hs.name, hs.color, hs.winner,
	      hs.loser, hs.rating, hs.date);
    conf->writeEntry(s, e);
  }
  
  // restore old group
  conf->setGroup(grp);
}

int MAX(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

void App::showHighscore(int focusItem = -1) {
  // this may look a little bit confusing...
  QDialog *dlg = new QDialog(0, locale->translate("Hall of Fame"), TRUE);
  QPushButton *b = new QPushButton(locale->translate("Close"), dlg);
  dlg->resize(550, 400);
  dlg->setCaption(locale->translate("KReversi: Hall Of Fame"));
  
  QLabel *l = new QLabel(locale->translate("KReversi: Hall Of Fame"), dlg);
  l->setFont(QFont("Helvetica", 24, QFont::Bold));
  l->setAlignment(AlignCenter);

  // insert listbox
  //QListBox *lb = new QListBox(dlg);
  KTabListBox *lb = new KTabListBox(dlg);
  lb->setNumCols(5);  
  lb->setColumn(0, "Rank");
  lb->setColumn(1, "Name");
  lb->setColumn(2, "Color");
  lb->setColumn(3, "Score");
  lb->setColumn(4, "Rating");

  // calculate the width of the fields
  int w = 0;
  int t;
  QFontMetrics fm = dlg->fontMetrics();
#define WIDTH(str) (fm.boundingRect((str)).width() + 24)

  t = WIDTH(locale->translate("Rank"));
  w += t;
  lb->setColumnWidth(0, t);

  t = WIDTH("XXXXXXXXXXXXXXX");
  w += t;
  lb->setColumnWidth(1, t);

  t = MAX(MAX(WIDTH(locale->translate("blue")),
	  WIDTH(locale->translate("red"))),
	  WIDTH(locale->translate("Color")));
  w += t;
  lb->setColumnWidth(2, t);
  
  t = WIDTH(locale->translate("Score"));
  w += t;
  lb->setColumnWidth(3, t);
  
  t = WIDTH(locale->translate("Rating"));
  w += t;
  lb->setColumnWidth(4, t);

  dlg->resize(w + 32, w+16);
  lb->setGeometry(12, 70, w+8, w - 120);
  
  // title geometry
  l->setGeometry(0, 10, w+32, 40);

  // button geometry
  b->adjustSize();  
  b->move((w+32 - b->width())/2, dlg->height() - b->height() - 24);

  lb->setSeparator('\t');
  QString s;
  for(int i = 0; i < (int)highscore.size(); i++) {
    HighScore hs = highscore[i];
    const char *color;

    if(hs.color == Score::BLACK)
      color = locale->translate("blue");
    else
      color = locale->translate("red");

    s.sprintf("%d.\t%s\t%s\t%d/%d\t%6.1f%%\t",
 	      i+1, hs.name, color, hs.winner, 
	      hs.loser, hs.rating);
    lb->insertItem((const char *)s);
  }

  // connect the "Close"-button to done
  connect(b, SIGNAL(released()), dlg, SLOT(accept()));
  b->setAutoDefault(TRUE);
  b->setFocus();
  if(focusItem != -1)
    lb->setCurrentItem(focusItem);
  playSound("hof.wav");
  dlg->exec();
  delete dlg;
}


QString App::getPlayerName() {
  QDialog *dlg = new QDialog(this, "Hall Of Fame", TRUE);
  dlg->resize(300, 175);
  QLabel  *l1  = new QLabel(locale->translate("You've made in into the \"Hall Of Fame\". Type in your name so mankind will always remember your cool rating."), dlg);
  l1->setAlignment(AlignVCenter | AlignLeft | WordBreak);
  l1->setGeometry(10, 10, 280, 50);

  QFontMetrics fm = dlg->fontMetrics();

  QLabel *l2 = new QLabel(locale->translate("Your name:"), dlg);
  l2->setAlignment(AlignRight|AlignVCenter);
  l2->adjustSize();

  QLineEdit *e = new QLineEdit(dlg);
  e->adjustSize();
  e->setGeometry(10 + l2->width() + 10, 80, 
		 300 - 40 -l2->width(), e->height());
  e->setFocus();
  l2->setGeometry(10, 80, l2->width(), e->height());

  QPushButton *b = new QPushButton(locale->translate("Ok"), dlg);
  b->adjustSize();
  b->setGeometry(110, 125, 80, 32);
  b->setDefault(TRUE);
  b->setAutoDefault(TRUE);
  connect(b, SIGNAL(released()), dlg, SLOT(accept()));
  QAccel *accel = new QAccel(dlg);
  accel->insertItem(ASCII_ACCEL + Key_Enter, 1000);
  accel->connectItem(1000, dlg, SLOT(accept()));
  connect(e, SIGNAL(returnPressed()), dlg, SLOT(accept()));
  dlg->exec();

  QString s = e->text();
  delete dlg;

  if(s.length() == 0)
    s = " ";
  return s;
}

void App::slotBarChanged() {
  KConfig *conf = kapp->getConfig();
  if(conf) {
    conf->writeEntry(QString("Menubar_Pos"),
		     (int)(menu->menuBarPos()));
    conf->writeEntry(QString("Toolbar_1_Pos"),
		     (int)(tb->barPos()));
  }
}

#include "app.moc"
