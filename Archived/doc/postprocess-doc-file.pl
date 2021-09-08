#!/usr/bin/perl -w

my $filename = shift @ARGV or die("Usage: postprocess-doc-file.pl FILENAME");
my $outfile = $filename;
if ($#ARGV >= 0) {
	$outfile = shift @ARGV;
}

my $filetext = `cat $filename`;
my $output = "";

foreach my $line (split(/\r?\n/, $filetext)) {
	$line =~ s/const std::string &amp;/string /gi;
	$line =~ s/QWidget [*]/QWidget* /gi;
	$line =~ s/std::string/string/gi;
	$line =~ s/Q_DECL_OVERRIDE//gi;
	$line =~ s/Q_DECL_DEPRECATED/\<span class=\"deprecated\"\>\(deprecated\)\<\/span\>/gi;
	$line =~ s/(<tr class=\"memitem.*>)virtual(?:&#160;| )?/$1/gi;
	$line =~ s/(<tr class=\"memitem.*<\/a>) \(/$1(/gi;
	
	$line =~ s/(\<p\>\<code\>#include )&lt;(.*)&gt;/$1"$2"/;
	$output .= "$line\n";
}

open OUTPUT, ">", $outfile or die("Can't open $outfile for writing: $!");
print OUTPUT $output;
close OUTPUT;
