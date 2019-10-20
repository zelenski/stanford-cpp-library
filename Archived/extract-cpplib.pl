#!/usr/bin/perl -w
# This silly script takes the entire Stanford C++ library and merges it into
# a single C++ source file so that it can easily be embedded inside a student
# program without needing to create complicated build/header files.

my $WRITE_CPP_FILE = 1;   # extract/write the spl.cpp file?
my $WRITE_H_FILE = 1;     # extract/write the spl.h   file?

my $outfile = "spl.cpp";
if ($#ARGV >= 0) {
	$outfile = shift @ARGV;
}
my $out_h_file = $outfile;
$out_h_file =~ s/\.cpp/.h/;   # "stanfordcpplib.h"
my $basedir = "StanfordCPPLib";
my $projdir = "StanfordCPPLib_CodeStepByStep_Project/src";
my $autograder_basedir = "autograder";

my @ALL_FILES = (
# .h headers (order is very important because of dependencies of declarations)
# basic library functionality
"$basedir/private/init.h",                   # deps: none
"$basedir/private/static.h",                 # deps: none
"$basedir/system/error.h",                   # deps: none
"$basedir/util/require.h",
"$basedir/private/headless.h",
# "$basedir/util/memory.h",
"$basedir/io/base64.h",                      # deps: none
"$basedir/util/direction.h",                 # deps: none
"$basedir/collections/hashcode.h",           # deps: none
"$basedir/util/random.h",                    # deps: none
"$basedir/util/point.h",                     # deps: none
"$basedir/io/simpio.h",                      # deps: none
# "$basedir/graphics/gtypes.h",              # deps: none
"$basedir/collections/functional.h",         # deps: none
"$basedir/util/gmath.h",                     # deps: gtypes.h
"$basedir/collections/shuffle.h",            # deps: random.h
"$basedir/collections/collections.h",        # deps: none
"$basedir/util/observable.h",                # deps: error.h
"$basedir/util/recursion.h",
"$basedir/system/exceptions.h",
"$basedir/collections/vector.h",
"$basedir/util/strlib.h",                    # deps: none
"$basedir/util/stringutils.h",               # deps: none
"$basedir/util/intrange.h",                  # deps: error, Point
"$basedir/io/filelib.h",                     # deps: vector.h
"$basedir/collections/deque.h",
"$basedir/collections/queue.h",
"$basedir/collections/stack.h",
# "$basedir/private/tokenpatch.h",
"$basedir/io/tokenscanner.h",
"$basedir/collections/map.h",
"$basedir/collections/set.h",
"$basedir/collections/hashmap.h",
"$basedir/collections/hashset.h",
"$basedir/collections/graph.h",              # deps: map, set, tokenscanner
"$basedir/collections/gridlocation.h",
"$basedir/collections/grid.h",
"$basedir/collections/lexicon.h",
"$basedir/collections/linkedlist.h",
"$basedir/collections/priorityqueue.h",
"$basedir/collections/pqueue.h",

"$basedir/collections/sparsegrid.h",
"$basedir/collections/dawglexicon.h",        # deps: set.h
"$basedir/collections/linkedhashmap.h",      
"$basedir/collections/linkedhashset.h",      # deps: linkedhashmap.h
"$basedir/collections/basicgraph.h",         # deps: map, set, graph, many other collections
"$basedir/collections/stl.h",

"$basedir/graphics/consoletext.h",
"$basedir/graphics/console.h",

# "$basedir/graphics/gwindow.h",
# "$basedir/graphics/gobjects.h",            # deps: gtypes, gwindow
# "$basedir/graphics/gtimer.h",
# "$basedir/graphics/gevents.h",
# "$basedir/graphics/ginteractors.h",
# "$basedir/graphics/gbufferedimage.h",
# "$basedir/graphics/gfilechooser.h",
# "$basedir/graphics/goptionpane.h",
# "$basedir/graphics/gtable.h",
# "$basedir/graphics/gtextarea.h",
 
"$basedir/io/bitstream.h",
"$basedir/io/plainconsole.h",
# "$basedir/util/sound.h",
"$basedir/util/complex.h",
"$basedir/util/biginteger.h",
"$basedir/util/bigfloat.h",
# "$basedir/private/consolestreambuf.h",
# "$basedir/private/echoinputstreambuf.h",
# "$basedir/private/forwardingstreambuf.h",
# "$basedir/private/limitoutputstreambuf.h",
# "$basedir/private/platform.h",
# "$basedir/private/tplatform.h",
"$basedir/private/version.h",
"$basedir/util/regexpr.h",
"$basedir/system/call_stack.h",
# "$basedir/system/stack_exception.h",
# "$basedir/system/thread.h",
# "$basedir/system/pstream.h",
# "$basedir/system/process.h",
"$basedir/util/timer.h",
"$basedir/util/note.h",
# "$basedir/util/managed.h",
# "$basedir/io/urlstream.h",

# .cpp implementations (order should be unimportant)
# "$basedir/util/range.cpp",
"$basedir/private/init.cpp",

"$basedir/collections/collections.cpp",
"$basedir/collections/basicgraph.cpp",
"$basedir/collections/dawglexicon.cpp",
"$basedir/collections/hashcode.cpp",
"$basedir/collections/lexicon.cpp",
"$basedir/collections/shuffle.cpp",
"$basedir/collections/gridlocation.cpp",

# "$basedir/graphics/gbufferedimage.cpp",
# "$basedir/graphics/gevents.cpp",
# "$basedir/graphics/gfilechooser.cpp",
# "$basedir/graphics/ginteractors.cpp",
# "$basedir/graphics/gobjects.cpp",
# "$basedir/graphics/goptionpane.cpp",
# "$basedir/graphics/gtable.cpp",
# "$basedir/graphics/gtextarea.cpp",
# "$basedir/graphics/gtimer.cpp",
# "$basedir/graphics/gtypes.cpp",
# "$basedir/graphics/gwindow.cpp",
 
"$basedir/io/base64.cpp",
"$basedir/io/bitstream.cpp",
"$basedir/io/filelib.cpp",
"$basedir/private/filelibunix.cpp",
"$basedir/io/simpio.cpp",
"$basedir/io/tokenscanner.cpp",
# "$basedir/io/urlstream.cpp",
# "$basedir/private/platform.cpp",
"$basedir/private/version.cpp",
# "$basedir/private/tplatform_posix.cpp",
"$basedir/system/call_stack_gcc.cpp",
# "$basedir/system/thread.cpp",
#"$basedir/system/call_stack_windows.cpp",   # not needed because the server runs *nix
"$basedir/system/error.cpp",
"$basedir/io/plainconsole.cpp",
"$basedir/system/exceptions.cpp",
# "$basedir/system/process.cpp",
"$basedir/util/recursion.cpp",
"$basedir/util/direction.cpp",
"$basedir/util/gmath.cpp",
# "$basedir/util/observable.cpp",
"$basedir/util/point.cpp",
"$basedir/util/random.cpp",
"$basedir/util/regexpr.cpp",
# "$basedir/util/sound.cpp",
"$basedir/util/strlib.cpp",
"$basedir/util/timer.cpp",
# "$basedir/util/memory.cpp",
"$basedir/graphics/console.cpp",

# some classes that come from practice exams but are needed for 106B/X section/exam problems
"$projdir/ArrayIntList.h",
"$projdir/BinaryTreeNode.h",
"$projdir/BinaryTree.h",
"$projdir/BinaryTreeMapNode.h",
"$projdir/BinaryTreeMap.h",
"$projdir/HashTableMap.h",
"$projdir/HashTableSet.h",
"$projdir/HeapPriorityQueue.h",
"$projdir/LinkedIntList.h",
"$projdir/graphsupport.h",
"$projdir/types.h",

"$projdir/ArrayIntList.cpp",
"$projdir/BinaryTree.cpp",
"$projdir/BinaryTreeMap.cpp",
"$projdir/HashTableSet.cpp",
"$projdir/HeapPriorityQueue.cpp",
"$projdir/LinkedIntList.cpp",
"$projdir/graphsupport.cpp",
"$projdir/types.cpp",

# csbs-specific
"$projdir/codestepbystep.h",
"$projdir/codestepbystep.cpp",
);


