# Makefile for the ACM Java Task Force acm.jar
# Last modified on Thu Aug  8 08:53:58 2013 by eroberts
#****************************************************************

JAR = acm.jar
CP = "."

CLASSES = \
    stanford/spl/JavaBackEnd.class

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(JAR)

acm.jar: classes/acm/program/Program.class
	@(cd classes; jar cf ../acm.jar `find acm -name '*.class'`; cd ..; cp -f acm.jar ../JavaBackEnd)
	
classes/acm/program/Program.class: src/acm/*/*.java
	javac -d classes -classpath $(CP) -sourcepath src \
              src/acm/*/*.java

# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	@rm -f `find . -name ',*' -o -name '.,*' -o -name '*~'`
	@rm -f `find . -name '*.tmp' -o -name '*.err'`
	@rm -f `find . -name core -o -name a.out`

scratch clean: tidy $(DERIVED)
	@rm -fr *.o *.a classes $(JAR)
	@mkdir classes