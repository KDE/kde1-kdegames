
#ifndef __KAST_TOPLEVEL_H__
#define __KAST_TOPLEVEL_H__

#include <kstatusbar.h>
#include <ktoolbar.h>
#include <ktopwidget.h>
#include <qlcdnum.h>
#include <kmenubar.h>

#include "kfixedtopwidget.h"
#include "kastmsg.h"
#include "view.h"


class KAstTopLevel : public KFixedTopWidget
{
    Q_OBJECT
public:
    KAstTopLevel();

private:
    void createMenuBar();

protected:
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void keyReleaseEvent( QKeyEvent *event );
    virtual void focusInEvent( QFocusEvent *event );
    virtual void focusOutEvent( QFocusEvent *event );

private slots:
    void slotNewGame();
    void slotQuit();
    void slotHelp();
    void slotAbout();

    void slotShipKilled();
    void slotRockHit( int size );
    void slotRocksRemoved();

private:
    KAsteroidsView *view;
    KMenuBar *menu;
    QLCDNumber *scoreLCD;
    QLCDNumber *levelLCD;
    QLCDNumber *shipsLCD;
    KAstMsg *message;

    // waiting for user to press Enter to launch a ship
    bool waitShip;

    int shipsRemain;
    int score;
    int level;
};

#endif

