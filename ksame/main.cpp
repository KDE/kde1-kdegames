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
  
  
  QWidget score;



  QTabDialog *s=new QTabDialog(&score);

  connect(s,SIGNAL(defaultButtonPressed()),score,SLOT(close()));

  s->addTab(new ScoreWidget(s),"&Two Colors");
  s->addTab(new ScoreWidget(s),"Th&ree Colors");
  s->addTab(new ScoreWidget(s),"&Four Colors");

  s->move(0,0);
  s->show();
  score.resize(s->width(),s->height());
  score.show();

  
  return a.exec();
}

