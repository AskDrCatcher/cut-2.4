# CUT 2.4
# Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006 Samuel A. Falvo II, et. al.
# See LICENSE for details.

CC		= gcc
SRCS	= cutgen cuttemplate

posix: $(SRCS:%=%.c)
	$(CC) -D__POSIX__ -o cutgen $(SRCS:%=%.c)

clean:
	rm -f *.o *~ cutgen

