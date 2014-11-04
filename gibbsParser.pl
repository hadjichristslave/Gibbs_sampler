use strict;
use warnings;

`rm output.txt`;
`rm report*.txt`;
`./GibbsSampler > output.txt`;
my $file = 'output.txt';
open my $info, $file or die "Could not open $file: $!";
my $counter = 0;
my $mu      = 0;
my $filename;

while( my $line = <$info>){
    if ($line eq "--------\n"){
	print "counter increased \n";
	$mu = 0;
	$counter = $counter+1;
    }elsif($line eq "++++++++\n"){
	$mu = 1;
    }else{
	if($mu==0){
	        $filename = 'report'.$counter.'.txt';
	}else{
		$filename = "mu".$counter.".txt";
	}
        open(my $fh, '>>', $filename) or die "Could not open file '$filename' $!";
        print $fh "$line";
        close $fh;
    }
}

close $info;
`rm output.txt`;
`octave mystats.m`;
