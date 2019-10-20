# Makefile for StanfordCPPLib/doc
# Last modified on Wed Dec 19 20:21:15 2012 by eroberts
#****************************************************************

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all:
	cppdoc -title "The Stanford <code>cslib</code> package" \
               -icon "images/StanfordTreeLogo.png" \
               -description "include/package.html" \
               -elide "generic.h" -elide "bst.h" -elide "platform.h" \
               -elide "iterator.h" -elide "itertype.h" \
               -elide "options.h" -elide "unittest.h" \
               -out . -generic -stylesheet cppdoc.css ..


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out *.err

scratch clean: tidy
	rm -f *.o *.a *.html

