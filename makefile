# Nmos: Ncurses mosaic asc art editor

source = main.c nmos.c img.c color.c
include = nmos.h img.h color.h
objs = main.o nmos.o color.o img.o

CFLAGS = -g -O2 -march=native
CFLAGS_NCURSES = $(CFLAGS) -lncurses -lpanel


all : $(objs)
	$(CC) $(objs) $(CFLAGS_NCURSES)


main.o : main.c nmos.o
	$(CC) -c $< $(CFLAGS_NCURSES)
	
nmos.o : nmos.c nmos.h color.o img.o
	$(CC) -c $< $(CFLAGS_NCURSES)
	
color.o : color.c color.h
	$(CC) -c $< $(CFLAGS_NCURSES)

img.o : img.c img.h
	$(CC) -c $< $(CFLAGS)


document :
	doxygen
	xdg-open html/index.html

run : all
	@./a.out

clean :
	@rm -rf html a.out *.o *~
