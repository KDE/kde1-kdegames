## ##########################
## # Installation definitions

kde_icon_DATA = ksirtet.xpm
minidir = $(kde_minidir)
appsdir = $(kde_appsdir)/Games
apps_DATA = ksirtet.kdelnk
htmldir = $(kde_htmldir)/en/ksirtet
html_DATA = index.html index-1.html index-2.html index-3.html index-4.html \
            logotp3.gif

EXTRA_DIST = $(kde_icon_DATA) $(apps_DATA) $(html_DATA) mini-ksirtet.xpm

install-data-local:
	$(mkinstalldirs) $(minidir)
	$(INSTALL_DATA) $(srcdir)/mini-ksirtet.xpm $(minidir)/ksirtet.xpm

uninstall-local:
	rm -f $(minidir)/ksirtet.xpm
	rm -rf $(htmldir)
