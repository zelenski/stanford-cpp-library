# Quick and dirty Makefile to generate qrc file from set of image files
# for use as library resource package
#
# @author Julie Zelenski
# @version 2019/11/05

QRC_FILE = generated.qrc
PRODUCTS = $(QRC_FILE)

all:: $(PRODUCTS)

glob_img = $(wildcard $(1)/*.gif) $(wildcard $(1)/*.png)

# argument is output, prefix, directory of files, path
define jz
	@echo '<qresource prefix="$2">' >>$1
	@$(foreach f, $(call glob_img,$3), echo '<file alias="$(notdir $(basename $f))">$4$(notdir $f)</file>' >>$1;)
	@echo "</qresource>"  >>$1
endef

$(QRC_FILE):
		@rm -f $@
		@echo "<!DOCTYPE RCC><RCC version=\"1.0\">" >> $@
		$(call jz,$@,/,.,images/)
		@echo "</RCC>" >> $@
		@echo "Wrote resource file" $@
		cat $@

clean::
	rm -rf $(PRODUCTS)

.PHONY: all clean
