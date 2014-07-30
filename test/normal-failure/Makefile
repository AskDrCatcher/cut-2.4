check: cutcheck.c
	gcc cutcheck.c cut-explode.o -o cutcheck
	./cutcheck

cutcheck.c: cut-explode.o
	cutgen-2 cut-*.c -o cutcheck.c

cut-explode.o: cut-explode.c
	gcc -c cut-explode.c

clean:
	rm -f *.o *~ cutcheck*

