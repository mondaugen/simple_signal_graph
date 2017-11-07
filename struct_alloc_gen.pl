# Generate structure allocation code.

my $is = 0;
my %d={};
my $struct_name;
while (<>) {
    if (($n) = $_ =~ m< */\*-- +DSTRUCT +(\S+) +--\*/ *>) {
        $struct_name = $n;
        $is = 1; next; 
    }
    if ($_ =~ m( */\*-- DSTRUCT --\*/ *)) {
        print "name: $struct_name\n";
        $is = 0; next; 
    }
    if ($is) {
        if (($t,$n,$a,$s) = $_ =~ /(\w+) \*(\**\w+[^_])(_?) *; *\/{0,2} *(\S*)/) {
            print "t: $t n: $n s: $s a: $a\n";
            $d{$n} = ($t,$s,$a);
        }
    }
}
