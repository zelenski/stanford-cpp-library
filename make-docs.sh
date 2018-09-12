#!/bin/bash
cd doc
doxygen doxygen.ini
for FILE in `ls -1B html/*html`; do
	./postprocess-doc-file.pl $FILE
done
cd ..
