#!/bin/bash
OUTDIR=cppdoc/dist
SPLJARFILE=JavaBackEnd/eclipseproject/obf/spl.jar
echo "Updating $SPLJARFILE in lib projects ..."
cp $SPLJARFILE StanfordCPPLib_QtCreatorProject/lib/spl.jar
cp $SPLJARFILE Autograder_QtCreatorProject/lib/spl.jar

echo "Copying $SPLJARFILE ..."
cp -f $SPLJARFILE $OUTDIR

CPPLIBFILE=$OUTDIR/StanfordCPPLib.zip
echo "Building $CPPLIBFILE ..."
rm $CPPLIBFILE 2>/dev/null
zip -rq $CPPLIBFILE StanfordCPPLib/
zip -q --update $CPPLIBFILE -j JavaBackEnd/eclipseproject/obf/spl.jar
zip -q --update $CPPLIBFILE -j $OUTDIR/addr2line.exe

EMPTYPROJECTFILE=cppdoc/dist/empty-project.zip
echo "Building $EMPTYPROJECTFILE ..."
cp -f StanfordCPPLib_QtCreatorProject/stanfordcpplib.pro empty-project/empty-project.pro
rm -rf empty-project/lib/StanfordCPPLib/
mkdir empty-project/lib/StanfordCPPLib/
cp -r StanfordCPPLib/* empty-project/lib/StanfordCPPLib/
rm -f empty-project/lib/spl.jar
cp -f $SPLJARFILE empty-project/lib/spl.jar
rm $EMPTYPROJECTFILE 2>/dev/null
zip -rq $EMPTYPROJECTFILE empty-project/
cp -f empty-project/empty-project.pro $OUTDIR

echo "Done."
