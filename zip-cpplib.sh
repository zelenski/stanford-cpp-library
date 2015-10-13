#!/bin/bash
rm cppdoc/StanfordCPPLib.zip
zip -r cppdoc/StanfordCPPLib.zip StanfordCPPLib/
cd cppdoc
cp ../JavaBackEnd/eclipseproject/obf/spl.jar .
cp ../StanfordCPPLib_QtCreatorProject/lib/addr2line.exe .
zip StanfordCPPLib.zip spl.jar
zip StanfordCPPLib.zip addr2line.exe
scp StanfordCPPLib.zip stepp@myth.stanford.edu:/afs/.ir.stanford.edu/users/s/t/stepp/WWW/cppdoc/StanfordCPPLib.zip
cd ..
