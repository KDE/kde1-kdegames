#ifndef KTETRIS_B_H
#define KTETRIS_B_H

#include "gtetris.h"
#include "net.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <qframe.h>
#include <qtimer.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbt.h>
#include <qsocknot.h>

#include <Kconfig.h>

#define SINGLE 1
#define MULTI  2

class Board : public QFrame, public GenericTetris
{
 Q_OBJECT
		
 public:
    Board( int type, QWidget *parent=0, const char *name=0 );

    void gameOver();
    void startGame();

 public slots:
    void updateNext()	   { GenericTetris::updateNext(); }
    void key(QKeyEvent *e) { keyPressEvent(e); }
    void start()		   { startGame(); }
    void pause();
	void hideBoard(bool pause);
	void showHighScores();
	void options();
	void initMultiGame(NetObject *net_obj);
	void pMoveLeft() { if ( !waitingAfterLine ) moveLeft(); }
	void pMoveRight() { if ( !waitingAfterLine ) moveRight(); }
	void pDropDown() { if ( !waitingAfterLine ) dropDown(); }
	void pOneLineDown() { if ( !waitingAfterLine ) oneLineDown(); }
	void pRotateLeft() { if ( !waitingAfterLine ) rotateLeft(); }
	void pRotateRight() { if ( !waitingAfterLine ) rotateRight(); }

 private slots:
	void timeout();
	
 signals:
    void gameOverSignal();
    void drawNextSquareSignal(int x,int y,QColor *color1);
    void updateRemovedSignal(int noOfLines);
    void updateScoreSignal(int score);
    void updateLevelSignal(int level);
	void showOpponents();
	void updateOpponents();

 protected:	
	void updateRemoved(int noOfLines);
	void updateScore(int newScore);
	void pieceDropped(int dropHeight);
	void updateLevel(int newLlevel);
	
	QTimer   *timer;
	int  timeoutTime;
    
	bool noGame;
    bool isPaused;
	NetObject *net_obj;
	
	QLabel   *msg;
	QPushButton *pb;

 private:
    void drawContents( QPainter * );
    void resizeEvent( QResizeEvent * );
    void drawSquare(int x,int y,int value);
    void drawNextSquare(int x,int y,int value);
    void updateTimeoutTime();
	void midbutton(bool); 
	void setHighScore(int);
	void setPieceMovingKeys( bool activate );

    int  xOffset,yOffset;
    int  blockWidth,blockHeight;

    bool waitingAfterLine;

	int gameType;
    QColor    colors[8];
    QPainter *paint;
	QString serror;

	int removedLines, oldFullLines;
	bool multiGame;
	
	bool isConfigWritable;
	KConfig *kconf;
};

void drawTetrisButton( QPainter *, int x, int y, int w, int h,
					   const QColor *color );  

#endif
