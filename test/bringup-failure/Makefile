.c.o:
	gcc -c -o $@ $<

check:
	echo "Type make check1 to perform the first check."
	echo "Type make check2 to perform the second check."

check1: cutcheck1.c
	gcc cutcheck1.c cut-explode1.o -o cutcheck
	./cutcheck

check2: cutcheck2.c
	gcc cutcheck2.c cut-explode2.o -o cutcheck
	./cutcheck

cutcheck1.c: cut-explode1.o
	cutgen-2 cut-explode1.c -o cutcheck1.c

cutcheck2.c: cut-explode2.o
	cutgen-2 cut-explode2.c -o cutcheck2.c

clean:
	rm -f *.o *~ cutcheck*

