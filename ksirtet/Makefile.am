## ############################################################################
## This Makefile.am is intended to be generic for standard (small) applications
## contact hadacek@kde.org for comments/suggestions

## ######################
## # Building definitions

bin_PROGRAMS = ktetris
INCLUDES = $(all_includes)
ktetris_LDFLAGS = $(all_libraries) $(KDE_RPATH)
ktetris_LDADD = -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)

ktetris_SOURCES= gtetris.cpp tpiece.cpp spiece.cpp gtboard.cpp board.cpp \
				hack.cpp net.cpp dialogs.cpp field.cpp main.cpp 
BUILTSOURCES = spiece.moc board.moc dialogs.moc field.moc main.moc
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ktranslate -x ../../kdelibs/po/kde.pot $(ktetris_SOURCES) && mv messages.po ../po/ktetris.pot

## ##########################
## # Installation definitions

icondir = $(kde_icondir)
icon_DATA = ktetris.xpm
minidir = $(kde_minidir)
appsdir = $(kde_appsdir)/Games
apps_DATA = ktetris.kdelnk
htmldir = $(kde_htmldir)/en/ktetris
html_DATA = index.html

install-data-local:
	$(mkinstalldirs) $(minidir)
	$(INSTALL_DATA) mini-ktetris.xpm $(minidir)/ktetris.xpm

uninstall-local:
	rm -f $(minidir)/ktetris.xpm
	rm -rf $(htmldir)

## ##############
## # MOC building

board.cpp: board.moc
board.moc: board.h
	$(MOC) -o board.moc board.h

dialogs.cpp: dialogs.moc
dialogs.moc: dialogs.h
	$(MOC) -o dialogs.moc dialogs.h

main.cpp: main.moc
main.moc: main.h
	$(MOC) -o main.moc main.h

spiece.cpp: spiece.moc
spiece.moc: spiece.h
	$(MOC) -o spiece.moc spiece.h

field.cpp: field.moc
field.moc: field.h
	$(MOC) -o field.moc field.h
