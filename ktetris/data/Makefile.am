## ##########################
## # Installation definitions

icondir = $(kde_icondir)
icon_DATA = ktetris.xpm
minidir = $(kde_minidir)
appsdir = $(kde_appsdir)/Games
apps_DATA = ktetris.kdelnk
htmldir = $(kde_htmldir)/en/ktetris
html_DATA = index.html index-1.html index-2.html index-3.html index-4.html \
            logotp3.gif

EXTRA_DIST = $(icon_DATA) $(apps_DATA) $(html_DATA) mini-ktetris.xpm

install-data-local:
	$(mkinstalldirs) $(minidir)
	$(INSTALL_DATA) mini-ktetris.xpm $(minidir)/ktetris.xpm

uninstall-local:
	rm -f $(minidir)/ktetris.xpm
	rm -rf $(htmldir)
