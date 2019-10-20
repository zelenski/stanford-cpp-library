# Makefile for HelloWorld
# Last modified on Tue Jul 26 18:02:56 2011 by eroberts
#****************************************************************

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: HelloWorld.pdf

HelloWorld.pdf: HelloWorld.ps
	makepdf -png -pad HelloWorld.ps


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	rm -f *.o *.a HelloWorld.pdf HelloWorld.png
