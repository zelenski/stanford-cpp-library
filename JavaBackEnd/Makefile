# Makefile for the Stanford Portable Library java side
# Last modified on Thu Aug  8 08:53:58 2013 by eroberts
#****************************************************************

JAR = spl.jar
CP = acm.jar:.

CLASSES = \
    stanford/spl/JavaBackEnd.class

PSFI = public static final int

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(JAR)

spl.jar: stanford/spl/JavaBackEnd.class
	@cp acm.jar spl.jar
	@(cd classes; jar ufm ../spl.jar ../JBEManifest.txt \
                      `find stanford -name '*.class'`)
	@echo jar cf spl.jar . . .

stanford/spl/JavaBackEnd.class: $(DERIVED) src/stanford/spl/*.java
	javac -d classes -classpath $(CP) -sourcepath src \
              src/stanford/spl/JavaBackEnd.java


# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	@rm -f `find . -name ',*' -o -name '.,*' -o -name '*~'`
	@rm -f `find . -name '*.tmp' -o -name '*.err'`
	@rm -f `find . -name core -o -name a.out`

scratch clean: tidy $(DERIVED)
	@rm -fr *.o *.a classes
	@mkdir classes