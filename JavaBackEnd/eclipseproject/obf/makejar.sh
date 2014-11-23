#!/bin/bash
# Make sure to set -source 1.5 -target 1.5 in compiler options
# so that the class files are compatible with Sandmark/Proguard

OUTFILE=spl.jar

echo "Compiling code ..."
rm -rf temp/*
# ./compile.sh

echo "Repacking library JARs ..."
if [ -e "spl.jar" ]; then
	rm spl.jar 2>&1 > /dev/null
fi

cd temp
for JAR in `ls -1 ../../lib/*.jar`; do
	unzip -qo $JAR
done
cd ..

echo "Creating JAR archive in $OUTFILE ..."
cd temp
#cp -r ../../bin/stanford/cs106/diff/*.class stanford/cs106/diff/
#cp -r ../../bin/stanford/cs106/gui/*.class stanford/cs106/gui/
#cp -r ../../bin/stanford/cs106/io/*.class stanford/cs106/io/
#cp -r ../../bin/stanford/cs106/util/*.class stanford/cs106/util/
#cp -r ../../bin/stanford/spl/*.class stanford/spl/
#cp -r ../../bin/acm/graphics/*.class acm/graphics
#cp -r ../../bin/acm/io/*.class acm/io
#cp -r ../../bin/acm/util/*.class acm/util
cp -r ../../bin/* .
jar -cvmf ../MANIFEST ../$OUTFILE acm/*/*.class stanford/*/*.class stanford/*/*/*.class
cd ..