# "main"

print("C++ library merger tool\n");
print("by Marty Stepp\n");
print("=======================\n");

# gather the C++ code text
print("Reading source code ...\n");

my $author = "Marty Stepp";
my $nowdate = `date`;   # e.g. "Fri Oct 14 10:52:02 PDT 2016"
chomp($nowdate);

my $overall_shared_comment_header = 
	"// Stanford C++ library (extracted)\n"
	. "// \@author ${author}\n"
	. "// \@version ${nowdate}\n"
	. "//\n"
	. "// This library has been merged into a single .h and .cpp file by an automatic script\n"
	. "// to make it easier to include and use with the CodeStepByStep tool.\n"
	. "// DO NOT EDIT THIS FILE DIRECTLY!\n"
	. "// If you want to make changes or additions to the Stanford C++ library,\n"
	. "// make them to the library's original source as separate .cpp / .h files,\n"
	. "// then re-run the script to extract the library into these single large merged files.\n\n";

my $overall_cpp_text = $overall_shared_comment_header
	# . "using namespace std;\n"
	. "#include \"$out_h_file\"\n"
	. "\n";
my $overall_h_text = $overall_shared_comment_header
	. "#define STANFORD_CPP_LIB_PRESENT true\n\n";
$overall_h_text .= "using namespace std;\n\n";
foreach my $cppfile (@ALL_FILES) {
	print("  - $cppfile\n");
	if (not -f $cppfile) {
		die("ERROR: required file not found: $cppfile\n");
	}
	
	$cppfiletext = `cat $cppfile`;
	
	# clean up the code a bit;
	# remove local includes from within lib
	$cppfiletext =~ s/#include[ \t]{0,99}"[^"]{1,99}".{0,99}\n/\n/gi;
	
	# remove redeclaration of, static Platform *pp = getPlatform();
	# $cppfiletext = regex_remove_lines($cppfiletext, "static[ ]*Platform[ *]+pp[ ]*=[ ]*getPlatform();[ ]*");
	
	$text_to_add = "/////////////////////// BEGIN code extracted from $cppfile ///////////////////////\n"
			. $cppfiletext . "\n"
			. "/////////////////////// END code extracted from $cppfile ///////////////////////\n\n";
	
	if ($cppfile =~ m/\.h$/) {
		# special case: including console.h, must disable graphical console
		if ($cppfile =~ m/\/console\.h$/) {
			$overall_h_text .= "#define __DONT_ENABLE_GRAPHICAL_CONSOLE\n";
		}

		$overall_h_text .= $text_to_add;

		if ($cppfile =~ m/\/console\.h$/) {
			$overall_h_text .= "#undef __DONT_ENABLE_GRAPHICAL_CONSOLE\n";
		}
		
	} else {
		$overall_cpp_text .= $text_to_add;
	}
}

if ($WRITE_CPP_FILE) {
	print("Writing output file $outfile ...\n");
	open(OUTFILE, ">", $outfile) or die "Cannot open $outfile for writing: $!";
	print OUTFILE $overall_cpp_text;
	close OUTFILE;
}

if ($WRITE_H_FILE) {
	print("Writing header file $out_h_file ...\n");
	open(OUTFILE, ">", $out_h_file) or die "Cannot open $out_h_file for writing: $!";
	print OUTFILE $overall_h_text;
	close OUTFILE;
}

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
