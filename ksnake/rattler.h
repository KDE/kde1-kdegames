#ifndef RATTLER_H
#define RATTLER_H

#include <qwidget.h>
#include <qlist.h>

#include "board.h"
#include "level.h"
#include "basket.h"
#include "ball.h"
#include "pixServer.h"
#include "snake.h"

enum GameState { Init, Playing, Demo, Paused, Over };

class Rattler : public QWidget
{
    Q_OBJECT
public:
    Rattler ( QWidget *parent=0, const char *name=0 );

    void setBalls(int);
    void setCompuSnakes(int);
    void setSkill(int);
    void setRoom(int);

public slots:
    void closeGate(int);
    void openGate();
    void scoring(bool, int);
    void restart();
    void newTry();
    void pause();
    void restartTimer();
    void levelUp();
    void speedUp();
    void runDemo();
    void runPlay();
    void demo();

private slots:
    void start();
    void stop();
    void showRoom();
    void restartDemo();

signals:
    void setPoints(int);
    void setTrys(int);

    void setScore(int);

    // progress
    void rewind();
    void advance();

protected:
    void  timerEvent( QTimerEvent * );
    void  paintEvent( QPaintEvent * );
    void  keyPressEvent( QKeyEvent * );
    void  focusOutEvent( QFocusEvent * ) { ; }
    void  focusInEvent( QFocusEvent * )  { ; }
private:

    GameState gameState;

    int  timerCount;
    bool leaving;

    int check;
    int points;
    int trys;

    int direction;

    Board     *board;
    PixServer *pix;
    Level     *level;
    Basket     *basket;
    SamySnake *samy;

    QList<Ball> *balls;
    void restartBalls(bool);
    int numBalls;

    QList<CompuSnake> *computerSnakes;
    void restartComputerSnakes(bool);
    int numSnakes;

    int room;
    int skill;

    int gameTimer;
    bool timerHasRunOut;
    void start(int);
    void resetFlags();
    void init(bool);
};


#endif // RATTLER_H
