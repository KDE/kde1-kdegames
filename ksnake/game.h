#ifndef GAME_H
#define GAME_H

#include <kmenubar.h>
#include <qpopmenu.h>
#include <qlcdnum.h>

#include <kapp.h>

#include "rattler.h"
#include "trys.h"
#include "score.h"
#include "progress.h"
#include "levels.h"


class Game : public QWidget
{
    Q_OBJECT
public:
    Game( QWidget *parent=0, const char *name=0 );
protected:
    //   void keyPressEvent( QKeyEvent * );
private slots:
    void ballsChecked(int);
    void skillChecked(int);
    void snakesChecked(int);

    void newGame();
    void pauseGame();
    void quitGame();

    void showHighScores();
    void startingRoom();


    void help();
    void about();

private:

    Rattler *rattler;
    QLCDNumber *lcd;

    Trys *trys;
    Score   *score;
    Progress *pg;
    Levels *levels;

    KConfig *conf;

    void menu();
    void checkMenuItems();
    KMenuBar *menubar;
    QPopupMenu *options;
    QPopupMenu *balls;
    QPopupMenu *snakes;
    int ballsID[4];
    int snakesID[4];
    int skillID[4];
};

#endif // GAME_H
