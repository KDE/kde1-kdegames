bin_PROGRAMS = ksame
INCLUDES = $(all_includes)
ksame_LDADD = $(all_libraries) -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET) $(KDE_RPATH)

ksame_SOURCES= main.cpp StoneWidget.cpp KSameWidget.cpp ScoreWidget.cpp
BUILTSOURCES = StoneWidget.moc KSameWidget.moc ScoreWidget.moc
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

pics_DATA = stones.bmp stones_mask.bmp stones2000_mask.bmp stones2000.bmp 
docs_DATA = index.html
apps_DATA = ksame.kdelnk

