#ifndef _STONEWIDGET
#define _STONEWIDGET

#include <qlined.h> 
#include <qpixmap.h>
#include <qbitmap.h> 
#include <qwidget.h>
#include <qstring.h>
//#include "StoneWidget.moc"
class StoneWidget : public QWidget {
 Q_OBJECT
public:
  StoneWidget( QWidget *parent=0, int x=10,int y=10);
 ~StoneWidget();
  long getScore();
  int getgameOver();
protected:
  void paintEvent( QPaintEvent *e );
  void mousePressEvent ( QMouseEvent *e);
  void timerEvent( QTimerEvent *e );
  void mouseMoveEvent ( QMouseEvent *e);
private:

#define MAXENTRY 10
  struct {
    QString name;
    long score;
  } Table[MAXENTRY];
  
  int Position;

  void recursive_markfield(int i,char color);
  void markfield(int x, int y);
  void drawField( QPaintEvent *e=0,int erase=0 );
  void checkGameOver();
  int lastfield,lastcolor;
  int stones;
  long score;
  int dx,dy,sx,sy,pcount;
  QPixmap stonemap;
  QPixmap *backmap;
  QPixmap *tempmap;
  QBitmap maskmap;
  char *field;
  char *change;
  
  int gameover;
  int highscore;
  
  QLineEdit *edit;
  

public slots:
   void newGame();
   void returnpressed();
   void showHighscore();
   void hideHighscore();
signals:
  void updateScoreSignal(long score);
  void updateScoreSignal(double score);
  void gameOverSignal(long score);
    
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


