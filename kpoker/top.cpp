/*
 *  $Id$
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *
 * Comment:
 * This is my first "bigger" application I did with Qt and my very first KDE application.
 * So please apologize some strange coding styles --> if you can't stand some really
 * bad parts just send me a patch including you "elegant" code ;)
*/  

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
#include "global.h"

const int ID_SOUND = 100;
int soId;

PokerWindow::PokerWindow() :   KTopLevelWidget()
{
	locale = kapp->getLocale();
	
	_kpok = new kpok(this,0);
	setView( _kpok, FALSE );
	_kpok->show();
	
	
	menu = new KMenuBar(this, "_mainMenu" );
	menu->show();
	setMenu( menu );
	
	
	filePopup = new QPopupMenu(0,"fpopup");
	filePopup->insertItem(locale->translate("&New game"),_kpok,SLOT(initPoker()));
	filePopup->insertSeparator();
	filePopup->insertItem(locale->translate("&Quit"), qApp, SLOT(quit()));

       	optionsPopup = new QPopupMenu(0,"oppup");
	
	soId = optionsPopup->insertItem(locale->translate("&Sound"), this, SLOT(toggleSound()));
	optionsPopup->setCheckable(TRUE);

       	if (_kpok->initSound() == 1) {
	  optionsPopup->setItemChecked(soId, TRUE);
	}
	else {
	  optionsPopup->setItemEnabled(soId, FALSE);
	}
	
	helpPopup = new QPopupMenu(0,"helpp");
	helpPopup->insertItem(locale->translate("&Help"), _kpok, SLOT(help()));
	helpPopup->insertSeparator();
	helpPopup->insertItem(locale->translate("&About..."), _kpok, SLOT(showAboutBox()));
	helpPopup->insertItem(locale->translate("About &Qt..."), _kpok, SLOT(showQtAboutBox()));
	
	menu->insertItem(locale->translate("&File"), filePopup);
	menu->insertItem(locale->translate("&Options"), optionsPopup);
	
	menu->insertSeparator();
	menu->insertItem(locale->translate("&Help"), helpPopup);
	
}

PokerWindow::~PokerWindow()
{
	delete menu;
}

int PokerWindow::toggleSound()
{
  if (optionsPopup->isItemChecked(soId) == TRUE)
    optionsPopup->setItemChecked(soId, FALSE);
  else
    optionsPopup->setItemChecked(soId, TRUE);
  _kpok->toggleSound();
  return 1;
}



