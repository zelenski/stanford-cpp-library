TEMPLATE = subdirs
SUBDIRS = Library ClientCompile

# ordered forces subprojects to build sequentially, according to SUBDIRS
CONFIG += ordered
