bin_PROGRAMS = ksame
INCLUDES = $(all_includes)
ksame_LDADD = $(all_libraries) -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET) $(KDE_RPATH)

ksame_SOURCES= main.cpp StoneWidget.cpp KSameWidget.cpp ScoreWidget.cpp
BUILTSOURCES = StoneWidget.moc KSameWidget.moc ScoreWidget.moc
DISTCLEANFILES = $(BUILTSOURCES)

DOCDIR = $(kde_htmldir)/en/ksame/
APPSDIR= $(kde_appsdir)/Games
PICSDIR= $(kde_datadir)/ksame/pics

StoneWidget.cpp: StoneWidget.moc
StoneWidget.moc: StoneWidget.h
	$(MOC) -o StoneWidget.moc StoneWidget.h

KSameWidget.cpp: KSameWidget.moc
KSameWidget.moc: KSameWidget.h
	$(MOC) -o KSameWidget.moc KSameWidget.h

ScoreWidget.cpp: ScoreWidget.moc
ScoreWidget.moc: ScoreWidget.h
	$(MOC) -o ScoreWidget.moc ScoreWidget.h

install-data-local: 
	$(mkinstalldirs) $(DOCDIR)
	$(INSTALL) index.html $(DOCDIR)
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL) ksame.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(PICSDIR)
	$(INSTALL) stones.bmp $(PICSDIR)
	$(INSTALL) stones_mask.bmp $(PICSDIR)

uninstall-local:
	rm -f $(DOCDIR)/index.html
	rm -f $(APPSDIR)/ksame.kdelnk
	rm -f $(PICSDIR)/stones.bmp $(PICSDIR)/stones_mask.bmp


