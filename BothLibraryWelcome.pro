TEMPLATE = subdirs
SUBDIRS = Library Welcome

# ordered forces subprojects to build sequentially, according to SUBDIRS
CONFIG += ordered

# pass var down to subprojects
CURRENTLY_INSTALLING_LIBRARY = $$PWD
cache(CURRENTLY_INSTALLING_LIBRARY)
