#include <qlcdnum.h>
#include <qkeycode.h>
#include <qcolor.h>
#include <qpopmenu.h>

#include <kapp.h>
#include <kmsgbox.h>
#include <kmenubar.h>

#include "rattler.h"
#include "trys.h"
#include "score.h"
#include "progress.h"
#include "game.h"
#include "lcdrange.h"
#include "startroom.h"

#include "levels.h"

Game::Game( QWidget *parent, const char *name )
    : QWidget( parent, name )
{

    setCaption("Snake Race");
    setIcon("Snake Race");

    conf = kapp->getConfig();
    if(conf == NULL) {
	printf("KConfig error ??\n");
	kapp->quit();
    }

    levels = new Levels();
    score     = new Score;
    menu();
    checkMenuItems();

    lcd  = new QLCDNumber( this);
    lcd->setFrameStyle( QFrame::Panel | QFrame::Sunken );

    trys = new Trys(this);
    pg = new Progress(this);
    rattler = new Rattler( this);

    connect(rattler, SIGNAL(setPoints(int)), lcd, SLOT(display(int)));
    connect(rattler, SIGNAL(setTrys(int)), trys, SLOT(set(int)));
    connect(rattler, SIGNAL(rewind()), pg, SLOT(rewind()));
    connect(rattler, SIGNAL(advance()), pg, SLOT(advance()));
    connect(pg, SIGNAL(restart()), rattler, SLOT(restartTimer()));
    connect(rattler, SIGNAL(setScore(int)), score, SLOT(setScore(int)));

    int y;

    menubar->setGeometry(0, 0, rattler->width(), menubar->height() );
    y = menubar->height() +2;
    lcd->setGeometry(420, y+5, 135, 42);
    trys->setGeometry(0, y, 405, 50);
    y += 52;
    pg->setGeometry(5, y, 550, 12);
    y += 14;
    rattler->setGeometry(0, y, rattler->width(), rattler->height());
    setFixedSize(rattler->width(), y + rattler->height() );
    rattler->setFocus();

}


void Game::menu()
{
    QPopupMenu *game = new QPopupMenu();
    CHECK_PTR( game );
    game->insertItem( "New", this, SLOT(newGame()),Key_F2);
    game->insertItem( "Pause", this , SLOT(pauseGame()), Key_F3);
    game->insertItem( "High Scores...", this, SLOT(showHighScores()));
    game->insertSeparator();
    game->insertItem( "Exit",  this, SLOT(quitGame()), Key_F10 );

    balls = new QPopupMenu;
    CHECK_PTR( balls );
    ballsID[0] = balls->insertItem( "0");
    ballsID[1] = balls->insertItem( "1");
    ballsID[2] = balls->insertItem( "2");
    ballsID[3] = balls->insertItem( "3");
    balls->setCheckable( TRUE );
    connect(balls, SIGNAL(activated(int)), this, SLOT ( ballsChecked(int) ));

    snakes = new QPopupMenu;
    CHECK_PTR( snakes );
    snakesID[0] = snakes->insertItem( "0");
    snakesID[1] = snakes->insertItem( "1");
    snakesID[2] = snakes->insertItem( "2");
    snakesID[3] = snakes->insertItem( "3");
    snakes->setCheckable( TRUE );
    connect(snakes, SIGNAL(activated(int)), this,
	    SLOT ( snakesChecked(int) ));

    options = new QPopupMenu();
    CHECK_PTR( options );
    skillID[0] = options->insertItem( "Beginner");
    skillID[1] = options->insertItem( "Intermediate");
    skillID[2] = options->insertItem( "Advanced");
    skillID[3] = options->insertItem( "Expert");
    options->insertSeparator();
    options->insertItem("Balls", balls);
    options->insertItem("Computer Snakes", snakes);
    options->insertSeparator();
    options->insertItem("Starting Room...", this, SLOT(startingRoom()));
    options->setCheckable( TRUE );
    connect(options, SIGNAL(activated(int)), this, SLOT ( skillChecked(int) ));

    QPopupMenu *help = new QPopupMenu();
    CHECK_PTR( help );
    help->insertItem("Help", this, SLOT(help()), Key_F1);
    help->insertSeparator();
    help->insertItem("About Snake Race...", this, SLOT(about()) );

    menubar = new KMenuBar( this );
    CHECK_PTR( menu );
    menubar->insertItem( "&Game", game );
    menubar->insertItem( "&Options", options );
    menubar->insertSeparator();
    menubar->insertItem( "&Help", help);
}

void Game::about()
{
    KMsgBox::message(0, "About Snake Race",
		     "Snake Race \n\nMichel Filippi (mfilippi@sade.rhein-main.de) \n\nA snake game for the KDE Desktop",
		     KMsgBox::INFORMATION, "Ok");
}

void Game::help()
{
    KApplication::getKApplication()->invokeHTMLHelp(0, 0);
}

void Game::ballsChecked(int id)
{
    for ( int x = 0; x < 4; x++)
	if (ballsID[x] != id)
	    balls->setItemChecked( ballsID[x], FALSE );
	else { balls->setItemChecked( ballsID[x], TRUE );
	conf->writeEntry("Balls", x);
	rattler->setBalls(x);
	}
}

void Game::snakesChecked(int id)
{
    for ( int x = 0; x < 4; x++)
	if (snakesID[x] != id)
	    snakes->setItemChecked( snakesID[x], FALSE );
	else { snakes->setItemChecked( snakesID[x], TRUE );
	conf->writeEntry("ComputerSnakes", x);
	rattler->setCompuSnakes(x);
	}
}

void Game::skillChecked(int id)
{
    if (options->indexOf(id) > 3)
	return;

    for ( int x = 0; x < 4; x++)
	if (skillID[x] != id)
	    options->setItemChecked( skillID[x], FALSE );
	else { options->setItemChecked( skillID[x], TRUE );
	conf->writeEntry("Skill", x);
	rattler->setSkill(x);
	}
}

void Game::checkMenuItems()
{
    balls->setItemChecked( ballsID[conf->readNumEntry("Balls", 1)], TRUE );
    snakes->setItemChecked( snakesID[conf->readNumEntry("ComputerSnakes", 1)], TRUE );
    options->setItemChecked( skillID[conf->readNumEntry("Skill", 1)], TRUE );
}

void Game::quitGame()
{
    qApp->quit();
}

void Game::showHighScores()
{
    score->display();
}

void Game::newGame()
{
    rattler->restart();
}

void Game::pauseGame()
{
    rattler->pause();
}

void Game::startingRoom()
{
    int r = 0;
    StartRoom *sr = new StartRoom(conf->readNumEntry("StartingRoom", 1), &r);
    sr->exec();
    delete sr;

    if (r != 0) {
	conf->writeEntry("StartingRoom", r);
	rattler->setRoom(r);
    }
}
