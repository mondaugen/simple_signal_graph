# Generate structure allocation code.

my $is = 0;
my @d;
my $struct_name;
while (<>) {
    if (($n) = $_ =~ m< */\*-- +DSTRUCT +(\S+) +--\*/ *>) {
        $struct_name = $n =~ s/_t$//r;
        print "In the struct now\n";
        $is = 1; next; 
    }
    if ($_ =~ m( */\*-- DSTRUCT --\*/ *)) {
        print "name: $struct_name\n";
        $is = 0; next; 
    }
    if ($is) {
        $mch=qr/(\w+) \*(\**\w+[^_])(_?) *; *\/{0,2} *(\S*)/;
        if (($t,$n,$a,$s) = $_ =~ /$mch/) {
            print "t: $t n: $n s: $s a: $a\n";
            push @d, [$n,$t,$s,$a];
        }
    }
}

print "struct {\n";
foreach(@d){
    ($n,$t,$s,$a)=@$_;
    print " $t *$n;\n";
    if ($a) {
        print " size_t ${n}_sz;\n";
    }
}
print "} ${struct_name}_t;\n";

print "\n";

print "size_t\n";
print "${struct_name}_sz(";
foreach(@d){
    ($n,$t,$s,$a)=@$_;
    if ($a) {
        print " size_t ${n}_sz,\n";
    }
}
print "\b\b)\n";
print "{";
print " return ";
foreach(@d){
    ($n,$t,$s,$a)=@$_;
    if ($a) {
        print " ${n}_sz * ";
        if ($s) {
            print "$s";
        } else {
            print "sizeof($t)";
        }
        print " +\n";
    }
}
print "\b\b;\n";
    
