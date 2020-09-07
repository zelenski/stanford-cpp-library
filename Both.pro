TEMPLATE = subdirs
SUBDIRS = Library Client ClientSimpleTest ClientDebug ClientOverride

# ordered forces subprojects to build sequentially, according to SUBDIRS
CONFIG += ordered
