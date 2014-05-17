env = Environment (
	LIBS = ['ncurses', 'panel', 'menu'],
	LIBPATH = ['/usr/lib', '/usr/local/lib'],
	CCFLAGS = "-g -Wall -pipe"
)
env.Decider ('MD5-timestamp')


src = ['mosaic.c', 'img.c', 'color.c', 'main.c', 'positioning.c', 'wins.c'],
cat_src = ['img.c', 'moscat.c'],

env.Program ('maae', src)
env.Program ('moscat', cat_src)
