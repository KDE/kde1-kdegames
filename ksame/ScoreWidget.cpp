#include <stdio.h>
#include <qbutton.h> 
#include <qwidget.h>
#include <qlabel.h>
#include <qlined.h> 
#include "ScoreWidget.h"
#include "ScoreWidget.moc"

 ScoreWidget::ScoreWidget( QWidget *parent ) : QWidget(parent) {
 
   int w=400,h=500;
   
   QLabel *label=new QLabel("---   Highscore   ---",this);
   label->setFont( QFont( "Times", 20, QFont::Bold ) );
   label->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
   label->setAlignment( AlignCenter );
   label->show();
   label->adjustSize();  
   label->resize(300,label->height());
   label->move(5,5);
   w=label->width()+10;
   h=label->height()+10;
   
   QLineEdit *edit;
   //   QLabel *label;
   
   for (int i=0;i<10;i++) {
     
     if (i==5) {
       edit=new QLineEdit(this);
       edit->setFont( QFont( "Times", 20, QFont::Bold ) );
       edit->move(5,h);
       edit->resize(w/2-10,edit->height());
       edit->setText("edit");
     } else {
       label=new QLabel(this);
       label->setFont( QFont( "Times", 20, QFont::Bold ) );
       label->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
       label->setAlignment( AlignCenter );
       label->move(5,h);
       label->resize(w/2-10,label->height());
       label->setText("Label");
     }
     label=new QLabel(this);
     label->setFont( QFont( "Times", 20, QFont::Bold ) );
     label->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
     label->setAlignment( AlignCenter );
     label->move(w/2+5,h);
     label->resize(w/2-10,label->height());
     label->setText("1231254");
     h+=label->height();
   }
   
  h+=10; 
   
   closebt=new QPushButton("Close",this,"closebt");
   connect( closebt, SIGNAL(clicked()), this, SLOT(hide()));
   closebt->move(0,h);
  
  h+=closebt->height();
  
  
  setGeometry(0,0,w,h); 
  setFixedSize(w,h);
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
