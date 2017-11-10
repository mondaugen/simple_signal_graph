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
        $mch=qr/([\w ]+ \**)\*(\w+[^_])(_?) *; *\/{0,2} *(\S*)/;
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

$str="";
$str.="struct {\n";
foreach(@d){
    ($n,$t,$s,$a)=@$_;
    if ($a) {
        $str.=" size_t ${n}_sz;\n";
    }
}
$str.="} ${struct_name}_init_t;\n\n";

$str.="size_t\n";
$str.="${struct_name}_sz(${struct_name}_init_t *si)\n";
$str.="{\n";
$str.=" return ";
foreach(@d){
    ($n,$t,$s,$a)=@$_;
    if ($a) {
        $str.=" si->${n}_sz * ";
        $str.=" sizeof($t)";
        $str.=" +\n";
        if ($s) {
            $str.=" si->${n}_sz * ";
            $str.="$s";
            $str.=" +\n";
        }
    } else {
        if ($s) {
            $str.=" sizeof($t*) +\n";
            $str.=" $s +\n";
        }
    }
}
$str=substr($str,0,-2).";\n";
$str.="}\n";
print$str;
    
