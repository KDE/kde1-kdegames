/* 
 * patience -- main program
 *   Copyright (C) 1995  Paul Olav Tvete
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 *
 *
 * Heavily modified by Mario Weilguni <mweilguni@sime.com>
 *
 */

#include "pwidget.h"
#include "version.h"
#include "global.h"
#include <qregexp.h>
#include "basiccard.h"
#include <qobjcoll.h>

// menu constants
// file menu
#define ID_FQUIT	101

// game menu
#define ID_GUNDO	201
#define ID_GRESTART	202
#define ID_GNEWTYPE	203
#define ID_GNKLONDIKE	(ID_GNEWTYPE + PT_KLONDIKE)
#define ID_GNMSOLITAIRE	(ID_GNEWTYPE + PT_MSOLITAIRE)
#define ID_GNIDIOT	(ID_GNEWTYPE + PT_IDIOT)
#define ID_GNGRANDFATHER	(ID_GNEWTYPE + PT_GRANDFATHER)
#define ID_GNNAPOLEON	(ID_GNEWTYPE + PT_NAPOLEON)
#define ID_GNTEN	(ID_GNEWTYPE + PT_TEN)
#define ID_GNCOMPUTATION	(ID_GNEWTYPE + PT_COMPUTATION)

// option menu
#define ID_OBACK	801
#define ID_OAFTERBACK	820
#define ID_OANIMATION	821

// help menu
#define ID_HHELP	901
#define ID_HHELPRULES	902
#define ID_HABOUT	903

pWidget::pWidget(QWidget *, const char *) 
  : KFixedTopWidget()
{
  type = PT_NONE;
  dill = 0;
  
  m = new KMenuBar( this );
  connect(m, SIGNAL(activated(int)),
	  this, SLOT(action(int)));
#define menuPosition
  connect(m, SIGNAL(moved(menuPosition)),
	  this, SLOT(slotMenubarChanged()));
#undef menuPosition

  // start default game
  actionNewGame(getDefaultType() + ID_GNEWTYPE);

  QPopupMenu *p1 = new QPopupMenu;
  p1->insertItem(locale->translate("&Quit"), ID_FQUIT);

  m->insertItem(locale->translate("&File"), p1 );

  QPopupMenu *p2 = new QPopupMenu;
  m->insertItem(locale->translate("&Game"), p2 );

  p2->insertItem(locale->translate("&Undo"), ID_GUNDO);
  p2->insertItem(locale->translate("&Restart game"), ID_GRESTART);

  QPopupMenu *popup = new QPopupMenu;

  popup->insertItem(locale->translate("&Klondike"), ID_GNKLONDIKE);
  popup->insertItem(locale->translate("MicroSolitaire"), ID_GNMSOLITAIRE);
  popup->insertItem(locale->translate("Calculation"), ID_GNCOMPUTATION);
  popup->insertItem(locale->translate("Napoleon's Tomb"), ID_GNNAPOLEON);
  popup->insertItem(locale->translate("The Idiot"), ID_GNIDIOT);
  popup->insertItem(locale->translate("Grandfather"), ID_GNGRANDFATHER);
  popup->insertItem(locale->translate("Ten"), ID_GNTEN);

  p2->insertItem(locale->translate("Choose new game"), popup);

  // option menu
  QPopupMenu *m_opt = new QPopupMenu();
  m_opt->setCheckable(TRUE);
  QPopupMenu *m_opt_bg = new QPopupMenu();
  m_opt_bg->insertItem(locale->translate("KDE"), ID_OBACK);
  m_opt_bg->insertItem(locale->translate("Classic blue"), ID_OBACK + 1);
  m_opt_bg->insertItem(locale->translate("Classic red"), ID_OBACK + 2);
  m_opt_bg->insertItem(locale->translate("Technical"), ID_OBACK+3);
  m_opt->insertItem(locale->translate("Card backside"), m_opt_bg);
  m_opt->insertItem(locale->translate("Animation on startup"), ID_OANIMATION);
  m->insertItem(locale->translate("&Options"), m_opt);

  m->insertSeparator();
  QPopupMenu *m_help = new QPopupMenu;
  m_help->insertItem(locale->translate("Help..."), ID_HHELP);
  m_help->insertItem(locale->translate("Help on rules..."), ID_HHELPRULES);
  m_help->insertSeparator();
  m_help->insertItem(locale->translate("About..."), ID_HABOUT);
  m->insertItem(locale->translate("&Help"), m_help);

  // create the toolbar
  tb = new KToolBar(this);
#define BarPosition
  connect(tb, SIGNAL(moved(BarPosition)),
	  this, SLOT(slotToolbarChanged()));
#undef BarPosition

  QPixmap *pm1 = new QPixmap((KDEPICDIR + "exit.xpm").data());
  QPixmap *pm2 = new QPixmap((KDEPICDIR + "help.xpm").data());
  tb->insertButton(*pm1, ID_FQUIT, -1, locale->translate("Quit"));
  tb->insertButton(*pm2, ID_HHELP, -1, locale->translate("Help"));
  connect(tb, SIGNAL(clicked(int)),
	  this, SLOT(action(int)));

  // setup accelerators
  m->setAccel(CTRL+Key_K, ID_GNKLONDIKE);
  m->setAccel(CTRL+Key_M, ID_GNMSOLITAIRE);
  m->setAccel(CTRL+Key_C, ID_GNCOMPUTATION);
  m->setAccel(CTRL+Key_N, ID_GNNAPOLEON);
  m->setAccel(CTRL+Key_I, ID_GNIDIOT);
  m->setAccel(CTRL+Key_G, ID_GNGRANDFATHER);
  m->setAccel(CTRL+Key_T, ID_GNTEN);
  m->setAccel(CTRL+Key_Q, ID_FQUIT);
  m->setAccel(Key_F1, ID_HHELP);
  m->setAccel(CTRL+Key_Z, ID_GUNDO);

  if(config) {
    config->setGroup(QString("General Settings"));
    int bg = config->readNumEntry(QString("Backside"), 0);
    setBackSide(bg);

    // move the toolbar to it's default position
    config->setGroup(QString("General Settings"));
    int tbpos = config->readNumEntry(QString("Toolbar_1_Position"), 
				     (int)(KToolBar::Left));
    tb->setBarPos((KToolBar::BarPosition)(tbpos));

    // move the menubar to it's default position
    int mbpos = config->readNumEntry(QString("Menubar_Position"), 
				     (int)(KMenuBar::Top));
    m->setMenuBarPos((KMenuBar::menuPosition)(mbpos));

    bool animate = \
      (bool)(config->readNumEntry(QString("Animation"), 1) != 0);
    m->setItemChecked(ID_OANIMATION, animate);
  }

  m->show();
  dill->show();
  tb->show();  
  setView(dill);
  setMenu(m);
  addToolBar(tb);
  updateRects();
}

