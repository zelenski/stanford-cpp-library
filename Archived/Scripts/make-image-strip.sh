#!/bin/bash
cd icons/

# standard lib icon strip
convert \
	save.gif \
	save_as.gif \
	print.gif \
	script.gif \
	compare_output.gif \
	quit.gif \
	cut.gif \
	copy.gif \
	paste.gif \
	select_all.gif \
	clear_console.gif \
	font.gif \
	background_color.gif \
	text_color.gif \
	about.gif \
	check_for_updates.gif \
	+append \
	iconstrip.png

# autograder icon strip
convert \
	calendar.gif \
	check.gif \
	helpbig.gif \
	magnifier.gif \
	play.gif \
	pause.gif \
	stop.gif \
	textfile.gif \
	fail.gif \
	help.gif \
	pass.gif \
	running.gif \
	warn.gif \
	checkbox-checked.gif \
	checkbox-unchecked.gif \
	minus.gif \
	+append \
	iconstrip-autograder.png

cd ..
cp icons/iconstrip.png cppdoc/dist/
cp icons/iconstrip.png StanfordCPPLib_CodeStepByStep_Project/lib/
cp icons/iconstrip.png StanfordCPPLib_QtCreatorProject/lib/
cp icons/iconstrip.png sample-project/lib/
cp icons/iconstrip.png Autograder_QtCreatorProject/lib/
cp icons/iconstrip.png autograder-empty-project/lib/

cp icons/iconstrip-autograder.png Autograder_QtCreatorProject/lib/
cp icons/iconstrip-autograder.png autograder-empty-project/lib/

cp icons/progress.gif Autograder_QtCreatorProject/lib/
cp icons/progress.gif autograder-empty-project/lib/

cp icons/splicon-large.png cppdoc/dist/
cp icons/splicon-large.png StanfordCPPLib_CodeStepByStep_Project/lib/
cp icons/splicon-large.png StanfordCPPLib_QtCreatorProject/lib/
cp icons/splicon-large.png sample-project/lib/
cp icons/splicon-large.png Autograder_QtCreatorProject/lib/
cp icons/splicon-large.png autograder-empty-project/lib/
