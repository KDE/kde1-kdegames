SUBDIRS= data doc wav
AUTOMAKE_OPTIONS = foreign 

APPSDIR = $(kde_appsdir)/Games
INCLUDES = $(all_includes)

bin_PROGRAMS = ksmiletris

ksmiletris_SOURCES = main.cpp gamewindow.cpp gamewidget.cpp \
		     screenwidget.cpp mirrorwidget.cpp npiecewidget.cpp \
		     scoredialog.cpp newscoredialog.cpp

ksmiletris_METASOURCES = USE_AUTOMOC

ksmiletris_LDFLAGS = $(all_libraries) $(KDE_RPATH)
ksmiletris_LDADD   =    -lmediatool     $(LIB_KFM) $(LIB_KDEUI)

noinst_HEADERS = ksmiletris.h gamewindow.h gamewidget.h \
		 screenwidget.h mirrorwidget.h npiecewidget.h \
		 scoredialog.h newscoredialog.h

DISTCLEANFILES = $(ksmiletris_METASOURCES)

EXTRA_DIST = ksmiletris.xpm mini-ksmiletris.xpm ksmiletris.kdelnk

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(ksmiletris_SOURCES) && mv messages.po ../po/ksmiletris.pot

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
