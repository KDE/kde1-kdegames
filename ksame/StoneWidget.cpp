#include <stdio.h>
#include <stdlib.h>
#include <qpainter.h> 
#include <qpixmap.h> 
#include <qbitmap.h> 
#include <qlabel.h> 
#include <qstring.h>
#include "StoneWidget.moc"
#include "stone3.inc"
#include "stonemask3.xbm"
#include <kapp.h>

 StoneWidget::StoneWidget( QWidget *parent, int x, int y ) 
  : QWidget(parent) {
  
  stonemap= 0;
  maskmap = 0;
  backmap = 0;
  tempmap=0;
  
  edit=0;
  
  dx=40;
  dy=40; 
  sx=x;
  sy=y;
  setGeometry(0,0,dx*sx,dy*sy);
  setMinimumSize( dx*sx,dy*sy);
  setMaximumSize( dx*sx,dy*sy);
  pcount=0;
  field = new char[sx*sy];
  change = new char[sx*sy];
  setMouseTracking(TRUE);
  QColor c(115,115,115);
  setBackgroundColor(c);
  newGame();

  KConfig *conf=kapp->getConfig();
  conf->setGroup("ksame");
  for (int i=0;i<MAXENTRY;i++) {
    QString s;
    Table[i].score=conf->readNumEntry(s.sprintf("Score%i",i),0);
    Table[i].name=conf->readEntry(s.sprintf("Name%i",i),"FOR YOU");
  }  
  startTimer( 100 ); 
}

 StoneWidget::~StoneWidget() {  
   setMouseTracking(FALSE);
   killTimers();
   delete field;
   delete stonemap;
   delete maskmap;
   delete tempmap;
   debug("~StoneWidget\n");
}



void StoneWidget::newGame() {
  if (!edit) {
    score=0;
    gameover=0;
    highscore=0;
    for (int i=0;i<sx*sy;i++) { 
      field[i]=(char)(rand()%3)+1;
      change[i]=1;
    }
    lastfield=-2;
    emit updateScoreSignal(score);
    emit updateScoreSignal((double)score);
  }
}

long StoneWidget::getScore() {
  return score;
}
int StoneWidget::getgameOver() {
  return gameover;
}
void StoneWidget::showHighscore() {
  highscore=1;
  drawField();
  return;
}
void StoneWidget::hideHighscore() {
  if (!gameover) {
    highscore=0;
    drawField(0,true);
  }
  return;
}

