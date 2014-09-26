# Maae, a curses Mosaic ASC Art Editor
# Created by Gil Barbosa Reis

if not GetOption ('help'):
	env = Environment (
		LIBS = ['panel', 'menu', 'form', 'curses', 
				'mosaic', 'mosaic_color', 'mosaic_curses'],
		LIBPATH = ['/usr/lib', '/usr/local/lib'],
		CCFLAGS = '-g -Wall -pipe',
		CPPPATH = ['#include', '/usr/include'],
	)
	env.Decider ('MD5-timestamp')

	# build the editor in the 'build' directory, without duplicating
	VariantDir ('build', 'src', duplicate = 0)
	SConscript ('build/SConscript', exports = 'env')
