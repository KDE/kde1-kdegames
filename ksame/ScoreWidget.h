#ifndef _ScoreWIDGET
#define _ScoreWIDGET
#include <qwidget.h>
#include <qpushbt.h>

class ScoreWidget : public QWidget {
 Q_OBJECT
public:
  ScoreWidget( QWidget *parent=0 );

public slots:
   void show();
   void hide();
   void toggle();

private:
  QPushButton *closebt;   
    
};

#endif

/*
define MAXENTRY 10
    struct {
	QString name;
	long score;
    } Table[MAXENTRY];
    
    int Position;
    
    */
