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

KSameWidget::KSameWidget() {

  int w=300,h=100;
  line=0;
  
  stone = new StoneWidget(this,15,10);
  connect( stone, SIGNAL(gameOverSignal(long)), this, SLOT(gameover(long))); 
  w=stone->width();
  h=stone->height();

  quitbt = new KButton(this,"Quit");
  quitbt->setPixmap(kapp->getIconLoader()->loadIcon("exit.xpm"));  
  connect( quitbt, SIGNAL(clicked()), this, SLOT(quitbt_clicked()));
  quitbt->move(0,h);
 
  score  = new QLCDNumber( 8, this, "score" );
  score->setSegmentStyle(QLCDNumber::Outline);
  score->setFrameStyle(QFrame::NoFrame);
  
  connect( stone, SIGNAL(updateScoreSignal(double)), score, SLOT(display(double)));
  score->move((w-score->width())/2,h);
  score->resize(score->width(),quitbt->height());
 
  //  hiscore = new ScoreWidget(this);
  //  hiscore->move((width()-hiscore->width())/2,5);
  //hiscore->show();
  
  KButton *aboutbt  = new KButton(this,"About");
  aboutbt->setAutoResize(TRUE);
  aboutbt->setPixmap(kapp->getIconLoader()->loadIcon("help.xpm"));
  //CHECK_PTR(tempmap); 
  connect( aboutbt, SIGNAL(clicked()), this, SLOT(aboutbt_clicked()));
  aboutbt->move(w-aboutbt->width(),h);

  newbt  = new KButton(this,"New Game");
  newbt->setPixmap(kapp->getIconLoader()->loadIcon("reload.xpm"));
  
  connect( newbt, SIGNAL(clicked()), this, SLOT(newbt_clicked()));
  newbt->move(quitbt->width(),h);
 
 
  h+=quitbt->height();
  
  setFixedSize(w,h);

}
KSameWidget::~KSameWidget() {
  debug("~KSameWidget\n"); 
}


void KSameWidget::quitbt_clicked() {
  if (KMsgBox::yesNo(this, "ksame - Quit", 
		     "Do you want really quit?", KMsgBox::STOP)==1)
    kapp->quit();
}

void KSameWidget::newbt_clicked() {
  if (stone->getgameOver()||
      (KMsgBox::yesNo(this, "ksame - New", 
		      "Do you want to resign?", KMsgBox::STOP)==1))
   stone->newGame();
}

void KSameWidget::aboutbt_clicked() {
  stone->showHighscore();
  kapp->invokeHTMLHelp("ksame/ksame.html","");
  
  KMsgBox::message(stone, "ksame - About", 
		   "ksame\nby\nkreutzbe@informatik.mu-luebeck.de", 
		   KMsgBox::INFORMATION);
  stone->hideHighscore();
}

void KSameWidget::gameover(long) {

  if (0) {
    line=new QLineEdit(this);
    line->setFont( QFont( "Times", 20, QFont::Bold ) );
    line->move(width()/2,(height()-line->height())/2);
    line->show();
    connect( line, SIGNAL(returnPressed()), this, SLOT(returnpressed()));
  }
 
}

void KSameWidget::returnpressed() {
  
  if (line->text()[0]) { 
    delete line;
    printf("OK\n");
  }
  
  
}

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













