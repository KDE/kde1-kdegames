/* Yo Emacs, this is -*- C++ -*- */
/*
 *   ksame 0.4 - simple Game
 *   Copyright (C) 1997,1998  Marcus Kreutzberger
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *    
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <kapp.h>
#include <qaccel.h>
#include <qwidget.h>
#include <qpushbt.h>
#include <qpixmap.h>
#include <kmsgbox.h> 
#include <kiconloader.h>
#include "StoneWidget.h"
#include "version.h"
#include "KSameWidget.moc"
#include <qpopmenu.h> 
#include <kstatusbar.h> 
#include <kintegerline.h> 


static const int two_item = 2;
static const int three_item = 3;
static const int four_item = 4;

KSameWidget::KSameWidget() {

  KMenuBar *menu=new KMenuBar(this);

  QPopupMenu *gamemenu=new QPopupMenu;
  gamemenu->setAccel(CTRL+Key_N,
		     gamemenu->insertItem(klocale->translate("New"),this, SLOT(m_new())));
  gamemenu->setAccel(CTRL+Key_L,
		     gamemenu->insertItem(klocale->translate("Load"),this, SLOT(m_load())));
  gamemenu->setAccel(CTRL+Key_S,
		     gamemenu->insertItem(klocale->translate("Save"),this, SLOT(m_save())));
  gamemenu->insertSeparator();
  gamemenu->setAccel(CTRL+Key_H,
		     gamemenu->insertItem(klocale->translate("Show Highscore"),this, SLOT(m_showhs())));
  gamemenu->insertSeparator();
  gamemenu->setAccel(CTRL+Key_Q,
		     gamemenu->insertItem(klocale->translate("Quit"),this, SLOT(m_quit())));


  menu->insertItem(klocale->translate("Game"),gamemenu);
  menu->insertSeparator();
  optionsmenu = new QPopupMenu;
  optionsmenu->setCheckable(true);

  colorsmenu = new QPopupMenu;
  colorsmenu->setCheckable(true);
  colorsmenu->insertItem(klocale->translate("2"), two_item );
  colorsmenu->insertItem(klocale->translate("3"), three_item );
  colorsmenu->insertItem(klocale->translate("4"), four_item ); 
  connect (colorsmenu, SIGNAL (activated (int)), SLOT (m_colors(int)));
  //  optionsmenu->insertItem(klocale->translate("Colors"),colorsmenu);

  random_item = optionsmenu->insertItem(klocale->translate("Random Board"),this, SLOT(m_tglboard()));
  optionsmenu->setAccel(CTRL+Key_R,random_item);
  menu->insertItem(klocale->translate("Options"),optionsmenu);
  menu->insertSeparator();


  QPopupMenu *help = kapp->getHelpMenu(true, QString(klocale->translate("SameGame")) +
				       " " KSAME_VERSION +
				       klocale->translate("\n\nby Marcus Kreutzberger"
				       " (kreutzbe@informatik.mu-luebeck.de)"));
  menu->insertItem(klocale->translate("Help"),help);

  setMenu(menu);
  
  status=new KStatusBar(this);
  status->insertItem(klocale->translate("Colors: XX"),1);
  status->insertItem(klocale->translate("Board: XXXXXX"),2);
  status->insertItem(klocale->translate("Marked: XXXXXX"),3);
  status->insertItem(klocale->translate("Score: XXXXXX"),4);
  status->setAlignment(4,AlignRight);
  setStatusBar(status);
  
  stone = new StoneWidget(this,15,10);
  connect( stone, SIGNAL(s_newgame()), this, SLOT(history_clear()));
  connect( stone, SIGNAL(s_gameover()), this, SLOT(gameover()));
  connect( stone, SIGNAL(s_mark(int)), this, SLOT(set_marked(int)));   
  connect( stone, SIGNAL(s_score(int)), this, SLOT(set_score(int)));   
  connect( stone, SIGNAL(s_remove(int,int)), this, SLOT(history_add(int,int)));   
  connect(stone, SIGNAL(s_sizechanged()), this, SLOT(sizeChanged()));  

  setView(stone,false);
  stone->show();
  highscore = new HighScore();
  
  israndom=1;
  set_board(time(0));
  optionsmenu->setItemChecked(random_item, israndom); 
  set_score(0);
  set_marked(0);
  sizeChanged();   
  history.setAutoDelete(true); 
  if (!kapp->isRestored()) {
       stone->newgame(board);
  }
}
void 
KSameWidget::sizeChanged() {
     stone->setFixedSize(stone->sizeHint());
     updateRects();
}     

KSameWidget::~KSameWidget() {
     //  debug("~KSameWidget\n"); 
}
#define MAX(a,b) ((a)>(b)?(a):(b))
void KSameWidget::m_new() {
     if (israndom) {
	  if (stone->gameover()||
	      (KMsgBox::yesNo(this, klocale->translate("ksame - New"), 
			      klocale->translate("Do you want to resign?"), KMsgBox::STOP)==1)) {
	       srand(time(0));
	       set_board(rand());
	       stone->newgame(board);
	  }
     } else {
	  QDialog dlg(0,klocale->translate("Board"),1);
	  int w=10,h=10;
	  
	  KIntegerLine bno(&dlg,klocale->translate("Hallo"));
	  bno.setFocus();
	  QLabel l(&bno,klocale->translate("Please &enter the board number:"),&dlg);
	  
	  l.move(10,h);
	  l.resize(l.sizeHint());
	  w=l.width()+20;
	  h+=l.height()+10;
	  bno.move(10,h);
	  w=MAX(bno.width()+20,w);
	  h+=bno.height()+10;
     
	  QPushButton ok(klocale->translate("Ok"), &dlg);
	  ok.setDefault(true);
	  QPushButton cancel(klocale->translate("Cancel"), &dlg);
	  
	  w=MAX(ok.width()+cancel.width()+40,w);
	  
	  ok.move(w-ok.width()-20-cancel.width(),h);
	  cancel.move(w-cancel.width()-10,h);
	  h+=MAX(ok.height(),cancel.height())+10;
	  
	  dlg.resize(w,h);
	  dlg.setFixedSize(w,h);
	  dlg.connect(&ok, SIGNAL(clicked()), SLOT(accept()) );
	  dlg.connect(&cancel, SIGNAL(clicked()), SLOT(reject()) );
	  dlg.connect(&bno, SIGNAL(returnPressed()), SLOT(accept()) );
	  bno.setValue(board);
	  bno.selectAll();
	  if (dlg.exec()) {
	       set_board(bno.value());
	       stone->newgame(board);
	  }
     }
	  
}
void KSameWidget::m_load() {
     debug(klocale->translate("menu load not supported")); 
}
void KSameWidget::m_save() {
     debug(klocale->translate("menu save not supported"));
}
void KSameWidget::m_showhs() {
     if (highscore->isVisible())
	  highscore->hide();
     else highscore->show();
}
void KSameWidget::m_quit() {
     if (KMsgBox::yesNo(this, klocale->translate("ksame - Quit"), 
			klocale->translate("Do you want really quit?"), KMsgBox::STOP)==1) {
	  kapp->quit();
     }
}

void KSameWidget::m_colors( int id) {
     debug(klocale->translate("menu colors not supported"));
     int colors=id;
     char p[30];
     colors=stone->colors();
     sprintf(p,klocale->translate("Colors: %01i"),colors);
     status->changeItem(p,1);
}

void KSameWidget::m_tglboard() {

     israndom=!israndom;
     optionsmenu->setItemChecked(random_item, israndom);
}
void KSameWidget::set_board(int no) {
     char p[30];
     if (no<0) no*=-1;
     board=no%1000000;
     sprintf(p,klocale->translate("Board: %06i"),board);
     status->changeItem(p,2);        
}
void KSameWidget::gameover() {
     debug(klocale->translate("GameOver"));
     highscore->add(stone->board(),stone->score(),stone->colors());
}
void KSameWidget::set_marked(int m) {
    char p[30];
    sprintf(p,klocale->translate("Marked: %06i"),m);
    status->changeItem(p,3);   
}
void KSameWidget::set_score(int score) {
    char p[30];
    sprintf(p,klocale->translate("Score: %06i"),score);
    status->changeItem(p,4);   
}
void KSameWidget::history_add(int x,int y) {
     history.append(new QPoint(x,y));
}
void KSameWidget::history_clear() {
     history.clear();
}

void KSameWidget::readProperties(KConfig *conf) {
     history.clear();
     
     if (!conf->hasKey(klocale->translate("Board"))||!conf->hasKey(klocale->translate("Stones"))) {
	  set_board(time(0));
	  return;
     }
	 
     board=conf->readNumEntry("Board");
     QString s=conf->readEntry("Stones");

     set_board(board);
     stone->newgame(board);
     char *p=s.data();
     while (*p) {
	  int x=-1,y=-1;
	  sscanf(p,"%02X%02X",&x,&y);
	  history.append(new QPoint(x,y));
	  stone->remove(x,y);
	  *p?*++p?*++p?*++p?p++:0:0:0:0;
     }
}

void KSameWidget::saveProperties(KConfig *conf) {
     char *tmpbuf=new char[history.count()*4+1];
     char *p=tmpbuf;
     for (QPoint *item=history.first();item;item=history.next()) {
	  sprintf(p,"%02X%02X",item->x(),item->y());
	  p+=4;
     }
     *p=0;
     conf->writeEntry("Stones",tmpbuf);
     conf->writeEntry("Board",board);
     conf->sync();
     delete[] tmpbuf;
}










