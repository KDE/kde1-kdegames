# Generated automatically from Makefile.in by configure.
HELPFILES = ksnake.html snakeRace.gif

DOCSDIR = /home/kulow/prod/kde/doc/HTML/ksnake
INSTALL_DIR = /home/kulow/bin/install -c -d -m 0755
INSTALL_DATA = /home/kulow/bin/install -c -m 644

all:

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
