
#ifndef __KAST_TOPLEVEL_H__
#define __KAST_TOPLEVEL_H__

#include <kstatusbar.h>
#include <ktoolbar.h>
#include <ktopwidget.h>
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
    void createToolBar();
    void createStatusBar();

protected:
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void keyReleaseEvent( QKeyEvent *event );
    virtual void focusInEvent( QFocusEvent *event );
    virtual void focusOutEvent( QFocusEvent *event );

private slots:
    void slotNewGame();
    void slotQuit();
    void slotAbout();

    void slotShipKilled();
    void slotRockHit( int size );
    void slotRocksRemoved();

private:
    KAsteroidsView *view;
    KMenuBar *menu;
    KToolBar *toolbar;
    KStatusBar *statusbar;
    KAstMsg *message;

    // waiting for user to press Enter to launch a ship
    bool waitShip;

    int shipsRemain;
    int score;
    int level;
};

#endif

