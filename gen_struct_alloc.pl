use warnings;

=head1 gen_struct_alloc.pl
Process a file containing structure declarations yielding a structure and
initialization structure (afterwards referred to as "si"), a way to determine the
resulting structure's size and an allocation function.

The structure declaration should have this format

struct_name_t {
 # Comments beginning with # are ignored.
 # Comments must be only thing on line other than whitespace

 # This is just passed unchanged to the output structure
 type1 fieldname1

 # This adds type2 *fieldname2 and size_t fieldname2_sz to the output
 # structure and fieldname2_sz to the initialization structure.
 # The allocation function allocates enough space for an array with elements
 # of type type2 by calling SIZEOF(type2) *
 # si->fieldname2_sz where SIZEOF is custom_sizeof
 # if provided or just C's sizeof if not. custom_sizeof can be any valid C
 # without spaces e.g., MY_CONST_SIZE, etc.
 type2 fieldname2 a [custom_sizeof(...)]

 # This is like a but each element can have a size determined by The
 # SIZE(&si->fieldname3) function which is by default type3_sz. If
 # type3_init_type is not specified, the init type put into struct_name_t's
 # initialization structure is the type called type3_meminit_t. This is so many
 # structures can be nested and will have predictable initialization types. Of
 # course if a custom type is provided, the SIZE function needs to accept this
 # type.  The allocation of the structure in this case is as follows. An array
 # of pointers of size si->fieldname3_sz to elements of type3 is allocated, to
 # which "fieldname3" will point. Then an array of size si->fieldname_sz *
 # SIZE(&si->fieldname3) is allocated to hold the objects of type3 and each
 # element of fieldname3 points to the start of each object.
 type3 fieldname3 d [[custom_sz(&si->fieldname3)] type3_init_type]

 # The last case is almost like d except fieldname4 points to a single instance
 # of an object whose size is determined by SIZE(&si->fieldname4). Note in case
 # fieldname4 is just pointer, not an array of pointers (not a pointer to a
 # pointer). Also no fieldname4_sz field is present in struct_name_t's
 # initialization structure as it would just always contain 1.
 type4 fieldname4 s [[custom_sz(&si->fieldname4)] type4_init_type]

 # The structure must be closed with a line containing only }
}

Processing this results in a file containing definitions of the structure and
its initialization structure, a function "size_t struct_name_sz(struct_meminit_t
*si)" determining the structure's size and a function "struct_name_t *
struct_name_alloc(struct_meminit_t *si)" returning a new allocated structure,
with its fields initialized so that they point to the parts of memory where
their objects are stored. The memory is allocated using a macro CALLOC which is
like that provided by the C standard library. This can be redefined using the
appropriate directives. This structure will most-likely have to be initialized
further to use it in a program so the alloc function should be used as part of a
struct_name_new(...) routine.

The allocated structure can simply be freed using FREE.

TODO: Separate the initialize part of the alloc function so that arbitrary
memory of appropriate size can be initialized.
=cut
 

my$in_struct=0;
my@fields;
my$struct_def;
my$init_struct;
my$szfun;
my$alloc_fun;

sub defs_append {
 ($type,$fld_name) = @_;
 $struct_def.="$type $fld_name;\n";
}

sub defs_append_a {
 ($type,$fld_name,$szof)=@_;
 $struct_def.="$type *${fld_name};\n";
 $struct_def.="size_t ${fld_name}_sz;\n";
 $init_struct.="size_t ${fld_name}_sz;\n";
 if(!$szof){$szof="sizeof($type)";}
 $szfun.="sz += si->${fld_name}_sz * $szof;";
 # memory initialization
 $alloc_fun.=<<"END";
ret->$fld_name = ($type*)ptr;
ptr += si->${fld_name}_sz * $szof;
ret->${fld_name}_sz = si->${fld_name}_sz;
END
 ;
}

