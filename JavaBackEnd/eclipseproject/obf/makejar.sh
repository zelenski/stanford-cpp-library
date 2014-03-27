#!/bin/bash
# Make sure to set -source 1.5 -target 1.5 in compiler options
# so that the class files are compatible with Sandmark/Proguard

OUTFILE=spl.jar

echo "Compiling code ..."
if [ -e "spl.jar" ]; then
	rm spl.jar 2>&1 > /dev/null
fi

echo "Repacking library JARs ..."
rm -rf temp/*
cd temp
for JAR in `ls -1 ../../lib/*.jar`; do
	unzip -qo $JAR
done
cd ..

echo "Creating JAR archive in $OUTFILE ..."
cd temp
cp -r ../../bin/stanford/spl/*.class stanford/spl/
cp -r ../../bin/acm/graphics/*.class acm/graphics
jar -cvmf ../MANIFEST ../$OUTFILE acm/*/*.class stanford/*/*.class
cd ..
