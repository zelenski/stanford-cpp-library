#!/bin/bash
OUTDIR=cppdoc/dist
CPPLIBZIPFILE=$OUTDIR/StanfordCPPLib.zip

echo "This script prepares ZIPs of the Stanford C++ library"
echo "for distribution and posting on the web."
echo "====================================================="
echo "Building $CPPLIBZIPFILE ..."
rm $CPPLIBZIPFILE 2>/dev/null
zip -rq $CPPLIBZIPFILE StanfordCPPLib/
zip -q --update $CPPLIBZIPFILE -j addr2line/addr2line.exe
zip -q --update $CPPLIBZIPFILE -j addr2line/addr2line64.exe
zip -q --update $CPPLIBZIPFILE -j $OUTDIR/iconstrip.png

EMPTYPROJECTZIPFILE=cppdoc/dist/empty-project.zip
echo "Building $EMPTYPROJECTZIPFILE ..."
cp -f StanfordCPPLib_QtCreatorProject/stanfordcpplib.pro empty-project/empty-project.pro
rm -rf empty-project/lib/StanfordCPPLib/
mkdir empty-project/lib/StanfordCPPLib/
cp -r StanfordCPPLib/* empty-project/lib/StanfordCPPLib/
./pack-lib-into-single-cpp-file.sh empty-project
rm $EMPTYPROJECTZIPFILE 2>/dev/null
zip -rq $EMPTYPROJECTZIPFILE empty-project/
cp -f empty-project/empty-project.pro $OUTDIR

SAMPLEPROJECTZIPFILE=cppdoc/dist/sample-project.zip
echo "Building $SAMPLEPROJECTZIPFILE ..."
cp -f StanfordCPPLib_QtCreatorProject/stanfordcpplib.pro sample-project/sample-project.pro
rm -rf sample-project/lib/StanfordCPPLib/
mkdir sample-project/lib/StanfordCPPLib/
cp -r StanfordCPPLib/* sample-project/lib/StanfordCPPLib/
./pack-lib-into-single-cpp-file.sh sample-project
rm $SAMPLEPROJECTZIPFILE 2>/dev/null
zip -rq $SAMPLEPROJECTZIPFILE sample-project/
cp -f sample-project/sample-project.pro $OUTDIR

cp -f StanfordCPPLib_QtCreatorProject/stanfordcpplib.pro autograder-empty-project/autograder-empty-project.pro

echo "Done."
