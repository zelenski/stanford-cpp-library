TEMPLATE = subdirs
SUBDIRS = Library Welcome DebugHelper ShadowTest SimpleTest BugFixes

# ordered forces subprojects to build sequentially, according to SUBDIRS
CONFIG += ordered
