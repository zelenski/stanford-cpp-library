TEMPLATE = subdirs
SUBDIRS = Library Client

# ordered forces subprojects to build sequentially, according to SUBDIRS
CONFIG += ordered

#Client.depends = Install
#Install.file = Library/Library.pro
#Install.target = bogus
#Install.CONFIG = recursive

#uck.target = JZ_WAS_HERE
#duck.commands = touch $$mytarget.target
#duck.depends = blah


#parent.commands = @echo Building $$duck.target
#QMAKE_EXTRA_TARGETS
#panda.recurse#make panda recursion
#panda.recurse_target = install