
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

#include <X11/Xlib.h>

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
    { 1, 1.0 },
    { 2, 0.8 },
    { 2, 1.0 },
    { 2, 1.1 },
    { 3, 0.9 },
    { 3, 1.1 },
    { 3, 1.2 },
    { 4, 1.0 },
    { 4, 1.1 },
    { 4, 1.2 },
    { 5, 1.0 },
    { 5, 1.1 },
    { 5, 1.3 },
    { 5, 1.5 }
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
    QString text;
    text.sprintf("%s:         ", klocale->translate("Score"));
    statusbar->insertItem( text, SB_SCORE );
    text.sprintf("%s:         ", klocale->translate("Level"));
    statusbar->insertItem( text, SB_LEVEL );
    text.sprintf("%s:       ", klocale->translate("Ships"));
    statusbar->insertItem( text, SB_SHIPS );
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
	    view->shoot( true );
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

	case Key_Space:
	    view->shoot( false );
	    event->accept();
	    break;

	default:
	    event->ignore();
    }
}

void KAstTopLevel::focusInEvent( QFocusEvent * )
{
    XAutoRepeatOff( qt_xdisplay() );
}

void KAstTopLevel::focusOutEvent( QFocusEvent * )
{
    XAutoRepeatOn( qt_xdisplay() );
}

void KAstTopLevel::slotNewGame()
{
    QString text;
    score = 0;
    text.sprintf("%s:      0", klocale->translate("Score"));
    statusbar->changeItem( text, SB_SCORE );
    level = 0;
    text.sprintf("%s:      1", klocale->translate("Level"));
    statusbar->changeItem(text , SB_SCORE );
    shipsRemain = 5;
    text.sprintf("%s:    5", klocale->translate("Ships"));
    statusbar->changeItem( text, SB_SHIPS );
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
    sprintf( buffer, "%s: %5d", klocale->translate("Ships"), shipsRemain );
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
    sprintf( buffer, "%s: %6d", klocale->translate("Score"), score );
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
    sprintf( buffer, "%s: %5d", klocale->translate("Level"), level );
    statusbar->changeItem( buffer, SB_LEVEL );
}