void pWidget::action(int id) {
  bool animate;

  switch(id) {
  case ID_FQUIT:
    kapp->quit();
    break;

  case ID_GUNDO:
    if(dill)
      dill->undo();
    break;

  case ID_GRESTART:
    if(dill)
      dill->restart();
    break;

  case ID_GNEWTYPE:
  case ID_GNKLONDIKE:
  case ID_GNMSOLITAIRE:
  case ID_GNIDIOT:
  case ID_GNGRANDFATHER:
  case ID_GNNAPOLEON:
  case ID_GNTEN:
  case ID_GNCOMPUTATION:
    actionNewGame(id);
    break;

  case ID_OANIMATION:
    animate = !(bool)(config->readNumEntry(QString("Animation"), 1) != 0);    
    config->setGroup(QString("General Settings"));
    config->writeEntry(QString("Animation"), (int)animate);
    m->setItemChecked(ID_OANIMATION, animate);
    break;

  case ID_HHELP:
    help();
    break;

  case ID_HHELPRULES:
    helpRules();
    break;

  case ID_HABOUT:
    about();
    break;
    
  default:
    // background stuff?
    if(id >= ID_OBACK && id < ID_OAFTERBACK) {
      setBackSide(id - ID_OBACK);
    } else
      fprintf(stderr, 
	      locale->translate("COMMAND %d not implemented!\n"), id);
  }
}

