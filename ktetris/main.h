#ifndef KTETRIS_H
#define KTETRIS_H

#include "board.h"
#include "net.h"

#include <qframe.h>
#include <qlcdnum.h>
#include <qpushbt.h>
#include <qpainter.h>
#include <qmenubar.h>
#include <qpopmenu.h>

#include <kprogress.h>
#include <Kconfig.h>


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


class KTetris : public QWidget
{
 Q_OBJECT
		
 public:
    KTetris( QWidget *parent=0, const char *name=0 );

	bool menu_visible;
	
 signals:
	void initMultiGame(NetObject *net_obj);
	
 private slots:
	void about();
	void help();
	void multiGame();
	void showOpponents();
	void updateOpponents();
	void configKeys();
	void toggleMenu();
	void quit();
	
 protected:
	void resizeEvent(QResizeEvent *);
	bool eventFilter(QObject *, QEvent *e);
	
 private:
    Board  *board;
    ShowNextPiece *showNext;

	KConfig *kconf;
	
	QMenuBar *menu;
	QPopupMenu *popup;
	int tog_id;
	
    QLCDNumber *showScore, *showLevel, *showLines;
    QPushButton *quitButton , *startButton, *pauseButton;
	QLabel *lScore, *lLines, *lLevel, *lNext;
	QLabel *labPrevName, *labNextName;
	KProgress *prevHeight, *nextHeight;
	
	NetObject *net_obj;
};

#endif
