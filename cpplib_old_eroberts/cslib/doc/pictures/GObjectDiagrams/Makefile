# Makefile for pictures/GObjectDiagrams
# Last modified on Fri Dec 14 23:52:48 2012 by eroberts
#****************************************************************

PICTURES = \
    GArcExamples.pdf \
    GArcGeometry.pdf \
    GLabelGeometry.pdf \
    GPolygonExamples.pdf \
    PacMan.pdf \
    StopSign.pdf


# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(PICTURES)

GArcExamples.pdf: GArcExamples.ps Makefile
	makepdf -png -pad GArcExamples.ps

GArcGeometry.pdf: GArcGeometry.ps Makefile
	makepdf -png -pad GArcGeometry.ps

GLabelGeometry.pdf: GLabelGeometry.ps Makefile
	makepdf -png -pad GLabelGeometry.ps

GPolygonExamples.pdf: GPolygonExamples.ps Makefile
	makepdf -png -pad GPolygonExamples.ps

PacMan.pdf: PacMan.ps Makefile
	makepdf -png -pad PacMan.ps

StopSign.pdf: StopSign.ps Makefile
	makepdf -png -pad StopSign.ps


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	rm -f *.o *.a *.html $(PICTURES)
