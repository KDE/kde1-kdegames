#include<qpopmenu.h>
#include<qlayout.h>
#include<qpixmap.h>
#include<qkeycode.h>

#include<kiconloader.h>
#include<kapp.h>
#include<ktopwidget.h>
#include<kmenubar.h>
#include<ktoolbar.h>
#include<kstatusbar.h>
#include<kmsgbox.h>

#include "kpoker.h"

#include "top.moc"

PokerWindow::PokerWindow() :   KTopLevelWidget()
{
	_kpok = new kpok(this,0);
	setView( _kpok, FALSE );
	_kpok->show();
	
	
	menu = new KMenuBar(this, "_mainMenu" );
	menu->enableFloating(false);
	menu->show();
	setMenu( menu );
	
	
	QPopupMenu *filePopup = new QPopupMenu;
	filePopup->insertItem("&New game",_kpok,SLOT(initPoker()));
	filePopup->insertSeparator();
	filePopup->insertItem("&Quit", qApp, SLOT(quit()));
	
	
	QPopupMenu *helpPopup = new QPopupMenu;
	helpPopup->insertItem("&Help", _kpok, SLOT(help()));
	helpPopup->insertSeparator();
	helpPopup->insertItem("&About...", _kpok, SLOT(showAboutBox()));
	
	
	menu->insertItem("&File", filePopup);
	
	menu->insertSeparator();
	menu->insertItem("&Help", helpPopup);
	
	setFixedSize(420,250);
	
	
}

PokerWindow::~PokerWindow()
{
	delete menu;
}

