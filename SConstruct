env = Environment (
	LIBS = ['ncurses', 'panel', 'menu'],
	LIBPATH = ['/usr/lib', '/usr/local/lib'],
	CCFLAGS = "-g -Wall -pipe"
)
env.Decider ('MD5-timestamp')


src = ['nmos.c', 'img.c', 'color.c', 'main.c', 'positioning.c', 'wins.c'],
cat_src = ['img.c', 'cat-nmos.c'],

env.Program ('nmos', src)
env.Program ('cat-nmos', cat_src)
