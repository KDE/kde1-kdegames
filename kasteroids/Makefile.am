
INCLUDES = $(all_includes)

LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD = -lkdeui -lkdecore -lQwSpriteField -lmediatool -lqt -lXext -lX11 $(LIBSOCKET) $(LIBCOMPAT)

kasteroids_SOURCES = main.cpp view.cpp toplevel.cpp kfixedtopwidget.cpp
BUILTSOURCES = view.moc toplevel.moc kfixedtopwidget.moc
DISTCLEANFILES = $(BUILTSOURCES)

noinst_HEADERS = kfixedtopwidget.h sprites.h toplevel.h version.h view.h

EXTRA_DIST = mini-kasteroids.xpm kasteroids.xpm  kasteroids.kdelnk bg.ppm

bin_PROGRAMS = kasteroids

SUBDIRS = html sprites

picsdir = $(kde_datadir)/kasteroids/sprites
pics_DATA = bg.ppm

appsdir = $(kde_appsdir)/Games
apps_DATA = kasteroids.kdelnk

icondir = $(kde_icondir)
icon_DATA = kasteroids.xpm

install-data-local:
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kasteroids.xpm $(kde_minidir)/kasteroids.xpm
 
uninstall-local:
	-rm -f $(kde_minidir)/kasteroids.xpm

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(kasteroids_SOURCES) && mv messages.po ../po/kasteroids.pot

# Dependencies
view.cpp:view.moc
view.moc: view.h
	$(MOC) view.h -o view.moc

toplevel.cpp:toplevel.moc
toplevel.moc: toplevel.h
	$(MOC) toplevel.h -o toplevel.moc

kfixedtopwidget.cpp:kfixedtopwidget.moc
kfixedtopwidget.moc: kfixedtopwidget.h
	$(MOC) kfixedtopwidget.h -o kfixedtopwidget.moc

