#!/bin/bash
# Make sure to set -source 1.5 -target 1.5 in compiler options
# so that the class files are compatible with Sandmark/Proguard

OUTFILE=spl.jar

echo "Compiling code ..."
cd ..
javac -g \
	-source 1.5 -target 1.5 \
	-cp lib/acm.jar \
	-s src/ \
	-d obf/temp/ \
	src/acm/graphics/*.java \
	src/acm/io/*.java \
	src/acm/util/*.java \
	src/stanford/cs106/diff/*.java \
	src/stanford/cs106/gui/*.java \
	src/stanford/cs106/io/*.java \
	src/stanford/cs106/util/*.java \
	src/stanford/spl/*.java \
