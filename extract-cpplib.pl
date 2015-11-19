#!/usr/bin/perl -w
# This silly script takes the entire Stanford C++ library and merges it into
# a single C++ source file so that it can easily be embedded inside a student
# program without needing to create complicated build/header files.

my $outfile = "stanfordcpplib.cpp";
if ($#ARGV >= 0) {
	$outfile = shift @ARGV;
}
my $basedir = "StanfordCPPLib";
my $projdir = "StanfordCPPLib_QtCreatorProject/src";
my $autograder_basedir = "autograder";
my $practiceit_dir = "/home/stepp/Dropbox/data/docs/practiceit/web";

my @ALL_FILES = (
# .h headers
# basic library functionality
"$basedir/base64.h",             # deps: none
"$basedir/compare.h",            # deps: none
"$basedir/direction.h",          # deps: none
"$basedir/error.h",              # deps: none
"$basedir/gtypes.h",             # deps: none
"$basedir/hashcode.h",           # deps: none
"$basedir/random.h",             # deps: none
"$basedir/point.h",              # deps: none
"$basedir/simpio.h",             # deps: none
"$basedir/strlib.h",             # deps: none
"$basedir/plainconsole.h",       # deps: none
"$basedir/gmath.h",              # deps: gtypes.h
"$basedir/shuffle.h",            # deps: random.h
"$basedir/observable.h",         # deps: error.h

"$basedir/exceptions.h",
"$basedir/vector.h",

"$basedir/filelib.h",            # deps: vector.h

# collections
"$basedir/queue.h",
"$basedir/stack.h",
"$basedir/deque.h",

"$basedir/private/tokenpatch.h",
"$basedir/tokenscanner.h",

"$basedir/map.h",
"$basedir/set.h",
"$basedir/hashmap.h",
"$basedir/hashset.h",

"$basedir/graph.h",              # deps: map, set, tokenscanner
"$basedir/basicgraph.h",         # deps: map, set, graph, 
"$basedir/grid.h",
"$basedir/lexicon.h",
"$basedir/linkedlist.h",
"$basedir/pqueue.h",
"$basedir/sparsegrid.h",
"$basedir/dawglexicon.h",        # deps: set.h
"$basedir/linkedhashmap.h",      
"$basedir/linkedhashset.h",      # deps: linkedhashmap.h


# gui; not wanted
"$basedir/gwindow.h",
"$basedir/gobjects.h",           # deps: gtypes, gwindow
"$basedir/gtimer.h",
"$basedir/gevents.h",
"$basedir/ginteractors.h",

"$basedir/gbufferedimage.h",
"$basedir/gfilechooser.h",
"$basedir/goptionpane.h",

# not wanted
"$basedir/bitstream.h",
#"$basedir/foreach.h",
#"$basedir/console.h",

"$basedir/sound.h",
"$basedir/platform.h",
#"$basedir/private/foreachpatch.h",
#"$basedir/private/main.h",
#"$basedir/private/randompatch.h",
#"$basedir/private/tplatform.h",
"$basedir/private/version.h",
"$basedir/regexpr.h",
"$basedir/stacktrace/call_stack.h",
"$basedir/stacktrace/stack_exception.h",
"$basedir/timer.h",
"$basedir/urlstream.h",

# .cpp implementations
"$basedir/base64.cpp",
"$basedir/direction.cpp",
"$basedir/error.cpp",
"$basedir/gtypes.cpp",
"$basedir/gmath.cpp",
"$basedir/hashcode.cpp",
"$basedir/random.cpp",
"$basedir/shuffle.cpp",
"$basedir/strlib.cpp",
"$basedir/plainconsole.cpp",       # deps: error

# collections
"$basedir/basicgraph.cpp",
"$basedir/dawglexicon.cpp",
"$basedir/lexicon.cpp",

"$basedir/exceptions.cpp",
"$basedir/filelib.cpp",

#"$basedir/main.cpp",

"$basedir/observable.cpp",
"$basedir/platform.cpp",
"$basedir/point.cpp",
"$basedir/regexpr.cpp",
"$basedir/simpio.cpp",
"$basedir/sound.cpp",
"$basedir/stacktrace/call_stack_gcc.cpp",
"$basedir/startup.cpp",
"$basedir/timer.cpp",
"$basedir/tokenscanner.cpp",
"$basedir/urlstream.cpp",
"$basedir/version.cpp",

# not wanted
"$basedir/bitstream.cpp",
#"$basedir/console.cpp",

# gui; not wanted
"$basedir/gbufferedimage.cpp",
"$basedir/gevents.cpp",
"$basedir/gfilechooser.cpp",
"$basedir/ginteractors.cpp",
"$basedir/gobjects.cpp",
"$basedir/goptionpane.cpp",
"$basedir/gtimer.cpp",
"$basedir/gwindow.cpp",

# Practice-It CPP test harness
#"$practiceit_dir/PracticeItCppTestHarness.cpp_",

# some classes that come from practice exams but are needed for 106B section/exam problems
"$projdir/ArrayIntList.h",
"$projdir/ArrayIntList.cpp",
"$projdir/BinaryTreeNode.h",
"$projdir/BinaryTreeNode.cpp",
"$projdir/BinaryTree.h",
"$projdir/BinaryTree.cpp",
"$projdir/BinaryTreeMapNode.h",
"$projdir/BinaryTreeMap.h",
"$projdir/BinaryTreeMap.cpp",
"$projdir/HashTableMap.h",
"$projdir/HashTableSet.h",
"$projdir/HashTableSet.cpp",
"$projdir/HeapPriorityQueue.h",
"$projdir/HeapPriorityQueue.cpp",
"$projdir/LinkedIntList.h",
"$projdir/LinkedIntList.cpp",
"$projdir/graphsupport.h",
"$projdir/graphsupport.cpp",
"$projdir/types.h",
"$projdir/types.cpp",
);


