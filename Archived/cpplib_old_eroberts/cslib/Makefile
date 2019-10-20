# Makefile for cslib directory
# Last modified on Tue Jan 15 14:51:23 2013 by eroberts
#****************************************************************

SCRIPTS = \
    gccx

TESTS = \
    cstest

OBJECTS = \
    bst.o \
    charset.o \
    cmpfn.o \
    cslib.o \
    exception.o \
    filelib.o \
    foreach.o \
    generic.o \
    gevents.o \
    gobjects.o \
    gmath.o \
    graph.o \
    gtimer.o \
    gtransform.o \
    gtypes.o \
    gwindow.o \
    hashmap.o \
    iterator.o \
    loadobj.o \
    map.o \
    options.o \
    posixthread.o \
    platform.o \
    pqueue.o \
    queue.o \
    random.o \
    ref.o \
    set.o \
    simpio.o \
    sound.o \
    stack.o \
    strbuf.o \
    strlib.o \
    thread.o \
    tokenscanner.o \
    unittest.o \
    unixfile.o \
    vector.o

HEADERS = \
    bst.h \
    charset.h \
    cmdscan.h \
    cmpfn.h \
    cslib.h \
    exception.h \
    filelib.h \
    foreach.h \
    gcdebug.h \
    gevents.h \
    gmath.h \
    gobjects.h \
    graph.h \
    gtimer.h \
    gtransform.h \
    gtypes.h \
    gwindow.h \
    hashmap.h \
    iterator.h \
    itertype.h \
    list.h \
    loadobj.h \
    map.h \
    options.h \
    platform.h \
    pqueue.h \
    queue.h \
    random.h \
    ref.h \
    set.h \
    simpio.h \
    sound.h \
    stack.h \
    strbuf.h \
    strlib.h \
    thread.h \
    tokenscanner.h \
    unittest.h \
    vector.h

LIBRARIES = libcs.a


# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry
#    The / separators are for parallel versions of make

all: $(OBJECTS) / $(LIBRARIES) / $(TESTS)

export: all
	@-(if test -w /usr/local/bin ; then \
             target=/usr/local/bin ; \
           else \
             target=$(HOME)/bin ; \
           fi ; \
           if ! test -d $$target ; then mkdir $$target ; fi ; \
           for x in $(PROGRAMS) $(SCRIPTS) ; do \
             echo link `pwd`/$$x $$target ; \
             rm -f $$target/$$x ; \
             ln -s `pwd`/$$x $$target ; \
           done)
	@-(if test -w /usr/local/lib ; then \
             target=/usr/local/lib ; \
           else \
             target=$(HOME)/lib ; \
           fi ; \
           if ! test -d $$target ; then mkdir $$target ; fi ; \
           for x in $(LIBRARIES) ; do \
             echo link `pwd`/$$x $$target ; \
             rm -f $$target/$$x ; \
             ln -s `pwd`/$$x $$target ; \
           done)
	@-(if test -w /usr/local/include ; then \
             target=/usr/local/include ; \
           else \
             target=$(HOME)/include ; \
           fi ; \
           if ! test -d $$target ; then mkdir $$target ; fi ; \
           for x in $(HEADERS) private ; do \
             echo link `pwd`/$$x $$target ; \
             rm -f $$target/$$x ; \
             ln -s `pwd`/$$x $$target ; \
           done)
	@-(if test -w /usr/local/include ; then \
             target=/usr/local/include/cpph ; \
           else \
             target=$(HOME)/include/cpph ; \
           fi ; \
           if ! test -d $$target ; then mkdir $$target ; fi ; \
           for x in $(CPPHEADERS) ; do \
             echo link `pwd`/cpph/$$x $$target ; \
             rm -f $$target/$$x ; \
             ln -s `pwd`/cpph/$$x $$target ; \
           done)


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err *.gch

scratch clean: tidy
	rm -f -r *.o *.a *.app $(TESTS)


# ***************************************************************
# Tools

cstest.o: cstest.c cslib.h exception.h strlib.h unittest.h
	gccx -c cstest.c

cstest: cstest.o libcs.a
	gccx -o cstest cstest.o


# ***************************************************************
# Library compilations

bst.o: bst.c bst.h cmpfn.h cslib.h exception.h foreach.h iterator.h \
       itertype.h strlib.h unittest.h
	gccx -c bst.c

charset.o: gccx charset.c cslib.h foreach.h iterator.h itertype.h strlib.h \
           unittest.h
	gccx -c charset.c

cmdscan.o: gccx cmdscan.c cmdscan.h cmpfn.h cslib.h exception.h generic.h \
           hashmap.h iterator.h itertype.h simpio.h strlib.h tokenscanner.h
	gccx -c cmdscan.c

