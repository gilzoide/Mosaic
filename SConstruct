# Mosaic, a curses ascart editor
# Created by Gil Barbosa Reis
# SCons script for building the program

env = Environment (
	LIBS = ['panel', 'menu', 'form', 'ncurses'],
	LIBPATH = ['/usr/lib', '/usr/local/lib'],
	CCFLAGS = '-g -Wall -pipe',
	CPPPATH = '#include',
)
env.Decider ('MD5-timestamp')


# build the editor in the 'build' directory, without duplicating
VariantDir ('build', 'src', duplicate = 0)
SConscript ('build/SConscript', exports = 'env')
