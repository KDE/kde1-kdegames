
SUBDIRS = cs de es fi fr hr hu it no pl pt ro ru sk nl el \
	zh_CN.GB2312 zh_TW.Big5 mk pt_BR eo sv is ko he

# this is a general list for merging!
APPS = kabalone kasteroids kmahjongg kmines kpat kpoker kreversi ksame \
	kshisen ksnake ktetris kblackbox ksokoban ksmiletris konquest

merge:
	@apps='$(APPS)'; \
	langs='$(SUBDIRS)'; \
	for cat in $$apps; do \
	  echo $$cat; \
	  for lang in $$langs; do \
	    echo $$lang/$$cat ; \
	    if test -r $$lang/$$cat.po; then \
	      echo $$lang; \
	      msgmerge -o $$cat.new $$lang/$$cat.po $$cat.pot; \
	      if test -s $$cat.new && diff $$lang/$$cat.po $$cat.new; then \
	        rm $$cat.new;  \
 	      else  \
		mv $$cat.new $$lang/$$cat.po ; \
	    fi; \
	  fi ;\
         done ;\
	done

messages:
