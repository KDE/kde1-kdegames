

INCLUDES = $(all_includes) 

LDFLAGS	= $(all_libraries) $(KDE_RPATH)
LDADD = -lmediatool -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET) $(LIBCOMPAT)

appsdir = $(kde_appsdir)/Games
apps_DATA = kpoker.kdelnk

icondir = $(kde_icondir)
icon_DATA = kpoker.xpm

bin_PROGRAMS = kpoker
kpoker_SOURCES = kpoker.cpp kpaint.cpp top.cpp main.cpp

noinst_HEADERS = defines.h global.h kpaint.h kpoker.h top.h version.h

SRCMETA =	kpoker.moc kpaint.moc top.moc
DISTCLEANFILES = $(SRCMETA)

SUBDIRS = docs sounds bitmaps

EXTRA_DIST = $(apps_DATA) $(icon_DATA) kpoker.xpm mini-kpoker.xpm

install-data-local:
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kpoker.xpm $(kde_minidir)/kpoker.xpm

uninstall:
	rm -f $(kde_minidir)/kpoker.xpm

messages:
	@XGETTEXT@ -C -ktranslate -x ../../kdelibs/po/kde.pot *.cpp && mv messages.po ../po/kpoker.pot 

kpoker.cpp:kpoker.moc
kpoker.moc: kpoker.h
	$(MOC) kpoker.h -o kpoker.moc

kpaint.cpp:kpaint.moc
kpaint.moc: kpaint.h
	$(MOC) kpaint.h -o kpaint.moc

top.cpp:top.moc
top.moc: top.h
	$(MOC) top.h -o top.moc

