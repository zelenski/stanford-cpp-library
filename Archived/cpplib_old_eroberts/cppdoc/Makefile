# Makefile for cppdoc
# Last modified on Thu Jul 21 15:09:26 2011 by eroberts
#****************************************************************

PROGRAMS = \
    appsection \
    cppdoc \
    fillcomments

EXPORTDIR = $(HOME)/bin

CC = gcc -I../cslib

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(PROGRAMS)

export: all
	@-(for x in $(PROGRAMS) ; do \
             echo link `pwd`/$$x $(EXPORTDIR) ; \
             rm -f $(EXPORTDIR)/$$x ; \
             ln -s `pwd`/$$x $(EXPORTDIR) ; \
           done)

appsection.o: appsection.c
	$(CC) -c appsection.c

cppdoc.o: cppdoc.c
	$(CC) -c cppdoc.c


fillcomments.o: fillcomments.c
	$(CC) -c fillcomments.c

appsection: appsection.o
	$(CC) -o appsection appsection.o

cppdoc: cppdoc.o
	$(CC) -o cppdoc cppdoc.o

fillcomments: fillcomments.o
	$(CC) -o fillcomments fillcomments.o


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	rm -f *.o *.a
