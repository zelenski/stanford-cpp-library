#!/bin/sh
# Make sure to set -source 1.5 -target 1.5 in compiler options
# so that the class files are compatible with Sandmark/Proguard

OBFS[0]='Boolean Splitter'
OBFS[1]="Simple Opaque Predicates"
OBFS[2]='Transparent Branch Insertion'
OBFS[3]='Random Dead Code'
OBFS[4]='Duplicate Registers'
OBFS[5]='Merge Local Integers'
#OBFS[6]='Interleave Methods'
#OBFS[2]='Insert Opaque Predicates'
#OBFS[5]='Reorder Instructions'
#OBFS[8]='Buggy Code'         # crashes with exception no matter what
#OBFS[9]='String Encoder'

if [ ! -e "in.jar" ]; then
	./makejar.sh
fi

# phase 1: ProGuard (name obfuscator and other obfuscations, but not as l33t as Sandmark)
echo ""
echo "Running ProGuard name obfuscator ..."
rm -rf in/ out/
mkdir in
mkdir out
cp in.jar in/temp.jar
java -jar jars/proguard.jar @proguard-settings.pro | grep -v "Note" | grep -v "accesses a" | grep -v "Maybe this is library" | grep -v "Maybe this is program"
cp out/temp.jar ./out.jar

# phase 2: Sandmark
echo ""
echo "Running Sandmark code obfuscator ..."
let LAST="${#OBFS[@]} - 1"
for i in `seq 0 $LAST`; do
	OBF=${OBFS[$i]}
	echo "Sandmark obfuscation #$i: $OBF"
	cp out.jar temp.jar
	java -cp jars/sandmark.jar sandmark.smash.SandmarkCLI -O -i temp.jar -o temp2.jar -A "$OBF"
	if [ -e temp2.jar ]; then rm out.jar; fi
	if [ -e temp2.jar ]; then mv temp2.jar out.jar; fi
done

# phase 3: clean up and move files
mv out.jar OBFUSCATED.jar
#cp OBFUSCATED.jar ../provided/
#rm temp.jar *.smconfig
# rm retro_log.txt
echo ""
echo "Done!  Created OBFUSCATED.jar ."
