#ifndef _HighScoreWIDGET
#define _HighScoreWIDGET
#include <qwidget.h>
#include <qtabdlg.h>
#include <qpushbt.h>

class HighScore : public QTabDialog {
 Q_OBJECT
public:
  HighScore( QWidget *parent=0 );

private:
  QPushButton *closebt;   
    
 
};

#endif