void StoneWidget::drawField(QPaintEvent *e,int erase) {

  static int active=0;
  if (active) {
    debug("already running\n");
    return;
  }
  active=1;
  
  if (!stonemap) {
     stonemap = new QPixmap();
     if (!stonemap) printf("zero stones\n");
     stonemap->loadFromData((const unsigned char *)stone3,stone3_len,"BMP");
  }
  if (!maskmap) {
    maskmap = new QBitmap(mask3_width,mask3_height,
			  (const unsigned char *)mask3_bits,TRUE);
    CHECK_PTR(maskmap);
    stonemap->setMask(*maskmap);
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

  for (int x=0;x<sx;x++) {
    for (int y=0;y<sy;y++) {
      int invalid;
      if (e) {
	QRect r(x*dx,y*dy,dx,dy);
	invalid=erase||r.intersects(e->rect());
      } else {
	invalid=erase;
      }
      char c=field[x+y*sx];
   
      if (c>=0) {
	if (change[x+y*sx]||invalid) {
	  change[x+y*sx]=0;
	  if (c==0) {
	    bitBlt(this,x*dx,y*dy,backmap,x*dx,y*dy,dx,dy,CopyROP,TRUE);
	  } else {
	    bitBlt(tempmap,0,0,backmap,x*dx,y*dy,dx,dy,CopyROP,TRUE);
	    bitBlt(tempmap,0,0,stonemap,0,(c-1)*dy,dx,dy,CopyROP,FALSE);
	    bitBlt(this,x*dx,y*dy,tempmap,0,0,dx,dy,CopyROP,TRUE);
	  }
	}
      } else {
	bitBlt(tempmap,0,0,backmap,x*dx,y*dy,dx,dy,CopyROP,TRUE);
	bitBlt(tempmap,0,0,
	       stonemap,pcount*dx,(-c-1)*dy,dx,dy,CopyROP,FALSE);
	bitBlt(this,x*dx,y*dy,tempmap,0,0,dx,dy,CopyROP,TRUE);
	
      }
      
    }
  }
    
  if (gameover||highscore) {
    QPainter p;
    p.begin( this );

    if (gameover) {
      char *s="game over";
      p.setFont( QFont( "Times", 50, QFont::Bold ) );
      int w=p.fontMetrics().width(s);
      int h=p.fontMetrics().height();
      int x=(width()-w-20);
      int y=0;
      QRect r(x,y,w,h);
      if (!e||r.intersects(e->rect()))
	p.drawText(x,y,w,h,AlignCenter,s);
    } else {
      if (edit) { 
	delete edit; 
	edit=0;
      }
    }
    if (highscore) {
      char *s="--- KSame Highscore ---";
      p.setFont( QFont( "Times", 20, QFont::Bold ) );
      int w=p.fontMetrics().width(s)+30;
      int h=p.fontMetrics().height();
      int x=20;
      int y=5;
      p.drawText(x,y,w,h,AlignCenter,s);
      y+=h;
      w/=2;
  
      if (!edit&&gameover) {
	for (Position=MAXENTRY;Position&&Table[Position-1].score<score;Position--);
	if (Position<MAXENTRY) {	  
	  for (int i=MAXENTRY-1;i>Position;i--) {
	    Table[i].name=Table[i-1].name;
	    Table[i].score=Table[i-1].score;	    
	  }
	  Table[Position].score=score;  
	  Table[Position].name="";
	  edit=new QLineEdit(this);
	  edit->setFont( QFont( "Times", 20, QFont::Bold ) );
	  edit->move(x-4,y+h*Position);
	  edit->resize(w,h);
	  edit->show();
	  connect( edit, SIGNAL(returnPressed()), this, SLOT(returnpressed()));
	}
      }
      for (int i=0;i<MAXENTRY;i++) {
	QString tmp;
	tmp.setNum(Table[i].score);
	//	char tmp[20];
	//sprintf(tmp,"%i",Table[i].score);
	p.drawText(x,y+h*i,w,h,AlignLeft,Table[i].name);
	p.drawText(x+w,y+h*i,w,h,AlignRight,tmp);
      }
    }
    
    p.end();
  }
  active=0;
  return;
}

void StoneWidget::returnpressed() {  
  if (edit&&edit->text()[0]) {
    debug("OK %s\n",edit->text());
    Table[Position].name=edit->text();
    Table[Position].score=score;
    KConfig *conf=kapp->getConfig();
    conf->setGroup("ksame");
    
    for (int i=0;i<MAXENTRY;i++) {
      QString s;
      conf->writeEntry(s.sprintf("Score%i",i),Table[i].score);
      conf->writeEntry(s.sprintf("Name%i",i),Table[i].name);
    }  
    conf->sync();
    delete edit;
    edit=0;    
    score=0;
  }
}


void StoneWidget::paintEvent( QPaintEvent *e ) {
   drawField(e);
}

void StoneWidget::timerEvent( QTimerEvent * )
{
  QPoint p;
  int x,y;

  p=mapFromGlobal(cursor().pos());
  x=p.x();
  y=p.y();
  if ((x<0)||(y<0)||(x>width())||(y>height())||gameover) {
    if (lastfield!=-1) {
      markfield(-1,-1);
      lastfield=-1;
      drawField();
    }
    return;
  }
  pcount=(++pcount)%15;
  drawField();
  return;
}



void StoneWidget::markfield(int x,int y) {
  int i,p;
  char c;
  
  if ((p=x+y*sx)!=lastfield) {
    for (i=0;i<sx*sy;i++) field[i]=abs(field[i]);
    if ((x<0)||(y<0)||(x>sx)||(y>sy)) { 
      lastcolor=-1;      
      return;
    }
    
    lastfield=p;
    stones=0;
    recursive_markfield(p,field[p]);
    c=abs(field[p]);
    if (stones>1) {
    if (abs(field[p])!=lastcolor) pcount=14;
    } else {
      field[p]=c;
   }
    lastcolor=c;
  }
}


void StoneWidget::recursive_markfield(int i,char color) {
  if ((i<0)||(i>sx*sy)) return;
  if ((!field[i])||(abs(field[i])!=color)||(field[i]<0)) return;
  field[i]*=(-1);
  change[i]=1;
  stones++;
  if ((i%sx)!=0) recursive_markfield(i-1,color);  // nicht ganz links
  if (i>=sx) recursive_markfield(i-sx,color);  // nicht ganz oben   
  if (i<(sx-1)*sy) recursive_markfield(i+sx,color);  // nicht ganz unten
  if (((i+1)%sx)!=0) recursive_markfield(i+1,color); // nicht ganz rechts
  return;
}

void StoneWidget::checkGameOver() {
  int i;
  char color;
  
  gameover=1;
  i=sx*sy-1;
  while ((i>=0)&&gameover) {
    while ((field[i]==0)&&(i>=0)&&gameover) i--;
    while (((color=field[i])!=0)&&(i>=0)&&gameover) {
      if (((i%sx)!=0)&&(field[i-1]==color))  // nicht ganz links
	gameover=0;  
      if ((i>=sx)&&(field[i-sx]==color))   // nicht ganz oben   
	gameover=0;  
      if ((((i+1)%sx)!=0)&&(field[i+1]==color))// nicht ganz rechts 
	gameover=0; 
      i--;
    }
  }
  if (gameover) {
    showHighscore();
    emit gameOverSignal(score);
  }
}


void StoneWidget::mouseMoveEvent ( QMouseEvent *e) {
  //  return;
  if (!gameover) {
    markfield((e->pos().x())/dx,(e->pos().y())/dy);
  }
  return;
}

void StoneWidget::mousePressEvent ( QMouseEvent *e) {
  int i;

  if (stones>1&&!gameover) {
    score+=(stones-2)*(stones-2);
    stones=0;
    lastfield=-1;
    for (i=0;i<sx*sy;i++) {
      if (field[i]<0) field[i]=0;
    }

    int j,x,i1,i2;
    for (x=0;x<sx;x++) {
      i1=x+(sy-1)*sx;
      while ((field[i1]!=0)&&(i1>=0)) i1-=sx;
      i2=i1;
      while (i2>=0) {
	while ((field[i2]==0)&&(i2>=0)) i2-=sx;
	while ((field[i2]!=0)&&(i2>=0)) {
	  field[i1]=field[i2];
	  field[i2]=0;
	  i1-=sx;
	  i2-=sx;
	}
      }
    }  
    for (x=0;x<sx;x++) {
      i1=x+(sy-1)*sx;
      while ((field[i1]!=0)&&(i1<sx*sy)) i1++;
      i2=i1;
      while (i2<sx*sy) {
	while ((field[i2]==0)&&(i2<sx*sy)) i2++;
	while ((field[i2]!=0)&&(i2<sx*sy)) {
	  for (j=0;j<sy*sx;j+=sx) {
	    field[i1-j]=field[i2-j];
	    field[i2-j]=0;
	  }
	  i1++;
	  i2++;
	}
      }
    }  
    for (i=0;i<sx*sy;i++) { 
      change[i]=1;
    }
    if (!field[(sy-1)*sx]) {
      debug("Bonus\n");
      score+=1000;
      
    }
    
    markfield((e->pos().x())/dx,(e->pos().y())/dy);
    checkGameOver();
    emit updateScoreSignal(score);
    emit updateScoreSignal((double)score);
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
