#!/bin/bash
if [ $# -le 0 ]; then
	echo "Usage: $0 DIRECTORY"
	echo "e.g. : $0 empty-project"
	exit 1
fi

INDIR="autograder"
OUTDIR="$1/lib/StanfordCPPLib"
OUTFILE="splautograder.cpp"
echo "Merging library .cpp source into $OUTDIR/$OUTFILE ..."
FILES=`cd $INDIR/ && find . -name "*.cpp"`
echo '' > $OUTDIR/$OUTFILE
cp spl_cpp_header_autograder_text.txt $OUTDIR/$OUTFILE

for file in $FILES; do
	# echo $file
	cat $INDIR/$file >> $OUTDIR/$OUTFILE
	
	# append blank line
	echo "" >> $OUTDIR/$OUTFILE
	
	# remove .cpp files from project folders (only need spl.cpp)
	if [ -e $OUTDIR/$file ]; then
		rm $OUTDIR/$file
	fi
done
# echo "Done."
