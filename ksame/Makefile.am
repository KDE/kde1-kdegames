bin_PROGRAMS = ksame
INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
ksame_LDADD =       $(LIB_KDEUI) 

ksame_SOURCES= main.cpp StoneWidget.cpp KSameWidget.cpp HighScore.cpp
BUILTSOURCES = StoneWidget.moc KSameWidget.moc HighScore.moc
DISTCLEANFILES = $(BUILTSOURCES)

noinst_HEADERS = StoneWidget.h KSameWidget.h HighScore.h version.h

docsdir = $(kde_htmldir)/en/ksame/
appsdir = $(kde_appsdir)/Games
picsdir = $(kde_datadir)/ksame/pics

StoneWidget.cpp: StoneWidget.moc
StoneWidget.moc: StoneWidget.h
	$(MOC) -o StoneWidget.moc StoneWidget.h

KSameWidget.cpp: KSameWidget.moc
KSameWidget.moc: KSameWidget.h
	$(MOC) -o KSameWidget.moc KSameWidget.h

HighScore.cpp: HighScore.moc
HighScore.moc: HighScore.h
	$(MOC) -o HighScore.moc HighScore.h

pics_DATA = stones.gif stones_mask.gif
docs_DATA = index.html index-1.html index-2.html index-3.html index-4.html logotp3.gif
apps_DATA = ksame.kdelnk

EXTRA_DIST = $(pics_DATA) $(docs_DATA) $(apps_DATA) ksame.xpm mini-ksame.xpm

install-data-local: 
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) ksame.xpm $(kde_icondir)/ksame.xpm
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-ksame.xpm $(kde_minidir)/ksame.xpm

uninstall-local:
	rm -f $(kde_icondir)/ksame.xpm
	rm -f $(kde_minidir)/ksame.xpm

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(ksame_SOURCES) && mv messages.po ../po/ksame.pot

