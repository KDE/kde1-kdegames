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

#ifndef _STONEWIDGET
#define _STONEWIDGET

#include <qlined.h> 
#include <qpixmap.h>
#include <qbitmap.h> 
#include <qwidget.h>
#include <qstring.h>
//#include "StoneWidget.moc"

#define F_CHANGED 128
#define F_MARKED  64
#define F_COLORMASK 63

#define SETCHANGED(field) field|=F_CHANGED
#define SETMARKED(field) field|=(F_MARKED|F_CHANGED)
#define CLEARCHANGED(field) field&=~F_CHANGED
#define CLEARMARKED(field) field=((field)&(~F_MARKED))|F_CHANGED
#define SETCOLOR(field,color) (field)=((color)&F_COLORMASK)|F_CHANGED
#define ISCHANGED(field) ((field)&F_CHANGED)
#define ISMARKED(field) ((field)&F_MARKED)
#define GETCOLOR(field) ((field)&F_COLORMASK)



class StoneWidget : public QWidget {
    Q_OBJECT
public:
    StoneWidget( QWidget *parent=0, int x=10,int y=10);
    ~StoneWidget();

    // Zustand abfragen
    int getScore();

    int getBoard();

    int getSizex();

    int getSizey();
    int getColors() { return colors; }
    void setColors(int c) { if ((colors<=maxcolor)&&(colors>1)) colors = c; }

    // Spielende abfragen
    int isGameover();

    // Das Spielfeld wird neu initialisiert.
    void newGame(int colors, int boardno, int size_x, int size_y);

    void setMultiSpin(int state);
    int getMultiSpin();

    // Das Spielfeld wird neu initialisiert. 
    // mit den alten/standard Parametern.
    void newGame();
protected:
    void paintEvent( QPaintEvent *e );
    void mousePressEvent ( QMouseEvent *e);
    void timerEvent( QTimerEvent *e );
    void mouseMoveEvent ( QMouseEvent *e);
private:

    
    void collapseArea();
    void recursive_markfield(int i,unsigned char color);
    int markfield(int x, int y,int always=0);
    void drawfield( QPaintEvent *e=0,int erase=0 );
    void drawfield_multispin( QPaintEvent *e=0,int erase=0 );
    void drawfield_singlespin( QPaintEvent *e=0,int erase=0 );
    void checkGameOver();

    void setStoneSize(int size_x, int size_y);
    
    int lastfield,lastcolor;
    int stones;

    int colors,boardno;

    long score;
    int dx,dy;   // Pixelgröße eines Steines.
    int sx,sy;   // Feldgröße in Steinen.
    int slice;   // Bildnummer des Steinmovies
    int px,py;   // Pixelgröße des SteinFeldes.
    int maxspin,maxcolor,doublecolor,multispin;
    QPixmap stonemap;
    QPixmap *backmap;
    QPixmap *tempmap;
    QBitmap maskmap;

    int lastpoint;
    unsigned char *field;
    unsigned char *spin;
    
    int gameover;
    int highscore;
  
    QLineEdit *edit;
    
public slots: 

// Das Spielfeld wird neu initialisiert.
//void newGame();

signals: 

    // Der Punktestand hat sich geändert.
    void s_updateScore(int score);
    void s_updateBoard(int boardno);
    void s_updateColors(int colors);

    // Das Spiel ist zueende. Die Endpunkte werden mit übergeben.
    void s_gameover(int score);

    // Der Stein (x,y) wurde angeklickt(entfernt), alle umliegend betroffenen Steine
    // verschwinden automatisch, d.h. kein extra Signal.
    void s_removeStone(int x, int y);
    
};

#endif





