# Makefile for StanfordCPPLib/doc
# Last modified on Sat Dec 15 10:17:20 2012 by eroberts
#****************************************************************

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all:
	cppdoc -title "The Stanford <code>cslib</code> package" \
               -icon "images/StanfordTreeLogo.png" \
               -description "include/package.html" \
               -stylesheet "cppdoc.css" -elide "platform.h" \
               -out . -sortindex -notemplate -novirtual -nostd ../include


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	@mv rationale.html rationale.htm
	rm -f *.o *.a *.html
	@mv rationale.htm rationale.html

