
INCLUDES = $(all_includes)

LDFLAGS	= $(all_libraries) $(KDE_RPATH)
LDADD =  -lkdeui -lkdecore -lqt -lXext -lX11 -lm

noinst_HEADERS = game.h trys.h rattler.h board.h\
		level.h ball.h snake.h\
		basket.h startroom.h lcdrange.h score.h\
		pixServer.h progress.h levels.h view.o keys.h \
		$(BUILTSOURCES)

ksnake_SOURCES = game.cpp trys.cpp rattler.cpp board.cpp\
		level.cpp ball.cpp snake.cpp\
		basket.cpp startroom.cpp lcdrange.cpp\
		score.cpp pixServer.cpp progress.cpp levels.cpp\
		view.cpp keys.cpp

BUILTSOURCES = 	mgame.cpp mrattler.cpp msnake.cpp  mtrys.cpp\
		mbasket.cpp mstartroom.cpp mlcdrange.cpp mscore.cpp\
	        mprogress.cpp mview.cpp mkeys.cpp

DISTCLEANFILES = $(BUILTSOUCES)

bin_PROGRAMS = 	ksnake

######### build rules ################

apps_DATA = ksnake.kdelnk
appsdir = $(kde_appsdir)/Games

icon_DATA = ksnake.xpm
icondir = $(kde_icondir)

SUBDIRS = doc data 

install: all
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-ksnake.xpm $(kde_minidir)/ksnake.xpm

uninstall:
	-rm -f $(kde_minidir)/ksnake.xpm

####### Meta classes

mrattler.cpp: rattler.h
	$(MOC) -o $@ $<

msnake.cpp: snake.h
	$(MOC) -o $@ $<

mgame.cpp: game.h
	$(MOC) -o $@ $<

mtrys.cpp: trys.h
	$(MOC) -o $@ $<

mbasket.cpp: basket.h
	$(MOC) -o $@ $<

mstartroom.cpp: startroom.h
	$(MOC) -o $@ $<

mlcdrange.cpp: lcdrange.h
	$(MOC) -o $@ $<

mscore.cpp: score.h
	$(MOC) -o $@ $<

mbox.cpp: box.h
	$(MOC) -o $@ $<

mprogress.cpp: progress.h
	$(MOC) -o $@ $<

mview.cpp: view.h
	$(MOC) -o $@ $<

mkeys.cpp: keys.h
	$(MOC) -o $@ $<

# DO NOT DELETE THIS LINE -- make depend depends on it.
