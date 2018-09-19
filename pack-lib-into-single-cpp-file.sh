#!/bin/bash
INDIR="StanfordCPPLib"
OUTDIR="empty-project/lib/StanfordCPPLib"
OUTFILE="spl.cpp"
echo "Merging library .cpp source into $OUTDIR/$OUTFILE ..."
FILES=`cd $INDIR/ && find . -name "*.cpp"`
echo '' > $OUTDIR/$OUTFILE
for file in $FILES; do
	# echo $file
	cat $INDIR/$file >> $OUTDIR/$OUTFILE
	if [ -e $OUTDIR/$file ]; then
		rm $OUTDIR/$file
	fi
done
# echo "Done."