cmpfn.o: gccx cmpfn.c cmpfn.h cslib.h strlib.h
	gccx -c cmpfn.c

cslib.o: gccx cslib.c cslib.h exception.h
	gccx -c cslib.c

exception.o: gccx exception.c cslib.h exception.h strlib.h \
             thread.h unittest.h
	gccx -c exception.c

filelib.o: gccx filelib.c cslib.h cmpfn.h exception.h filelib.h generic.h \
           iterator.h itertype.h strlib.h unittest.h
	gccx -c filelib.c

foreach.o: gccx foreach.c cslib.h foreach.h iterator.h
	gccx -c foreach.c

generic.o: gccx generic.c cmpfn.h cslib.h exception.h generic.h \
           ref.h strlib.h unittest.h
	gccx -c generic.c

gevents.o: gccx gevents.c cslib.h gevents.h gtimer.h gtypes.h hashmap.h \
           platform.h
	gccx -c gevents.c

gmath.o: gccx gmath.c gmath.h
	gccx -c gmath.c

gobjects.o: gccx gobjects.c gobjects.h platform.h vector.h
	gccx -c gobjects.c

graph.o: gccx graph.c cmpfn.h cslib.h foreach.h generic.h graph.h iterator.h \
         itertype.h set.h
	gccx -c graph.c

gtimer.o: gccx gtimer.c cslib.h
	gccx -c gtimer.c

gtransform.o: gccx gtransform.c gmath.h gtransform.h gtypes.h
	gccx -c gtransform.c

gtypes.o: gccx gtypes.c cslib.h generic.h gtypes.h
	gccx -c gtypes.c

gwindow.o: gccx gwindow.c gobjects.h gwindow.h platform.h
	gccx -c gwindow.c

hashmap.o: gccx hashmap.c cmpfn.h cslib.h exception.h generic.h hashmap.h \
          iterator.h itertype.h strlib.h unittest.h
	gccx -c hashmap.c

iterator.o: gccx iterator.c cmpfn.h cslib.h iterator.h itertype.h
	gccx -c iterator.c

loadobj.o: gccx loadobj.c cslib.h filelib.h loadobj.h strlib.h
	gccx -c loadobj.c

map.o: gccx map.c bst.h cmpfn.h cslib.h exception.h foreach.h generic.h \
       iterator.h itertype.h map.h strlib.h unittest.h
	gccx -c map.c

options.o: gccx options.c cslib.h generic.h hashmap.h options.h strlib.h \
           unittest.h vector.h
	gccx -c options.c

platform.o: gccx platform.c cslib.h filelib.h gtimer.h platform.h strlib.h
	gccx -c platform.c

posixthread.o: gccx posixthread.c cslib.h exception.h strlib.h thread.h 
	gccx -c posixthread.c

pqueue.o: gccx pqueue.c cslib.h generic.h queue.h unittest.h
	gccx -c pqueue.c

queue.o: gccx queue.c cslib.h generic.h queue.h unittest.h
	gccx -c queue.c

random.o: gccx random.c cslib.h random.h
	gccx -c random.c

ref.o: gccx ref.c cslib.h ref.h
	gccx -c ref.c

set.o: gccx set.c bst.h cmpfn.h cslib.h generic.h iterator.h itertype.h set.h
	gccx -c set.c

simpio.o: gccx simpio.c cslib.h simpio.h strlib.h
	gccx -c simpio.c

sound.o: gccx sound.c cslib.h platform.h sound.h
	gccx -c sound.c

stack.o: gccx stack.c cslib.h generic.h stack.h unittest.h
	gccx -c stack.c

strbuf.o: gccx strbuf.c cslib.h exception.h strbuf.h strlib.h unittest.h
	gccx -c strbuf.c

strlib.o: gccx strlib.c cslib.h exception.h strlib.h unittest.h
	gccx -c strlib.c

thread.o: gccx thread.c cslib.h exception.h thread.h unittest.h
	gccx -c thread.c

tokenscanner.o: gccx tokenscanner.c cslib.h strlib.h tokenscanner.h
	gccx -c tokenscanner.c                

unittest.o: gccx unittest.c cslib.h exception.h generic.h strlib.h unittest.h
	gccx -c unittest.c

unixfile.o: gccx unixfile.c cslib.h filelib.h iterator.h strlib.h
	gccx -c unixfile.c

vector.o: gccx vector.c cmpfn.h cslib.h generic.h iterator.h itertype.h \
          vector.h
	gccx -c vector.c


# ***************************************************************
# Entry to reconstruct the library archive

libcs.a: $(OBJECTS)
	-rm -f libcs.a
	ar cr libcs.a $(OBJECTS)
	ranlib libcs.a
