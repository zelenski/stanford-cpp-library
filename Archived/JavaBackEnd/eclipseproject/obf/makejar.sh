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
cp -r ../../res/ .
cd ..

echo "Creating JAR archive in $OUTFILE ..."
cd temp
cp -r ../../bin/* .
find . > files.list
jar -cvmf ../MANIFEST ../$OUTFILE @files.list

# also include source code in JAR
cd ../../src
jar uvf ../obf/$OUTFILE acm/*/*.java javazoom/*/*/*.java stanford/*/*.java stanford/*/*/*.java

# copy in the META-INF folder so mp3 files can play
cd ../obf
# zip -ru $OUTFILE META-INF/
jar -uvf $OUTFILE META-INF/
