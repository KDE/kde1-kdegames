bin_PROGRAMS = ksame
INCLUDES = $(all_includes)
ksame_LDADD = $(all_libraries) -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET) $(KDE_RPATH)

ksame_SOURCES= main.cpp StoneWidget.cpp KSameWidget.cpp ScoreWidget.cpp HighScore.cpp
BUILTSOURCES = StoneWidget.moc KSameWidget.moc ScoreWidget.moc HighScore.moc
DISTCLEANFILES = $(BUILTSOURCES)

docsdir = $(kde_htmldir)/en/ksame/
appsdir = $(kde_appsdir)/Games
picsdir = $(kde_datadir)/ksame/pics

StoneWidget.cpp: StoneWidget.moc
StoneWidget.moc: StoneWidget.h
	$(MOC) -o StoneWidget.moc StoneWidget.h

KSameWidget.cpp: KSameWidget.moc
KSameWidget.moc: KSameWidget.h
	$(MOC) -o KSameWidget.moc KSameWidget.h

ScoreWidget.cpp: ScoreWidget.moc
ScoreWidget.moc: ScoreWidget.h
	$(MOC) -o ScoreWidget.moc ScoreWidget.h

HighScore.cpp: HighScore.moc
HighScore.moc: HighScore.h
	$(MOC) -o HighScore.moc HighScore.h

pics_DATA = stones.bmp stones_mask.bmp stones2000_mask.bmp stones2000.bmp 
docs_DATA = index.html index-1.html index-2.html index-3.html index-4.html logotp3.gif
apps_DATA = ksame.kdelnk

install-data-local: 
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) ksame.xpm $(kde_icondir)/ksame.xpm
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-ksame.xpm $(kde_minidir)/ksame.xpm

uninstall-local:
	rm -f $(kde_icondir)/ksame.xpm
	rm -f $(kde_minidir)/ksame.xpm

