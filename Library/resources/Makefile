# Packs a set of individual gif files into icon strip for ease of distribution

PACKED_STRIP = iconstrip.png iconstrip-autograder.png
PRODUCTS = $(PACKED_STRIP)

all:: $(PRODUCTS)

ICONS = save.gif save_as.gif print.gif script.gif compare_output.gif \
	quit.gif cut.gif copy.gif paste.gif select_all.gif clear_console.gif \
	font.gif background_color.gif text_color.gif about.gif check_for_updates.gif

AG_ICONS = calendar.gif check.gif helpbig.gif magnifier.gif play.gif \
	pause.gif stop.gif textfile.gif fail.gif help.gif pass.gif running.gif \
	warn.gif checkbox-checked.gif checkbox-unchecked.gif minus.gif

iconstrip.png: $(ICONS)
iconstrip-autograder.png: $(AG_ICONS)

$(PACKED_STRIP):
	convert $^ +append $@

clean::
	rm -rf $(PRODUCTS)

.PHONY: all clean 
