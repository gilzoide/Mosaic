# Maae, a curses Mosaic ASC Art Editor
# Created by Gil Barbosa Reis

Help ("""
Welcome to the maae's build script

This package provides Maae: the Mosaic ASC Art Editor.
The default build output is the build/ directory.

You can `scons install` it in the /usr/bin/ directory,
and it can be uninstalled running `scons uninstall`.
""")

if not GetOption ('help'):
    env = Environment (
        LIBS = ['panel', 'menu', 'form', 'curses', 
                'mosaic', 'mosaic_color', 'mosaic_curses'],
        LIBPATH = ['/usr/lib', '/usr/local/lib'],
        CCFLAGS = '-Wall -pipe -O2 -g',
        CPPPATH = ['#include', '/usr/include'],
    )
    env.Decider ('MD5-timestamp')

    # if user pass debug=1, add -g flag for the compiler
    #debug = ARGUMENTS.get ('debug', 0)
    #if int (debug):
        #env.Append (CCFLAGS = ' -g')

    # build the editor in the 'build' directory, without duplicating
    VariantDir ('build', 'src', duplicate = 0)
    SConscript ('build/SConscript', exports = 'env')

    ## UNINSTALL ##
    env.Command ("uninstall", None, Delete (FindInstalledFiles()))
