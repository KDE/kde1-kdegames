#ifndef _ScoreWIDGET
#define _ScoreWIDGET
#include <qtablevw.h>
#include <qwidget.h>
#include <qpushbt.h>

class ScoreWidget : public QTableView {
 Q_OBJECT
public:
  ScoreWidget( QWidget *parent=0 );
  void paintCell ( QPainter * p, int row, int col );
  int cellWidth (int col);
  void fontChange ( const QFont & oldFont );
public slots:
   void show();
   void hide();
   void toggle();

private:
    QPushButton *closebt;   
    
    int colsize[3];

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