# "main"

print("C++ library merger tool\n");
print("by Marty Stepp\n");
print("=======================\n");

# gather the C++ code text
print("Reading source code ...\n");
my $overall_text = "using namespace std;\n\n";
my $overall_h_text = "#define STANFORD_CPP_LIB_PRESENT true\n\n";
$overall_h_text .= "using namespace std;\n\n";
foreach my $cppfile (@ALL_FILES) {
	print("  - $cppfile\n");
	$cppfiletext = `cat $cppfile`;
	
	# clean up the code a bit;
	# remove local includes from within lib
	$cppfiletext =~ s/#include[ \t]{0,99}"[^"]{1,99}".{0,99}\n/\n/gi;
	
	# remove redeclaration of, static Platform *pp = getPlatform();
	# $cppfiletext = regex_remove_lines($cppfiletext, "static[ ]*Platform[ *]+pp[ ]*=[ ]*getPlatform();[ ]*");
	
	$overall_text .= $cppfiletext . "\n";
	if ($cppfile =~ m/\.h$/) {
		$overall_h_text .= $cppfiletext . "\n";
	}
}

print("Writing output file $outfile ...\n");
open(OUTFILE, ">", $outfile) or die "Cannot open $outfile for writing: $!";
print OUTFILE $overall_text;
close OUTFILE;

my $out_h_file = $outfile;
$out_h_file =~ s/\.cpp/.h/;
print("Writing header file $out_h_file ...\n");
open(OUTFILE, ">", $out_h_file) or die "Cannot open $out_h_file for writing: $!";
print OUTFILE $overall_h_text;
close OUTFILE;

print("Complete.\n\n");


sub regex_remove_lines {
	my $text = shift;
	my $regex = shift;
	my @lines = split(/\r?\n/, $text);
	for (my $i = 0; $i <= $#lines; $i++) {
		$lines[$i] =~ s/$regex//gi;
	}
	$text = join("\n", @lines);
	return $text;
}
