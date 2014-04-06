# Nmos: Ncurses mosaic asc art editor

source = main.c nmos.c img.c color.c
include = nmos.h img.h color.h
objs = main.o nmos.o color.o img.o

CFLAGS = -g -Wall -O2 -lpanel -lncurses -march=native


all : $(objs)
	$(CC) $(objs) $(CFLAGS) -o nmos


main.o : main.c nmos.o
	$(CC) -c $< $(CFLAGS)
	
nmos.o : nmos.c nmos.h color.o img.o
	$(CC) -c $< $(CFLAGS)

color.o : color.c color.h
	$(CC) -c $< $(CFLAGS)

img.o : img.c img.h
	$(CC) -c $< $(CFLAGS)



document :
	doxygen
	xdg-open html/index.html &

run :
	@./nmos

clean :
	@rm -rf html a.out *.o *~ teste.mos*