sub defs_append_d {
 ($type,$fld_name,$szof,$itype)=@_;
 ($_type)=($type=~/(.+?)(_t|)$/);
 $struct_def.=" $type **${fld_name};";
 $struct_def.=" size_t ${fld_name}_sz;\n";
 $init_struct.=" size_t ${fld_name}_sz;\n";
 if(!$itype){$itype="${fld_name}_meminit_t";}
 $init_struct.="$itype $fld_name;";
 if(!$szof){$szof="${_type}_sz(&si->$fld_name)";}
 # type is a pointer in this case
 $szfun.="sz += si->${fld_name}_sz * sizeof($type*);";
 $szfun.="sz += si->${fld_name}_sz * $szof;";
 $alloc_fun.=<<"END";
ret->${fld_name} = ($type**)ptr;
tmp = ptr;
tmp += si->${fld_name}_sz * sizeof($type**);
for (n = 0; n < si->${fld_name}_sz; n++) {
*($type**)ptr = ($type*)tmp;
ptr = (char*)(($type**)ptr+1);
tmp += $szof;
}
ptr = tmp;
END
 ;
}

sub defs_append_s {
 ($type,$fld_name,$szof,$itype)=@_;
 $struct_def.=" $type *${fld_name};";
 if(!$itype){$itype="${fld_name}_meminit_t";}
 $init_struct.="$itype $fld_name;";
 if(!$szof){$szof="${_type}_sz(&si->$fld_name)";}
 $szfun.="sz += si->${fld_name}_sz * $szof;";
 $alloc_fun.=<<"END";
ret->${fld_name} = ($type*)ptr;
ptr += $szof;
END
 ;
}

sub defs_end {
 $struct_def.="} ${struct_name}_t;\n";
 $init_struct.="} ${struct_name}_meminit_t;\n";
 $szfun.="return sz;\n}\n";
 $alloc_fun.="return ret;\n}\n";
}

sub defs_start {
 $struct_def="struct {\n";
 $init_struct="struct {\n";

 $szfun=<<"END";
size_t
${struct_name}_sz(${struct_name}_meminit_t *si)
{
size_t sz = 0;
END
 ;

 $alloc_fun=<<"END";
${struct_name}_t *
${struct_name}_alloc(${struct_name}_meminit_t *si)
{
size_t sz = ${struct_name}_sz(si);
${struct_name}_t *ret = CALLOC(1,sz);
if (!ret) { return NULL; }
char *ptr = (char*)(ret+1);
char *tmp;
size_t n;
END
 ;
}

while(<>)
{
 if($in_struct)
 {
  if(!/^\s*#/)
  {
   $n=($type,$fld_name,$flag,$szof,$itype)=/(\S+)\s*/g;
   print "$n\n";
   if(/^\}$/)
   #  }elsif(/^\s*\}\s*;* *$/)
   {
    print "out of struct\n"; #D
    &defs_end();
    print"$struct_def\n$init_struct\n$szfun\n$alloc_fun\n";
    $in_struct=0;
   }elsif($n=($type,$fld_name,$flag,$szof,$itype)=/(\S+)\s*/g){
    if($n<=1){next;}
    if(!$flag){ 
     # Simply insert unchanged
     &defs_append($type,$fld_name);
    }
    else 
    {
     ($fld_name)=$fld_name=~/\**(\w+)/;
     if($flag eq "a")
     {
      # array, each item fixed size determined by sizeo
      &defs_append_a($type,$fld_name,$szof);
     }
     elsif($flag eq "d")
     {
      # array, each item dynamic size determined by 
      # type_sz(&si->name). Remember to allocate both array of pointers to
      # structures and the structures themselves
      &defs_append_d($type,$fld_name,$szof,$itype);
     }
     elsif($flag eq "s")
     {
      # single item but with dynamic size
      # no need to allocate array of pointers, just structure whose address
      # stored in this field.
      &defs_append_s($type,$fld_name,$szof,$itype);
     }
     else { print "// Unrecognized flag ${flag}"; }
    }
   }
  }
 }
 else
 {
  if(($struct_name)=$_=~/(\w+?)(?:_t|)\s*\{\s*$/)
  {
   print "in struct\n"; #D

   $in_struct=1;
   &defs_start();
  }
 }
}
