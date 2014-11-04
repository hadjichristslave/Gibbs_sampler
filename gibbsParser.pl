use strict;
use warnings;

`rm output.txt`;
`rm report*.txt`;
`./GibbsSampler > output.txt`;
my $file = 'output.txt';
open my $info, $file or die "Could not open $file: $!";
my $counter = 0;
while( my $line = <$info>){
    if ($line eq "--------\n"){
	print "counter increased \n";
	$counter = $counter+1;
    }else{
        my $filename = 'report'.$counter.'.txt';
        open(my $fh, '>>', $filename) or die "Could not open file '$filename' $!";
        print $fh "$line";
        close $fh;
    }
}

close $info;
`rm output.txt`;
#`octave mystats.m`;
