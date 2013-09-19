# Nmos: Ncurses mosaic asc art editor

all : nmos.c
	@cc nmos.c -O3 -march=native

run : nmos.c a.out
	@./a.out

commit : nmos.c makefile .gitignore
	@git commit -a && git push
