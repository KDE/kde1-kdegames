#include "main.h"

#include <qdatetm.h>
#include <qmsgbox.h>
#include <kmsgbox.h>
#include <kkeyconf.h>
#include "defines.h"
#include "field.h"

#include "main.moc"

KTetris::KTetris() 
{
	QTime t = QTime::currentTime();
	srand( t.hour()+t.minute()+t.second() );
	
	field = new Field(this);
	setView(field);
	
	/* menu */
	popup = new QPopupMenu;
	tog_id = popup->insertItem(klocale->translate("Hide menu"), this, SLOT(toggleMenu()) );
	field->fillPopup(popup);
	popup->insertItem(klocale->translate("&Quit"), this, SLOT(quit()) );
	
	QPopupMenu *options = new QPopupMenu;
	options->insertItem(klocale->translate("&Keys"), this, SLOT(configKeys()) );
	
	QPopupMenu *help = new QPopupMenu;
	help->insertItem(klocale->translate("&Contents"), this, SLOT(help()) );
	help->insertSeparator();
	help->insertItem(klocale->translate("&About"), this, SLOT(about()) );
	help->insertItem(klocale->translate("About &Qt"), this, SLOT(aboutqt()) );
	
	menu = new KMenuBar(this);
	menu->enableMoving(TRUE);
	menu->enableFloating(TRUE);
	menu->insertItem(klocale->translate("&File"), popup);
	menu->insertItem(klocale->translate("&Options"), options);
	menu->insertSeparator();
	menu->insertItem(klocale->translate("&Help"), help);
	setMenu(menu);
	
	/* read the menu visible/invisible config */
	kconf = kapp->getConfig();
	kconf->setGroup("");
	if ( !kconf->hasKey(OP_MENUBAR_VIS) )
		kconf->writeEntry(OP_MENUBAR_VIS, 1);
	if ( kconf->readNumEntry(OP_MENUBAR_VIS)!=1 ) menu->show();
	else menu->hide();
	toggleMenu();
}

void KTetris::quit()
{
	kconf->setGroup("");
	kconf->writeEntry(OP_MENUBAR_VIS, menu->isVisible());
	kapp->quit();
}

void KTetris::toggleMenu()
{
	if ( menu->isVisible() ) {
		popup->changeItem(klocale->translate("Show menu"), tog_id);
		menu->hide();
	} else {
		popup->changeItem(klocale->translate("Hide menu"), tog_id);
		menu->show();
	}
	
	updateRects();
}

void KTetris::about()
{
	QString str;
	str.sprintf(klocale->translate("%s (%s) \n\nby Nicolas HADACEK\
(hadacek@kde.org)\ntetris kernel by Eirik ENG.\n\
http://www.via.ecp.fr/~hadacek/KDE/ktetris.html"), SNAME, SDATE);
	KMsgBox::message(0, klocale->translate("ktetris : about"),
					 (const char *)str, KMsgBox::INFORMATION, klocale->translate("Close"));
}

void KTetris::aboutqt()
{
	QMessageBox::aboutQt(this);
}

void KTetris::help()
{
	kapp->invokeHTMLHelp("", "");
}

void KTetris::configKeys()
{
	kKeys->configureKeys(this);
}

/* MAIN */
int main( int argc, char **argv )
{
	KApplication a(argc, argv, NAME);
	KTetris *te = new KTetris();
	a.setMainWidget(te);
	te->setCaption(SNAME);
	te->show();
	return a.exec();
}
