exec: hexeditplus.c hexeditplus.h
	gcc -o exec hexeditplus.c

clear:
	rm ./exec