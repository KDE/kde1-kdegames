SUBDIRS = data doc
EXTRA_DIST = COPYRIGHT
AUTOMAKE_OPTIONS = foreign 

APPSDIR = $(kde_appsdir)/Games
INCLUDES = $(all_includes)

bin_PROGRAMS = 	ksmiletris

ksmiletris_SOURCES = main.cpp gamewindow.cpp gamewidget.cpp \
		     screenwidget.cpp mirrorwidget.cpp npiecewidget.cpp
ksmiletris_METASOURCES = USE_AUTOMOC

ksmiletris_LDFLAGS = $(all_libraries) $(KDE_RPATH)
ksmiletris_LDADD   = -lkfm -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)

noinst_HEADERS = ksmiletris.h gamewindow.h gamewidget.h \
		 screenwidget.h mirrorwidget.h npiecewidget.h

DISTCLEANFILES = $(ksmiletris_METASOURCES)

install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) ksmiletris.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) ksmiletris.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-ksmiletris.xpm $(kde_minidir)/ksmiletris.xpm

uninstall-local:
	-rm -f $(APPSDIR)/ksmiletris.kdelnk
	-rm -f $(kde_icondir)/ksmiletris.xpm
	-rm -f $(kde_minidir)/ksmiletris.xpm
