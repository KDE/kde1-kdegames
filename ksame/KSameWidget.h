#ifndef _KSAMEWIDGET
#define _KSAMEWIDGET
#include <qlined.h> 
#include <qwidget.h>
#include <kbutton.h>
#include <qpushbt.h>
#include <qlcdnum.h>
#include "StoneWidget.h"
#include "ScoreWidget.h"


class KSameWidget: public QWidget {
 Q_OBJECT
public:
  KSameWidget(); 
 ~KSameWidget();
 
private:
  StoneWidget *stone;
  ScoreWidget *hiscore;
  QLCDNumber *score;
  KButton *quitbt;
  KButton *newbt;

  QLineEdit *line;

 
public slots:

  void quitbt_clicked();
  void newbt_clicked();
  void aboutbt_clicked();
  
  void gameover(long);

  void returnpressed();
  

};


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
#endif
