
INCLUDES = $(all_includes)

LDFLAGS	= $(all_libraries) $(KDE_RPATH)
LDADD =  -lkdeui -lkdecore -lqt -lXext -lX11 -lm

noinst_HEADERS = game.h trys.h rattler.h board.h\
		level.h ball.h snake.h\
		basket.h startroom.h lcdrange.h score.h\
		pixServer.h progress.h levels.h view.h keys.h version.h \
		bitmaps.h

ksnake_SOURCES = game.cpp trys.cpp rattler.cpp board.cpp\
		level.cpp ball.cpp snake.cpp\
		basket.cpp startroom.cpp lcdrange.cpp\
		score.cpp pixServer.cpp progress.cpp levels.cpp\
		view.cpp keys.cpp

ksnake_METASOURCES = USE_AUTOMOC

DISTCLEANFILES = $(ksnake_METASOURCES)

bin_PROGRAMS = 	ksnake

######### build rules ################

apps_DATA = ksnake.kdelnk
appsdir = $(kde_appsdir)/Games

icon_DATA = ksnake.xpm
icondir = $(kde_icondir)

EXTRA_DIST = $(apps_DATA) $(icon_DATA) mini-ksnake.xpm

SUBDIRS = doc data 

install-data-local: all
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-ksnake.xpm $(kde_minidir)/ksnake.xpm

uninstall-local:
	-rm -f $(kde_minidir)/ksnake.xpm

messages:
	$(XGETTEXT) -C -ki18n -ktranslate -x../../kdelibs/po/kde.pot *.cpp && mv messages.po ../po/ksnake.pot

# DO NOT DELETE THIS LINE -- make depend depends on it.
