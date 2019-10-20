# Makefile for pictures/ClassHierarchies
# Last modified on Wed Dec 19 19:57:44 2012 by eroberts
#****************************************************************

PICTURES = \
    GObjectHierarchy.pdf \
    GInteractorHierarchy.pdf \
    GEventHierarchy.pdf


# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(PICTURES)

GObjectHierarchy.pdf: GObjectHierarchy.ps hierarchy.ps Makefile
	makepdf -png -pad GObjectHierarchy.ps

GInteractorHierarchy.pdf: GInteractorHierarchy.ps hierarchy.ps Makefile
	makepdf -png -pad GInteractorHierarchy.ps

GEventHierarchy.pdf: GEventHierarchy.ps hierarchy.ps Makefile
	makepdf -png -pad GEventHierarchy.ps


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	rm -f *.o *.a *.html $(PICTURES)
