/*
 *   ksame 0.3 - simple Game
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
#include <qpainter.h> 
#include <qpixmap.h> 
#include <qbitmap.h> 
#include <qlabel.h> 
#include <qstring.h>
#include "StoneWidget.moc"
#include <kapp.h>
#include <kiconloader.h>

StoneWidget::StoneWidget( QWidget *parent, int x, int y ) 
    : QWidget(parent) {
  
	backmap = 0;
	tempmap=0;
	
	edit=0;
	
	dx=40;
	dy=40; 
	maxspin=30;
	
	field = 0;
	spin=0;
	setStoneSize(x,y);
	
	colors=3;
	maxcolor=4;
	doublecolor=1;
	boardno=0;
	
	if ( stonemap.isNull() )
	    stonemap = kapp->getIconLoader()->loadIcon("stones2000.gif");
	
	if (  maskmap.isNull() ) {  // NO MASK !!!
	    maskmap = kapp->getIconLoader()->loadIcon("stones2000_mask.gif");
	    stonemap.setMask(maskmap);
	}
	if (!backmap) {
	    backmap = new QPixmap(sx*dx,sy*dy);
	    CHECK_PTR(backmap);
	    backmap->fill(QColor(70,70,70));
	}
	if (!tempmap) {
	    tempmap = new QPixmap(dx,dy);
	    CHECK_PTR(tempmap);
	}
	
	
	slice=0;
	setMouseTracking(TRUE);
	
	QColor c(115,115,115);
	setBackgroundColor(c);
	newGame();
	
	startTimer( 100 ); 
}

 StoneWidget::~StoneWidget() {  
   setMouseTracking(FALSE);
   killTimers();
   delete field;
   delete tempmap;
   //   debug("~StoneWidget\n");
}

void StoneWidget::setStoneSize(int size_x, int size_y) {
 
    sx=size_x;
    sy=size_y;
    setGeometry(0,0,dx*sx,dy*sy);
    setMinimumSize( dx*sx,dy*sy);
    setMaximumSize( dx*sx,dy*sy);  
    if (field) delete field;
    field = new unsigned char[sx*sy];
    if (spin) delete spin;
    spin = new unsigned char[sx*sy];
    lastpoint=-1;
    slice=0;
}
void StoneWidget::newGame(int colors, int boardno, int size_x, int size_y) {
    this->colors=colors;
    this->boardno=boardno;    
    setStoneSize(size_x,size_y);
    newGame();
}

void StoneWidget::newGame() {
    score=0;
    gameover=0;
    highscore=0;
    for (int i=0;i<sx*sy;i++) { 
	SETCOLOR(field[i], (unsigned char)(rand()%colors)+1);
	spin[i]=0;
    }
    lastfield=-2;
    emit s_updateScore(score);    
    emit s_updateColors(colors);
    emit s_updateBoard(boardno);
    
}

int StoneWidget::getScore() {
  return score;
}
int StoneWidget::getBoard() {
  return boardno;
}
int StoneWidget::getSizex() {
  return sx;
}
int StoneWidget::getSizey() {
  return sy;
}
int StoneWidget::isGameover() {
  return gameover;
}
int StoneWidget::getMultiSpin(){
    return multispin;
}

void StoneWidget::setMultiSpin(int state){
    multispin=state;
}

void StoneWidget::drawfield(QPaintEvent *e,int erase) {
    if (multispin) {
	drawfield_multispin(e,erase);
    } else {
	drawfield_singlespin(e,erase);
    }
}

void StoneWidget::drawfield_multispin(QPaintEvent *e,int erase) {
    int slicex,slicey;		
    int p,xdx,ydy,marked,redraw,color;

    for (int x=0;x<sx;x++) {
	for (int y=0;y<sy;y++) {	    
	    p=x+y*sx;
	    xdx=x*dx;
	    ydy=y*dy;
	    
	    marked=ISMARKED(field[p]);
	    redraw=erase;

	    if (!marked&&e) {
		QRect r(xdx,ydy,dx,dy);
		redraw=erase||r.intersects(e->rect());
	    }	    

	    if (marked||ISCHANGED(field[p])||redraw) {
		CLEARCHANGED(field[p]);
		color=GETCOLOR(field[p]);
		slicey=(color-1)*dy;
		if (marked) {
		    slicex=dx*(spin[p]++);
		    if (spin[p]==maxspin) spin[p]=0;
		    //		    if (doublecolor) slicey+=maxcolor*dy;
		} else {
		    slicex=spin[p]*dx;
		}
		if (!color) {
		    bitBlt(this,xdx,ydy,backmap,xdx,ydy,dx,dy,CopyROP,TRUE);
		} else {
		    bitBlt(tempmap,0,0,backmap,xdx,ydy,dx,dy,CopyROP,TRUE);
		    bitBlt(tempmap,0,0,&stonemap,slicex,slicey,dx,dy,CopyROP,FALSE);
		    bitBlt(this,xdx,ydy,tempmap,0,0,dx,dy,CopyROP,TRUE);
		}
	    }
	    
	}
    }
    return;
}
void StoneWidget::drawfield_singlespin(QPaintEvent *e,int erase) {
    int slicex,slicey;		
    int p,xdx,ydy,marked,redraw,color;

    for (int x=0;x<sx;x++) {
	for (int y=0;y<sy;y++) {	    
	    p=x+y*sx;
	    xdx=x*dx;
	    ydy=y*dy;
	    
	    marked=ISMARKED(field[p]);
	    redraw=erase;

	    if (!marked&&e) {
		QRect r(xdx,ydy,dx,dy);
		redraw=erase||r.intersects(e->rect());
	    }	    

	    if (marked||ISCHANGED(field[p])||redraw) {
		CLEARCHANGED(field[p]);
		color=GETCOLOR(field[p]);
		slicex=(marked)?dx*slice:0;	       
		slicey=(color-1)*dy;
		if (!color) {
		    bitBlt(this,xdx,ydy,backmap,xdx,ydy,dx,dy,CopyROP,TRUE);
		} else {
		    bitBlt(tempmap,0,0,backmap,xdx,ydy,dx,dy,CopyROP,TRUE);
		    bitBlt(tempmap,0,0,&stonemap,slicex,slicey,dx,dy,CopyROP,FALSE);
		    bitBlt(this,xdx,ydy,tempmap,0,0,dx,dy,CopyROP,TRUE);
		}
	    }
	    
	}
    }
    return;
}




int StoneWidget::markfield(int x,int y,int always) {
    // gibt true zurück, wenn sich ein Stein ändert.

    int changed=0;
    if ((x<0)||(y<0)||(x>sx)||(y>sy)) { 
	if (stones) {
	    changed=1;
	    for (int i=0;i<sx*sy;i++) { 
		CLEARMARKED(field[i]);
		SETCHANGED(field[i]);
	    }
	    stones=0;
	}
	lastpoint=-1;
	return (changed);
    }
    
    int p=x+y*sx;  
    if (!ISMARKED(field[p])||always) {
	if (stones||always) {
	    changed=1;
	    for (int i=0;i<sx*sy;i++) {
		if (ISMARKED(field[i])) {
		    CLEARMARKED(field[i]);
		}
	    }
	    stones=0;
	}
	
	if (lastpoint!=p||always) {
	    changed=1;
	    recursive_markfield(p,GETCOLOR(field[p]));
	    lastpoint=p;
	    if (stones==1) {
		CLEARMARKED(field[p]);
		stones=0;
	    }
	}
    }
    return(changed);
}

void StoneWidget::recursive_markfield(int i,unsigned char color) {
  if ((i<0)||(i>sx*sy)) return;
  if (ISMARKED(field[i])) return;

  int mycolor=GETCOLOR(field[i]);
  if (!mycolor||mycolor!=color) return;

  SETMARKED(field[i]);
  SETCHANGED(field[i]);
  stones++;
  if ((i%sx)!=0) recursive_markfield(i-1,color);  // nicht ganz links
  if (i>=sx) recursive_markfield(i-sx,color);  // nicht ganz oben   
  if (i<(sx-1)*sy) recursive_markfield(i+sx,color);  // nicht ganz unten
  if (((i+1)%sx)!=0) recursive_markfield(i+1,color); // nicht ganz rechts
  return;
}

void StoneWidget::checkGameOver() {
  int i;
  unsigned char color;
  
  gameover=1;
  i=sx*sy-1;
  while ((i>=0)&&gameover) {
    while ((GETCOLOR(field[i])==0)&&(i>=0)&&gameover) i--;
    while (((color=GETCOLOR(field[i]))!=0)&&(i>=0)&&gameover) {
      if (((i%sx)!=0)&&(GETCOLOR(field[i-1])==color))  // nicht ganz links
	gameover=0;  
      if ((i>=sx)&&(GETCOLOR(field[i-sx])==color))   // nicht ganz oben   
	gameover=0;  
      if ((((i+1)%sx)!=0)&&(GETCOLOR(field[i+1])==color))// nicht ganz rechts 
	gameover=0; 
      i--;
    }
  }
  if (gameover) {
    emit s_gameover(score);
  }
}



// Steine zusammenrücken erst nach unten, dann nach links aufrücken 
void StoneWidget::collapseArea() {
    int j,i1,i2;
    
    for (int x=0;x<sx;x++) {
	i1=x+(sy-1)*sx;
	while ((GETCOLOR(field[i1])!=0)&&(i1>=0)) i1-=sx;
	i2=i1;
	while (i2>=0) {
	    while ((GETCOLOR(field[i2])==0)&&(i2>=0)) i2-=sx;
	    while ((GETCOLOR(field[i2])!=0)&&(i2>=0)) {
		field[i1]=field[i2]|F_CHANGED;
		field[i2]=0|F_CHANGED;	  
		i1-=sx;
		i2-=sx;
	    }
	}
    }  
    for (int x=0;x<sx;x++) {
	i1=x+(sy-1)*sx;
	while ((GETCOLOR(field[i1])!=0)&&(i1<sx*sy)) i1++;
	i2=i1;
	while (i2<sx*sy) {
	    while ((GETCOLOR(field[i2])==0)&&(i2<sx*sy)) i2++;
	    while ((GETCOLOR(field[i2])!=0)&&(i2<sx*sy)) {
		for (j=0;j<sy*sx;j+=sx) {
		    field[i1-j]=field[i2-j]|F_CHANGED;
		    field[i2-j]=0|F_CHANGED;
		}
		i1++;
		i2++;
	    }
	}
    }  
}


void StoneWidget::paintEvent( QPaintEvent *e ) {
   drawfield(e);
}

void StoneWidget::timerEvent( QTimerEvent * ) {
    QPoint p=mapFromGlobal(cursor().pos());
    int x=p.x();
    int y=p.y();

    if (x<0||y<0||x>dx*sx||y>dy*sy) return;
  
    slice=(slice+1)%maxspin;    
    markfield(x/dx,y/dy);
    drawfield();
    return;
}

void StoneWidget::mouseMoveEvent ( QMouseEvent *e) { 
    if (!gameover&&markfield((e->pos().x())/dx,(e->pos().y())/dy)) {
	if (!multispin) slice=maxspin-1;
	drawfield();
    }
    return;
}

void StoneWidget::mousePressEvent ( QMouseEvent *e) {
    int x=(e->pos().x())/dx;
    int y=(e->pos().y())/dy;

    if (stones>1&&!gameover) {
	emit s_removeStone(x,y);
	score+=(stones-2)*(stones-2);
	stones=0;
	for (int i=0;i<sx*sy;i++) {
	    if (ISMARKED(field[i])) field[i]=0|F_CHANGED;
	}
    
	collapseArea();
	
	if (!GETCOLOR(field[(sy-1)*sx])) {
	    debug("Bonus\n");
	    score+=1000;      
	}
	if (markfield(x,y,1)) drawfield();
	emit s_updateScore(score);
	checkGameOver();
    }
}








