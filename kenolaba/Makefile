# Generated automatically from Makefile.in by configure.
# This is a sample Makefile.in 

# -g -Wall     : is set to -g -Wall (when debuging) or -O2 -Wall (when not)
# -I/usr/local/KDE/include -I/usr/local/qt/include -I/usr/X11R6/include : is set to all needed include paths for QT, KDE and X11
# -I.. 	         : needed for config.h
CXXFLAGS= -g -Wall -I/usr/local/KDE/include -I/usr/local/qt/include -I/usr/X11R6/include -DHAVE_CONFIG_H -I.. \
	-finline-functions

#     : is set to -s (when not debuging) for stripping
# @all_libra.@ : is set to all needed libraries paths for QT, KDE and X11
# you need also , if you link with -lkfm
LDFLAGS	=  -s -L/usr/local/KDE/lib -L/usr/local/qt/lib -L/usr/X11R6/lib -lkdecore -lkdeui -lqt -lX11  \
	-lXext

# both are clear, I think
CXX     = c++
MOC   	= /usr/local/bin/moc

############# INSTALL #################

install = /usr/bin/ginstall -c
INSTALL = $(install) -m 0755
INSTALL_DATA = $(install) -m 0644
INSTALL_DIR = $(install) -d -m 0755
INSTALL_DIR_PRIVATE = $(install) -d -m 0700

prefix = /usr/local/KDE
KDEINC = $(prefix)/include
BINDIR = $(prefix)/bin
LIBDIR = $(prefix)/lib
DOCDIR = $(prefix)/share/doc/HTML
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
DOCS    =       html/kabalone.html
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
	$(INSTALL_DATA) $(DOCS) $(DOCDIR)
	$(INSTALL_DIR) $(KDELNKDIR)
	$(INSTALL_DATA) kabalone.kdelnk $(KDELNKDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(DOCDIR)/$(DOCS)

dep:
	makedepend -Y *.cpp

$(TARGET).po:
	xgettext --default-domain=$(TARGET) --keyword=translate *.cpp


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
