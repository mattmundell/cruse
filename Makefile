include Makefile.config

VPATH = $(CURDIR)/src $(CURDIR)/include

.PHONY: cruse test clean archive

cruse:
	$(MAKE) -C src all

test:
	$(MAKE) -C test test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(MAKE) -C python clean

archive:
	$(MAKE) -C src clean
	export DATE=`date +%d_%b_%Y-%Hh%M | tr A-Z a-z | tr -d '\n'` \
	&& tar jcvf cruse-`cat etc/VERSION | tr -d '\n'`-$$DATE.tar.bz2 \
	--exclude=\*~ --exclude=\*~ --exclude=\*.o --exclude=curse \
	--exclude=GTK\* --exclude=gdit\* --exclude=old --exclude=\*scratch \
	--exclude=\*.tar.bz2 --exclude=model-autodia.xml --exclude=.cccc \
	--exclude=\*.pyc --exclude=\*.77 --exclude=.xvpics --exclude=archive \
	../cruse \
	&& rm -f cruse.tar.bz2 \
	&& cp cruse-`cat etc/VERSION | tr -d '\n'`-$$DATE.tar.bz2 cruse.tar.bz2
