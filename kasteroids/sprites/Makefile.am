
SUBDIRS = bits missile rock1 rock2 rock3 ship

SHELL = /bin/sh
POVRAY=x-povray  # Must be Povray 3.0
LDLIBS=-lpng -lz -lm

#all: pngtoppmpgm

#clean:
#	rm -f *~ pngtoppmpgm

#distclean: clean
#	rm -f Makefile

FILES = `find . \( -name "*.ppm" -o -name "*.pbm" \) -print`
prefix = @prefix@
kde_datadir = @kde_datadir@
install_root = @install_root@
DESTDIR = $(kde_datadir)/kasteroids/sprites
mkinstalldirs = $(SHELL) @top_srcdir@/mkinstalldirs

install-data-local:
	@$(mkinstalldirs) $(DESTDIR)/rock1 ;\
	$(mkinstalldirs) $(DESTDIR)/rock2 ;\
	$(mkinstalldirs) $(DESTDIR)/rock3 ;\
	$(mkinstalldirs) $(DESTDIR)/ship ;\
	$(mkinstalldirs) $(DESTDIR)/bits ;\
	$(mkinstalldirs) $(DESTDIR)/missile ;\
	for file in $(FILES); do \
	 echo "install $$file in "$(DESTDIR)/$$file ;\
	 $(INSTALL_DATA) $$file $(DESTDIR)/$$file ;\
	done 

uninstall-local:
	for file in $(FILES); do \
	 rm -f $(DESTDIR)/$$file ;\
	done
