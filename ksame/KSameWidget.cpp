/*
 *   ksame 0.2 - simple Game
 *   Copyright (C) 1997  Marcus Kreutzberger
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
#include <kapp.h>
#include <qpainter.h> 
#include <qwidget.h>
#include <qpushbt.h>
#include <kbutton.h>
#include <qpixmap.h>
#include <qlcdnum.h>
#include <kmsgbox.h> 
#include <kiconloader.h>
#include "StoneWidget.h"
//#include "KSameWidget.h"
#include "KSameWidget.moc"
#include <qpopmenu.h> 
#include <kstatusbar.h> 

KSameWidget::KSameWidget() {

  KMenuBar *menu=new KMenuBar(this);

  QPopupMenu *filemenu=new QPopupMenu;
  filemenu->insertItem("New Game",this, SLOT(m_new()));
  filemenu->insertItem("Load Game",this, SLOT(m_load()));
  filemenu->insertItem("Save Game",this, SLOT(m_save()));
  filemenu->insertSeparator();
  filemenu->insertItem("Quit",this, SLOT(m_quit()));
  menu->insertItem("File",filemenu);
  menu->insertSeparator();
  QPopupMenu *optionsmenu=new QPopupMenu;
  
  QPopupMenu *colorsmenu=new QPopupMenu;
  colorsmenu->insertItem("3",this, SLOT(m_colors3()));
  colorsmenu->insertItem("4",this, SLOT(m_colors4())); 
  colorsmenu->insertItem("5",this, SLOT(m_colors5())); 
  colorsmenu->insertItem("6",this, SLOT(m_colors6())); 
  optionsmenu->insertItem("Colors",colorsmenu);

  optionsmenu->insertItem("Random Board",this, SLOT(m_tglboard()));
  optionsmenu->insertItem("Multispin",this, SLOT(m_tglmultispin()));
  menu->insertItem("Options",optionsmenu);
  menu->insertSeparator();

  QPopupMenu *helpmenu=new QPopupMenu;
  helpmenu->insertItem("Rules",this, SLOT(m_help()));
  helpmenu->insertSeparator();
  helpmenu->insertItem("About KSame",this, SLOT(m_about())); 
  menu->insertItem("Help",helpmenu);

  setMenu(menu);
  
  status=new KStatusBar(this);
  status->insertItem("Colors: 3",1);
  status->insertItem("Board: 99999999",2);
  status->insertItem("Score: 99999999",3);
  status->setAlignment(3,AlignRight);
  
  setStatusBar(status);
    
  stone = new StoneWidget(this,15,10);
  connect( stone, SIGNAL(s_gameover(int)), this, SLOT(gameover(int)));
  connect( stone, SIGNAL(s_updateColors(int)), this, SLOT(updatecolors(int))); 
  connect( stone, SIGNAL(s_updateBoard(int)), this, SLOT(updateboard(int)));  
  connect( stone, SIGNAL(s_updateScore(int)), this, SLOT(updatescore(int)));   
  setView(stone);

  stone->newGame();
  /* 
 
  score  = new QLCDNumber( 8, this, "score" );
  score->setSegmentStyle(QLCDNumber::Outline);
  score->setFrameStyle(QFrame::NoFrame);
  
  connect( stone, SIGNAL(updateScoreSignal(double)), score, SLOT(display(double)));
  score->move((w-score->width())/2,h);
  score->resize(score->width(),quitbt->height());
 
  //  hiscore = new ScoreWidget(this);
  //  hiscore->move((width()-hiscore->width())/2,5);
  //hiscore->show();
  

 
  h+=quitbt->height();
  
  setFixedSize(w,h);
*/

}
KSameWidget::~KSameWidget() {
  debug("~KSameWidget\n"); 
}




void KSameWidget::m_new() {
  if (stone->isGameover()||
      (KMsgBox::yesNo(this, "ksame - New", 
		      "Do you want to resign?", KMsgBox::STOP)==1))
   stone->newGame();
}
void KSameWidget::m_load() {
  debug("menu load not supported");
}
void KSameWidget::m_save() {
  debug("menu save not supported");
}
void KSameWidget::m_quit() {
  if (KMsgBox::yesNo(this, "ksame - Quit", 
		     "Do you want really quit?", KMsgBox::STOP)==1)
    kapp->quit();
}
void KSameWidget::m_colors3() {
  debug("menu colors not supported");
}
void KSameWidget::m_colors4() {
  debug("menu colors not supported");
}
void KSameWidget::m_colors5() {
  debug("menu colors not supported");
}
void KSameWidget::m_colors6() {
  debug("menu colors not supported");
}
void KSameWidget::m_tglboard() {
  debug("menu board not supported");
}
void KSameWidget::m_tglmultispin() {
    stone->setMultiSpin(!stone->getMultiSpin());
}
void KSameWidget::m_help() {
  debug("menu help not supported");
}
void KSameWidget::m_about() {
    //  stone->showHighscore();
  kapp->invokeHTMLHelp("ksame/index.html","");
  
  KMsgBox::message(stone, "ksame - About", 
		   "ksame\nby\nkreutzbe@informatik.mu-luebeck.de", 
		   KMsgBox::INFORMATION);
  //  stone->hideHighscore();
}

void KSameWidget::gameover(int score) {
    
    debug("GameOver");
}

void KSameWidget::updatecolors(int colors) {
    char p[30];

    sprintf(p,"Colors: %01i",colors);
    status->changeItem(p,1);   
}
void KSameWidget::updateboard(int board) {
    char p[30];

    sprintf(p,"Board: %05i",board);
    status->changeItem(p,2);   
}
void KSameWidget::updatescore(int score) {
    char p[30];

    sprintf(p,"Score: %05i",score);
    status->changeItem(p,3);   
}















