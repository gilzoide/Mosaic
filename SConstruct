src = Glob ('*.c')
libs = ['ncurses', 'panel', 'menu']
libpath = ['/usr/lib', '/usr/local/lib']
Decider ('MD5-timestamp')

Program ('nmos', src, LIBS = libs, LIBPATH = libpath, CCFLAGS = "-g -Wall -pipe")
