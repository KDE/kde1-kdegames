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
				net.cpp dialogs.cpp field.cpp main.cpp dbutton.cpp
BUILTSOURCES = spiece.moc board.moc dialogs.moc field.moc main.moc
DISTCLEANFILES = $(BUILTSOURCES)

noinst_HEADERS = board.h ggboard.h spiece.h dbutton.h gtboard.h tpiece.h \
				defines.h gtetris.h version.h dialogs.h main.h field.h net.h

messages:
	$(XGETTEXT) -C -ktranslate -ki18n -x $(includedir)/kde.pot $(ktetris_SOURCES) && mv messages.po ../po/ktetris.pot

SUBDIRS = data

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

ksock2.cpp: ksock2.moc
ksock2.moc: ksock2.h
	$(MOC) -o ksock2.moc ksock2.h
