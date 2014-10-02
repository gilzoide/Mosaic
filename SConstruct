# Maae, a curses Mosaic ASC Art Editor
# Created by Gil Barbosa Reis

if not GetOption ('help'):
    env = Environment (
        LIBS = ['panel', 'menu', 'form', 'curses', 
                'mosaic', 'mosaic_color', 'mosaic_curses'],
        LIBPATH = ['/usr/lib', '/usr/local/lib'],
        CCFLAGS = '-Wall -pipe',
        CPPPATH = ['#include', '/usr/include'],
    )
    env.Decider ('MD5-timestamp')

    # if user pass debug=1, add -g flag for the compiler
    debug = ARGUMENTS.get ('debug', 0)
    if int (debug):
        env.Append (CCFLAGS = ' -g')

    # build the editor in the 'build' directory, without duplicating
    VariantDir ('build', 'src', duplicate = 0)
    SConscript ('build/SConscript', exports = 'env')
