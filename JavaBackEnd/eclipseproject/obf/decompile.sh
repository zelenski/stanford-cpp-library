#!/bin/sh
rm -rf decompiled
mkdir decompiled
jar -xf ../provided/SortDetective.jar
# wine jad.exe *.class
jad.exe *.class
mv *.class decompiled
mv *.jad decompiled
mv META-INF decompiled
