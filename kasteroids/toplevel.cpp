
#include <kapp.h>
#include <qaccel.h>
#include <qmsgbox.h>
#include <kstatusbar.h>
#include <ktoolbar.h>
#include <ktopwidget.h>
#include <kmenubar.h>
#include <klocale.h>

#include "kfixedtopwidget.h"
#include "toplevel.h"

#include "toplevel.moc"


#define SB_SCORE	1
#define SB_LEVEL	2
#define SB_SHIPS	3


struct SLevel
{
    int    nrocks;
    double rockSpeed;
};

#define MAX_LEVELS	16

SLevel levels[MAX_LEVELS] =
{
    { 1, 0.8 },
    { 1, 1.2 },
    { 2, 1.0 },
    { 2, 1.2 },
    { 2, 1.8 },
    { 3, 1.0 },
    { 3, 1.2 },
    { 3, 1.8 },
    { 4, 1.0 },
    { 4, 1.2 },
    { 4, 1.8 },
    { 5, 1.0 },
    { 5, 1.5 },
    { 5, 2.0 },
    { 5, 2.5 }
};

KAstTopLevel::KAstTopLevel() : KFixedTopWidget()
{
    view = new KAsteroidsView( this );
    connect( view, SIGNAL( shipKilled() ), SLOT( slotShipKilled() ) );
    connect( view, SIGNAL( rockHit(int) ), SLOT( slotRockHit(int) ) );
    connect( view, SIGNAL( rocksRemoved() ), SLOT( slotRocksRemoved() ) );

    createMenuBar();
    createToolBar();
    createStatusBar();

    view->setFixedSize( 640, 480 );
    view->show();
//    toolbar->show();
    statusbar->show();
    menu->show();
    setView( view );
    addToolBar( toolbar );
    setStatusBar( statusbar );
    setMenu( menu );

    message = new KAstMsg( this );
    message->hide();

    setFocusPolicy( StrongFocus );
    setFocus();
}

KAstTopLevel::~KAstTopLevel()
{
    delete toolbar;
}

void KAstTopLevel::createMenuBar()
{
    menu = new KMenuBar( this );
    CHECK_PTR( menu );

    QPopupMenu *fileMenu = new QPopupMenu;
    CHECK_PTR( fileMenu );
    fileMenu->insertItem( klocale->translate( "&New Game" ), this,
	SLOT(slotNewGame()), CTRL+Key_N );
    fileMenu->insertSeparator( );
    fileMenu->insertItem( klocale->translate( "&Quit" ), this, SLOT(slotQuit()),
	CTRL+Key_Q );

    QPopupMenu *helpMenu = new QPopupMenu;
    CHECK_PTR( helpMenu );
    helpMenu->insertItem( klocale->translate( "&About" ), this,
	SLOT(slotAbout()) );

    menu->insertItem( klocale->translate( "&File" ), fileMenu );
    menu->insertSeparator();
    menu->insertItem( klocale->translate( "&Help" ), helpMenu );
}

void KAstTopLevel::createToolBar()
{
    toolbar = new KToolBar( this );
    toolbar->enable( KToolBar::Hide );
}

void KAstTopLevel::createStatusBar()
{
    statusbar = new KStatusBar( this );
    statusbar->insertItem( "Score:         ", SB_SCORE );
    statusbar->insertItem( "Level:         ", SB_LEVEL );
    statusbar->insertItem( "Ships:       ", SB_SHIPS );
}

void KAstTopLevel::keyPressEvent( QKeyEvent *event )
{
    switch ( event->key() )
    {
	case Key_Left:
	    view->rotateLeft( true );
	    event->accept();
	    break;

	case Key_Right:
	    view->rotateRight( true );
	    event->accept();
	    break;

	case Key_Up:
	    view->thrust( true );
	    event->accept();
	    break;

	case Key_Space:
	    view->shoot();
	    event->accept();
	    break;

	default:
	    event->ignore();
    }
}

void KAstTopLevel::keyReleaseEvent( QKeyEvent *event )
{
    switch ( event->key() )
    {
	case Key_Left:
	    view->rotateLeft( false );
	    event->accept();
	    break;

	case Key_Right:
	    view->rotateRight( false );
	    event->accept();
	    break;

	case Key_Up:
	    view->thrust( false );
	    event->accept();
	    break;

	case Key_Return:
	    if ( waitShip )
	    {
		view->newShip();
		event->accept();
		waitShip = false;
		message->hide();
	    }
	    break;

	default:
	    event->ignore();
    }
}

void KAstTopLevel::slotNewGame()
{
    score = 0;
    statusbar->changeItem( "Score:      0", SB_SCORE );
    level = 0;
    statusbar->changeItem( "Level:    1", SB_SCORE );
    shipsRemain = 5;
    statusbar->changeItem( "Ships:    5", SB_SHIPS );
    view->newGame();
    view->setRockSpeed( levels[0].rockSpeed );
    view->addRocks( levels[0].nrocks );
    message->setMessage( klocale->translate( "Press Enter to launch.") );
    message->move( ( width() - message->width() ) / 2, 80 );
    message->show();
    waitShip = true;
}

void KAstTopLevel::slotQuit()
{
    kapp->quit();
}

void KAstTopLevel::slotAbout()
{
    QMessageBox::message( klocale->translate("KAsteriods"),
	"KAsteroids Version 0.0.1\n\nCopyright (c) Martin R. Jones 1997" );
}

void KAstTopLevel::slotShipKilled()
{
    shipsRemain--;
    char buffer[80];
    sprintf( buffer, "Ships: %5d", shipsRemain );
    statusbar->changeItem( buffer, SB_SHIPS );

    if ( shipsRemain )
    {
	message->setMessage( klocale->translate( "Ship Destroyed.  Press Enter to launch.") );
	waitShip = true;
	message->move( ( width() - message->width() ) / 2, 80 );
	message->show();
    }
    else
    {
	QMessageBox::message(klocale->translate("KAsteriods"),
	    klocale->translate("Game Over!"));
	view->endGame();
    }
}

void KAstTopLevel::slotRockHit( int size )
{
    switch ( size )
    {
	case 0:
	    score += 10;
	    break;

	case 1:
	    score += 20;
	    break;

	default:
	    score += 40;
    }

    char buffer[80];
    sprintf( buffer, "Score: %6d", score );
    statusbar->changeItem( buffer, SB_SCORE );
}

void KAstTopLevel::slotRocksRemoved()
{
    level++;

    if ( level >= MAX_LEVELS )
	level = MAX_LEVELS - 1;

    view->setRockSpeed( levels[level-1].rockSpeed );
    view->addRocks( levels[level-1].nrocks );

    char buffer[80];
    sprintf( buffer, "Level: %5d", level );
    statusbar->changeItem( buffer, SB_LEVEL );
}

