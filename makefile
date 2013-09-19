# Nmos: Ncurses mosaic asc art editor

objs = nmos.o color.o
CFLAGS = -lncurses -O3 -march=native


all : $(objs)
	@cc $(objs) $(CFLAGS)

	
nmos.o : nmos.c color.h
	@cc -c nmos.c $(CFLAGS)
	
color.o : color.h
	@cc -c color.c $(CFLAGS)

run : nmos.c a.out
	@./a.out



clean :
	@rm a.out $(objs) *~

commit : nmos.c makefile .gitignore
	@git commit -a && git push
