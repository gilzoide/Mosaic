# Nmos: Ncurses mosaic asc art editor

source = main.c nmos.c img.c color.c wins.c positioning.c
include = nmos.h img.h color.h wins.h positioning.h state.h
objs = main.o nmos.o color.o img.o wins.o positioning.o

CFLAGS = -g -Wall -O2 -march=native
CURSES_CFLAGS = $(CFLAGS) -lpanel -lmenu -lcurses


all : $(objs)
	$(CC) $(objs) $(CURSES_CFLAGS) -o nmos


main.o : main.c nmos.o
	$(CC) -c $< $(CFLAGS)
	
nmos.o : nmos.c nmos.h color.o img.o wins.o state.h
	$(CC) -c $< $(CFLAGS)

positioning.o : positioning.c positioning.h img.o state.h
	$(CC) -c $< $(CFLAGS)

wins.o : wins.c wins.h positioning.o
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
