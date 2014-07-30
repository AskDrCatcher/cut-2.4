check: cutcheck.c
	gcc cutcheck.c cut-pass.o -o cutcheck
	./cutcheck

cutcheck.c: cut-pass.o
	cutgen-2 cut-*.c -o cutcheck.c

cut-pass.o: cut-pass.c
	gcc -c cut-pass.c

clean:
	rm -f *.o *~ cutcheck*

