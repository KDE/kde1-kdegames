# Generated automatically from Makefile.in by configure.
HELPFILES = ksnake.html snakeRace.gif

DOCSDIR = /home/coolo/prod/kde/share/doc/HTML/ksnake
INSTALL_DIR = /usr/bin/install -c -d -m 0755
INSTALL_DATA = /usr/bin/install -c -m 644

all:

Makefile: Makefile.in
	cd .. && CONFIG_FILES=ksnake/doc/$@ CONFIG_HEADERS= $(SHELL) ./config.status

install:
	$(INSTALL_DIR) $(DOCSDIR)
	@for i in $(HELPFILES); do \
	 echo "$(INSTALL_DATA) $$i $(DOCSDIR)"; \
	 $(INSTALL_DATA) "$$i" $(DOCSDIR); \
	done

uninstall:
	for i in $(HELPFILES); do \
	 rm -f $(DOCSDIR)/"$$i"; \
	done
