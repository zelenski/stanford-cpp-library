#!/bin/bash
cd icons/
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
cd ..
cp icons/iconstrip.png cppdoc/dist/
cp icons/iconstrip.png Autograder_QtCreatorProject/res/
cp icons/iconstrip.png autograder-empty-project/res/
cp icons/iconstrip.png StanfordCPPLib_CodeStepByStep_Project/res/
cp icons/iconstrip.png StanfordCPPLib_QtCreatorProject/res/
cp icons/iconstrip.png autograder-empty-project/res/
cp icons/iconstrip.png sample-project/res/
