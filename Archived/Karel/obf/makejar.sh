#!/bin/bash
# Make sure to set -source 1.5 -target 1.5 in compiler options
# so that the class files are compatible with Sandmark/Proguard

OUTFILE=karel.jar

echo "Compiling code ..."
if [ -e "in.jar" ]; then
	rm karel.jar 2>&1 > /dev/null
fi

if [ ! -d "temp" ]; then
	mkdir temp
fi

javac -source 1.5 -target 1.5 -cp "../src:../lib/spl.jar" -d "temp" ../src/stanford/karel/*.java

echo "Repacking library JARs ..."
cd temp
for JAR in `ls -1 ../../lib/spl.jar`; do
	unzip -qo $JAR
done
cd ..

echo "Creating JAR archive in $OUTFILE ..."
cd temp
jar -cvf ../$OUTFILE *.class */*.class */*/*.class */*/*/*.class 2>/dev/null
cd ..
rm -rf temp/*