void pWidget::actionNewGame(int id) {
  if(dill != 0) {
    delete dill;
    dill = 0;
  }

  type = id - ID_GNEWTYPE;
  switch(id) {
  case ID_GNTEN:
    dill = new Ten(this, locale->translate("Ten"));
    break;

  case ID_GNGRANDFATHER:
    dill = new Grandf(this, locale->translate("Grandfather"));
    break;

  case ID_GNKLONDIKE:
    dill = new Klondike(this, locale->translate("Klondike"));
    break;

  case ID_GNMSOLITAIRE:
    dill = new MicroSolitaire(this, locale->translate("MicroSolitaire"));
    break;

  case ID_GNCOMPUTATION:
    dill = new Computation(this, locale->translate("Calculation"));
    break;

  case ID_GNIDIOT:
    dill = new Idiot(this, locale->translate("The Idiot"));
    break;

  case ID_GNNAPOLEON:
    dill = new Napoleon(this, locale->translate("Napoleons Tomb"));
    break;

  default:
    fprintf(stderr, 
	    locale->translate("kpat: unimplemented game type %d\n"), 
	    type);
    kapp->quit();
    break;
  }

  dill->setFixedSize(dill->sizeHint());
  dill->show();
  setView(dill);
  updateRects();

  QString title = "kpat: \"" + QString(dill->name()) + "\"";
  setCaption(title.data());

  setDefaultType();  
}

void pWidget::helpRules() {
  if (dill) {
    QString topic = dill->name();
    topic = topic.replace(QRegExp(" "), "_");
    topic = topic.lower();
    kapp->invokeHTMLHelp("", topic.data());
  } else
    kapp->invokeHTMLHelp("", "");
}

void pWidget::help() {
  kapp->invokeHTMLHelp("", "");
}

void pWidget::about() {
  QString s = locale->translate("KPat\n\nVersion ");
  s += KPAT_VERSION;
  s += locale->translate("\n" \
    "Copyright (C) 1995  Paul Olav Tvete\n\n"\
    "Additional work done by:\t\t \n"\
    "\tMatthias Ettrich\t<ettrich@kde.org>\n"\
    "\tMario Weilguni\t<mweilguni@sime.com>\n\n"\
    "This program is part of the K Desktop Environment.\n"\
    "Use help for license information.");
  KMsgBox::message(this, 
		   locale->translate("About kpat"),
		   s.data());
}

void pWidget::setDefaultType() {
  if(type != PT_NONE) {
    if(config != 0) {
      config->setGroup(QString("General Settings"));
      config->writeEntry(QString("DefaultGame"), type);
    }
  }
}

int pWidget::getDefaultType() {
  if(config == 0)
    return PT_KLONDIKE;
  else {
    config->setGroup(QString("General Settings"));
    return config->readNumEntry(QString("DefaultGame"), PT_KLONDIKE);
  }
}

void pWidget::setBackSide(int id) {
  if(cardmaps != 0) {
    if(id == 0) {
      cardmaps->setBackSide(0);
      config->setGroup(QString("General Settings"));
	config->writeEntry(QString("Backside"), 0);
    } else {
      QString fname;
      fname.sprintf("%s/back%d.bmp", PICDIR.data(), id);
      QPixmap *pm = new QPixmap(fname.data());
      if(pm->width() > 0 && pm->height() > 0) {
	cardmaps->setBackSide(pm);
	config->setGroup(QString("General Settings"));
	config->writeEntry(QString("Backside"), id);
      } else
	KMsgBox::message(this, locale->translate("Error"),
			 locale->translate("Could not load background image!"),
			 KMsgBox::STOP);
    }
  }

  // this hack is needed, update() is not enough
  QObjectList *ol = queryList("basicCard");
  QObjectListIt it( *ol );
  while(it.current()) {
    QWidget *w = (QWidget *)it.current();
    ++it;
    w->repaint(TRUE);
  }
  delete ol;
}

void pWidget::slotToolbarChanged() {
  int p = (int)tb->barPos();
  config->setGroup(QString("General Settings"));
  config->writeEntry(QString("Toolbar_1_Position"), p);
}

void pWidget::slotMenubarChanged() {
  int p = (int)m->menuBarPos();
  config->setGroup(QString("General Settings"));
  config->writeEntry(QString("Menubar_Position"), p);
}

#include "pwidget.moc"