/****************************************************************
Copyright (c) 1998 Sandro Sigala <ssigala@globalnet.it>.
All rights reserved.

Permission to use, copy, modify, and distribute this software
and its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaim all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
****************************************************************/

#include "config.h"

#include <kapp.h>
#include <kmenubar.h>
#include <kmsgbox.h>
#include <qpushbt.h>
#include <qaccel.h>
#include <qfile.h>
#include <qpopmenu.h>
#include <qtstream.h>
#include <ktopwidget.h>
#include <kstatusbar.h>

#include "ksmiletris.h"
#include "gamewindow.h"
#include "gamewidget.h"

const int default_width = 362;
const int default_height = 460;

GameWindow::GameWindow(QWidget *, const char *name)
        : KTopLevelWidget(name)
{
	setCaption("Smiletris");

	file_popup = new QPopupMenu();
	file_popup->insertItem("&New Game", this, SLOT(menu_newGame()), CTRL+Key_N);
        pauseID = file_popup->insertItem("&Pause", this, SLOT(menu_pause()), Key_F2);
	file_popup->insertSeparator();
	file_popup->insertItem("&Quit", qApp, SLOT(quit()), CTRL+Key_Q);

	pieces_popup = new QPopupMenu();
	smilesID = pieces_popup->insertItem("&Smiles", this, SLOT(menu_smiles()));
	symbolsID = pieces_popup->insertItem("S&ymbols", this, SLOT(menu_symbols()));

	iconsID = pieces_popup->insertItem("&Icons", this, SLOT(menu_icons()));

	options_popup = new QPopupMenu();
	options_popup->insertItem("&Pieces", pieces_popup);

	help_popup = new QPopupMenu();
	help_popup->insertItem("&Contents", this, SLOT(menu_help()), Key_F1);
	help_popup->insertSeparator();
	help_popup->insertItem("&About KSmiletris...", this, SLOT(menu_about()));
//	help_popup->insertItem("About &KDE...", this, SLOT(menu_aboutKDE()));

	menu = new KMenuBar(this);
	menu->insertItem("&File", file_popup);
	menu->insertItem("&Options", options_popup);
	menu->insertItem("&Help", help_popup);
	setMenu(menu);
	connect(menu, SIGNAL(moved(menuPosition)),
		this, SLOT(movedMenu(menuPosition)));

	status = new KStatusBar(this);
	status->insertItem("Level: 99", 1);
	status->insertItem("Points: 999999", 2);
	setStatusBar(status);
	status->changeItem("", 1);
	status->changeItem("", 2);

	game = new GameWidget(this);
	setView(game);
	connect(game, SIGNAL(changedStats(int, int)),
		this, SLOT(updateStats(int, int)));
	connect(game, SIGNAL(noStats()), this, SLOT(noStats()));

	resize(default_width, default_height + menu->height() + status->height());
	setMaximumSize(width(), height());
	setMinimumSize(width(), height());

	// Read configuration
	KConfig *config = kapp->getConfig();
	config->setGroup("Options");
	PiecesType pieces_type = (PiecesType)config->readNumEntry("Pieces", Pieces_Smiles);
	game->setPieces(pieces_type);
	switch (pieces_type) {
	case Pieces_Smiles:
		pieces_popup->setItemChecked(smilesID, true);
		break;
	case Pieces_Symbols:
		pieces_popup->setItemChecked(symbolsID, true);
		break;
	case Pieces_Icons:
		pieces_popup->setItemChecked(iconsID, true);
	}
}

void GameWindow::menu_newGame()
{
	file_popup->setItemChecked(pauseID, false);
	game->newGame();
}

void GameWindow::menu_pause()
{
	if (game->in_game) {
		game->in_pause = !game->in_pause;
		file_popup->setItemChecked(pauseID, game->in_pause);
		game->repaintChilds();
	}
}

void GameWindow::menu_smiles()
{
	options_popup->setItemChecked(smilesID, true);
	options_popup->setItemChecked(symbolsID, false);
	options_popup->setItemChecked(iconsID, false);
	KConfig *config = kapp->getConfig();
	config->setGroup("Options");
	config->writeEntry("Pieces", Pieces_Smiles);
	game->setPieces(Pieces_Smiles);
}

void GameWindow::menu_symbols()
{
	options_popup->setItemChecked(smilesID, false);
	options_popup->setItemChecked(symbolsID, true);
	options_popup->setItemChecked(iconsID, false);
	KConfig *config = kapp->getConfig();
	config->setGroup("Options");
	config->writeEntry("Pieces", Pieces_Symbols);
	game->setPieces(Pieces_Symbols);
}

void GameWindow::menu_icons()
{
	options_popup->setItemChecked(smilesID, false);
	options_popup->setItemChecked(symbolsID, false);
	options_popup->setItemChecked(iconsID, true);
	KConfig *config = kapp->getConfig();
	config->setGroup("Options");
	config->writeEntry("Pieces", Pieces_Icons);
	game->setPieces(Pieces_Icons);
}

void GameWindow::menu_help()
{
	KApplication::getKApplication()->invokeHTMLHelp("","");
}

void GameWindow::menu_about()
{
	KMsgBox::message(this, "About KSmiletris",
			 "KSmiletris " VERSION "\n\n"
			 "by Sandro Sigala <ssigala@globalnet.it>\n\n"
			 "Feedbacks are welcome!\n",
			 KMsgBox::INFORMATION);
}

void GameWindow::menu_aboutKDE()
{
}

void GameWindow::updateStats(int level, int points)
{
	QString l, p;
	l.setNum(level);
	p.setNum(points);
	status->changeItem(QString("Level: ") + l, 1);
	status->changeItem(QString("Points: ") + p, 2);
}

void GameWindow::noStats()
{
	status->changeItem("", 1);
	status->changeItem("", 2);
}

void GameWindow::movedMenu(menuPosition pos)
{
	if (pos == Top) {
		setMinimumSize(default_width, default_height + menu->height() + status->height());
		setMaximumSize(default_width, default_height + menu->height() + status->height());
		resize(width(), height() + menu->height());
	} else {
		setMinimumSize(default_width, default_height + status->height());
		setMaximumSize(default_width, default_height + status->height());
		resize(width(), height() - menu->height());
	}
}
