#!/bin/bash
OUTDIR=doc
echo "Creating Javadoc documents in $OUTDIR ..."

#!/bin/bash
#    -classpath ".:/home/stepp/Android/Sdk/platforms/android-23/android.jar:/home/stepp/Android/Sdk/tools/support/annotations.jar:/home/stepp/Android/Sdk/extras/android/m2repository/com/android/support/support-annotations/23.1.1/support-annotations-23.1.1.jar" \
#    -linkoffline http://developer.android.com/reference/ ~/Android/Sdk/docs/reference/ \
#    -sourcepath "/home/stepp/Downloads/jdk1.8.0_73/src.zip" \

javadoc \
    -classpath ".:/home/stepp/Downloads/jdk1.8.0_73/lib/tools.jar:obf/spl.jar" \
    -d doc/ \
    -link https://docs.oracle.com/javase/8/docs/api \
    -tag usage \
    -tag inherited \
    -tag noshow \
    -exclude javazoom.jlgui.basicplayer:stanford.spl:stanford.cs106.autograder \
    src/acm/*/*.java \
    src/stanford/karel/*.java \
    src/stanford/cs106/audio/*.java \
    src/stanford/cs106/collections/*.java \
    src/stanford/cs106/gui/*.java \
    src/stanford/cs106/io/*.java \
    src/stanford/cs106/junit/*.java \
    src/stanford/cs106/net/*.java \
    src/stanford/cs106/reflect/*.java \
    src/stanford/cs106/util/*.java \

echo "Done."
