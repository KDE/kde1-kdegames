
INCLUDES = $(all_includes) -IQwSpriteField

LDFLAGS = $(all_libraries) 
LDADD = -lkdeui -lkdecore QwSpriteField/libSpriteField.a -lqt -lXext -lX11 $(LIBSOCKET)

kasteroids_SOURCES = main.cpp view.cpp toplevel.cpp kfixedtopwidget.cpp kastmsg.cpp
BUILTSOURCES = view.moc toplevel.moc kfixedtopwidget.moc
bin_PROGRAMS = kasteroids

SUBDIRS = QwSpriteField html

picsdir = $(kde_datadir)/kasteroids/sprites
pics_DATA = bg.ppm

appsdir = $(kde_appsdir)/Games
apps_DATA = Kasteroids.kdelnk

icondir = $(kde_icondir)
icon_DATA = kasteroids.xpm

install-data-local:
	(cd sprites && $(MAKE) install)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-kasteroids.xpm $(kde_minidir)/kasteroids.xpm
 
uninstall-local:
	(cd sprites $$ $(MAKE) uninstall)
	-rm -f $(kde_minidir)/kasteroids.xpm

messages:
	@XGETTEXT@ -C -ktranslate -x ../../kdelibs/po/kde.pot *.cpp && mv messages.po ../po/kasteroids.pot

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

