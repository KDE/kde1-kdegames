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

#include <kapp.h>
#include <qwidget.h>
#include <qtabbar.h>
#include <qsize.h>
#include "KSameWidget.h"
#include "ScoreWidget.h"
#include "HighScore.h"
#undef TrueColor


void myMessageOutput( QtMsgType type, const char *msg ) {
  switch ( type ) {
  case QtDebugMsg:
    fprintf( stderr, "Debug: %s\n", msg );
    break;
  case QtWarningMsg:
    fprintf( stderr, "Warning: %s\n", msg );
    break;
  case QtFatalMsg:
    fprintf( stderr, "Fatal: %s\n", msg );
    abort();                        // dump core on purpose
  }
}

int main( int argc, char **argv )
{
  qInstallMsgHandler( myMessageOutput );
  KApplication::setColorSpec(QApplication::TrueColor+QApplication::CustomColor);
  KApplication a( argc, argv , "ksame");

  KSameWidget w;
  w.setGeometry( 100, 100, 400, 400 );
  a.setMainWidget( &w );
  w.show();
  
  
  QWidget highscore;
  QSize size(5,5);
  
  QLabel *label=new QLabel("---   Highscore   ---",&highscore);
  QFont f = label->font();
  f.setPointSize(20); 
  label->setFont(f);
  //  label->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
  label->setAlignment( AlignCenter );
  label->resize(10,label->sizeHint().height()+20);
  label->move(0,size.height());
  //  size.rwidth()+=label->width();
  size.rheight()+=label->height();
  label->show();
  
  QTabBar *tabbar=new QTabBar(&highscore);
  // child widgets must be created on heap, since they
  // are deleted by the parent widget
  QTab *c1 = new QTab;
  QTab *c2 = new QTab;
  QTab *c3 = new QTab;
  c1->label="&two colors";c1->enabled=true;
  c2->label="th&ree colors";c2->enabled=true;
  c3->label="&four colors";c3->enabled=true;
  
  tabbar->addTab(c1);
  tabbar->addTab(c2);
  tabbar->addTab(c3);
  tabbar->move(0,size.height());
  tabbar->resize(tabbar->sizeHint());
  //  size.rwidth()=tabbar->width();
  size.rheight()+=tabbar->height();
  tabbar->show();

  ScoreWidget *score=new ScoreWidget(&highscore);
  score->move(0,size.height());
  size.rwidth()=score->width();
  size.rheight()+=score->height();
  score->show();

  label->resize(size.width(),label->height());
  tabbar->resize(size.width(),tabbar->height());
  QObject::connect(tabbar,SIGNAL(selected(int)),score,SLOT(select(int)));
  
  highscore.resize(size);

  highscore.show();
  
  return a.exec();
}

