# Generated automatically from Makefile.in by configure.
SNAKEDIR = /home/kulow/prod/kde/lib/ksnake
LEVELDIR = /home/kulow/prod/kde/lib/ksnake/levels
PIXDIR = /home/kulow/prod/kde/lib/ksnake/pixmaps

INSTALL_DIR = /home/kulow/bin/install -c -d -m 0755
INSTALL_DATA = /home/kulow/bin/install -c -m 644
INSTALL_HIGH = /home/kulow/bin/install -c -m 666

HIGHSCORES = ./highScores
LEVELS = `find levels -name "room*" -print | sort`
PIXS = `find pixmaps -name "*.xpm" -print | sort`

all:

install:
	$(INSTALL_DIR) $(SNAKEDIR)
	$(INSTALL_HIGH) $(HIGHSCORES) $(SNAKEDIR);

	$(INSTALL_DIR) $(LEVELDIR)
		@for i in $(LEVELS); do \
		 echo "$(INSTALL_DATA) $$i $(LEVELDIR)"; \
		 $(INSTALL_DATA) "$$i" $(LEVELDIR); \
		done

	$(INSTALL_DIR) $(PIXDIR)
		@for i in $(PIXS); do \
		 echo "$(INSTALL_DATA) $$i $(PIXDIR)"; \
		 $(INSTALL_DATA) "$$i" $(PIXDIR); \
		done

uninstall:
