#include <stdio.h>
#include <qbutton.h> 
#include <qwidget.h>
#include <qlabel.h>
#include <qlined.h> 
#include <qpainter.h> 
#include "ScoreWidget.h"
#include "ScoreWidget.moc"

 ScoreWidget::ScoreWidget( QWidget *parent ) : QTableView(parent) {
 
   int w=400,h=100;

   setNumCols(3);
   setNumRows(10);
   //   setCellWidth(30);

   h=fontMetrics().height();

   setCellHeight(h);
   h*=10;
  colsize[0]=fontMetrics().maxWidth()*3;
  colsize[1]=fontMetrics().maxWidth()*20;
  colsize[2]=fontMetrics().maxWidth()*5;
  w=colsize[0]+colsize[1]+colsize[2];
  resize(w,h);
 
  return;
   
   QLineEdit *edit;
    QLabel *label;
   
    int hx=10;
   for (int i=0;i<10;i++) {
     
     if (i==5) {
       edit=new QLineEdit(this);
       //       edit->setFont( QFont( "Times", 20, QFont::Bold ) );
       hx=edit->sizeHint().height();
       edit->move(5,h);
       edit->resize(w/2-10,hx);
       edit->setText("edit");
     } else {
       label=new QLabel("Label",this);
       //  label->setFont( QFont( "Times", 20, QFont::Bold ) );
       //  label->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
       label->setAlignment( AlignLeft );
       hx=label->height();
       label->move(5,h);
       label->resize(w/2-10,hx);
     }
     label=new QLabel("123456",this);
     //     label->setFont( QFont( "Times", 20, QFont::Bold ) );
     //     label->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
     label->setAlignment( AlignLeft );
     hx=label->height();
     label->move(w/2+5,h);
     label->resize(w/2-10,hx);
     h+=hx;
   }
   
  h+=10; 
  
  resize(w,h);
}

void ScoreWidget::fontChange ( const QFont & oldFont ) {
    int h;
    int w=400;
    h=fontMetrics().height();
    setCellHeight(h);
    h*=10;
    colsize[0]=fontMetrics().maxWidth()*3;
    colsize[1]=fontMetrics().maxWidth()*20;
    colsize[2]=fontMetrics().maxWidth()*5;
    w=colsize[0]+colsize[1]+colsize[2];
    updateTableSize();
    resize(w,h);    
    update();
    return;
}

int ScoreWidget::cellWidth (int col) {
    switch(col) {
    case 0: return(colsize[0]);
    case 1: return(colsize[1]);
    case 2: return(colsize[2]);
    default: return(100);
    }
}

void ScoreWidget::paintCell ( QPainter * p, int row, int col ) {
 p->drawText(0,0,"123");
return;
}

void ScoreWidget::hide() {
  QWidget::hide();
}


void ScoreWidget::show() {
  QWidget::show();
}

void ScoreWidget::toggle() {
 if (isVisible())  QWidget::hide(); 
 else  QWidget::show();
}

   /*   
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
	  edit->setFocus();
	  
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
    if (edit)
        delete edit;
    edit=0;    
    score=0;
  }
}
 KConfig *conf=kapp->getConfig();
  conf->setGroup("ksame");
  for (int i=0;i<MAXENTRY;i++) {
    QString s;
    Table[i].score=conf->readNumEntry(s.sprintf("Score%i",i),0);
    Table[i].name=conf->readEntry(s.sprintf("Name%i",i),"FOR YOU");
  }  

*/
