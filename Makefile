# CUT 2.4
# Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006 Samuel A. Falvo II, et. al.
# See LICENSE for details.

.SUFFIXES:

INSTALLPATH := /usr/local
INCDIR := $(INSTALLPATH)/include/cut-2
BINDIR := $(INSTALLPATH)/bin

bin/cutgen:
	(cd src; make posix)
	mv src/cutgen bin

clean:
	(cd src; make clean)
	rm bin/*

install: bin/cutgen
	mkdir $(INCDIR)
	cp include/cut-2/cut.h $(INCDIR)
	cp bin/cutgen $(BINDIR)/cutgen-2

uninstall: deinstall

deinstall:
	rm -rf $(INCDIR)
	rm -f $(BINDIR)/cutgen-2
