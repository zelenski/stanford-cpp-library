# @author Julie Zelenski
# This sets the default goal of Makefile so that
# make with no argument will trigger make install.
# This is to get the staticlib + headers copied
# to install location without requiring student to
# select install target.

.DEFAULT_GOAL = assume_install

assume_install: message install

message:
	@echo "NOTE: Library project assuming install target"
