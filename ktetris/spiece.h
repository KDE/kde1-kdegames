#ifndef SPIECE_H
#define SPIECE_H

#include <qframe.h>

class ShowNextPiece : public QFrame
{
 Q_OBJECT
 friend class QTetris;
	
 public:
    ShowNextPiece( QWidget *parent=0, const char *name=0  );
 
 public slots:
    void drawNextSquare( int x, int y,QColor *color );

 signals:
    void update();
	
 private:
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );
    
    int  blockWidth, blockHeight;
    int  xOffset, yOffset;
};

#endif // SPIECE_H
