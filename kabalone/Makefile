# Generated automatically from Makefile.in by configure.
# This is a sample Makefile.in 

# -g -Wall     : is set to -g -Wall (when debuging) or -O2 -Wall (when not)
# -I/home/kulow/prod/kde/include -I/home/kulow/prod/qt-1.2/include -I/usr/openwin/include : is set to all needed include paths for QT, KDE and X11
# -I.. 	         : needed for config.h
CXXFLAGS= -g -Wall -I/home/kulow/prod/kde/include -I/home/kulow/prod/qt-1.2/include -I/usr/openwin/include -DHAVE_CONFIG_H -I.. \
	-finline-functions

#     : is set to -s (when not debuging) for stripping
# @all_libra.@ : is set to all needed libraries paths for QT, KDE and X11
# you need also -lsocket -lnsl, if you link with -lkfm
LDFLAGS	=  -s -L/home/kulow/prod/kde/lib -L/home/kulow/prod/qt-1.2/lib  -lkdecore -lkdeui -lqt -lX11  \
	-lXext

# both are clear, I think
CXX     = c++
MOC   	= /home/kulow/prod/qt-1.2/bin/moc

############# INSTALL #################

install = /home/kulow/bin/install -c
INSTALL = $(install) -m 0755
INSTALL_DATA = $(install) -m 0644
INSTALL_DIR = $(install) -d -m 0755
INSTALL_DIR_PRIVATE = $(install) -d -m 0700

prefix = /home/kulow/prod/kde
KDEINC = $(prefix)/include
BINDIR = $(prefix)/bin
LIBDIR = $(prefix)/lib
DOCDIR = $(prefix)/share/doc/HTML/kabalone
KDELNKDIR= $(prefix)/share/applnk/Games

########### File Formats ###########

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<

%.moc: %.h
	$(MOC) $< -o $@

########## Files #####################

OBJECTS =       Board.o Move.o BoardWidget.o AbTop.o kabalone.o
SRCMETA =	Board.moc AbTop.moc BoardWidget.moc
DOCS    =       html/KAbalone.html
TARGET	=	kabalone

######### build rules ################

all: $(TARGET)

Makefile: Makefile.in
	cd .. && CONFIG_FILES=kabalone/$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(TARGET): $(SRCMETA) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f *.o *.bak *.moc *~ *% #*
	rm -f $(SRCMETA) $(TARGET)

distclean: clean
	rm -f Makefile

install: all
	$(INSTALL_DIR) $(BINDIR)
	$(INSTALL) $(TARGET) $(BINDIR)
	$(INSTALL_DIR) $(DOCDIR)
	$(INSTALL) $(DOCS) $(DOCDIR)
	$(INSTALL_DIR) $(KDELNKDIR)
	$(INSTALL_DATA) kabalone.kdelnk $(KDELNKDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(DOCDIR)/$(DOCS)

dep:
	makedepend -Y *.cpp

# DO NOT DELETE
AbTop.o: AbTop.h Board.h Move.h BoardWidget.h AbTop.moc
Board.o: Board.h Move.h Board.moc
BoardWidget.o: Board.h Move.h BoardWidget.h bitmaps/Arrow1 bitmaps/Arrow1Mask
BoardWidget.o: bitmaps/Arrow2 bitmaps/Arrow2Mask bitmaps/Arrow3
BoardWidget.o: bitmaps/Arrow3Mask bitmaps/Arrow4 bitmaps/Arrow4Mask
BoardWidget.o: bitmaps/Arrow5 bitmaps/Arrow5Mask bitmaps/Arrow6
BoardWidget.o: bitmaps/Arrow6Mask BoardWidget.moc
Move.o: Move.h
kabalone.o: AbTop.h Board.h Move.h BoardWidget.h
