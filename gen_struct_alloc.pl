my$in_struct=1;
my@fields;
my$struct_def;
my$init_struct;
my$szfun;
my$alloc_fun;
while(<>){
 if($in_struct){
  if(!/^ *#/){
   $n=($type,$fld_name,$flag,$szof,$itype)=/(\w+) */g;
   if($n<=1){next;}
   if(!$flag){next;}
   # Start strings
#     ($fld_name)=($fld_name=~/(.+?)(_t|)$/);
   given($flag){
    when("a"){
     # array, each item fixed size determined by sizeof
     $struct_def.="$type *${fld_name};\n";
     $struct_def.="size_t ${fld_name}_sz;\n";
     $init_struct.="size_t ${fld_name}_sz;\n";
     if(!$szof){$szof="sizeof($type)"}
     $szfun.="sz += si->${fld_name}_sz * $szof;";
     # memory initialization
     $alloc_fun.=<<"END";
ret->$fld_name = ($type*)ptr;
ptr += si->${fld_name}_sz * $szof;
ret->${field_name}_sz = si->${fld_name}_sz;
END
    }
    when("d"){
     # array, each item dynamic size determined by 
     # type_sz(&si->name). Remember to allocate both array of pointers to
     # structures and the structures themselves
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
    }
    when("s"){
     # single item but with dynamic size
     # no need to allocate array of pointers, just structure whose address
     # stored in this field.
     $struct_def.=" $type *${fld_name};";
     if(!$itype){$itype="${fld_name}_meminit_t";}
     $init_struct.="$itype $fld_name;";
     if(!$szof){$szof="${_type}_sz(&si->$fld_name)";}
     $szfun.="sz += si->${fld_name}_sz * $szof;";
     $alloc_fun.=<<"END";
ret->${fld_name} = ($type*)ptr;
ptr += $szof;
END
    }
   }
  }elsif(/^\s*\}\s*; *$/){
   $struct_def.="} ${struct_name}_t;\n";
   $init_struct.="} ${struct_name}_meminit_t;\n";
   $szfun.="return sz;\n}\n";
   $alloc_fun.="return ret;\n}\n";
   print"$struct_def\n$init_struct\n$szfun\n$alloc_fun\n";
   $in_struct=0;
  }
 }
 else{
  if(($struct_name)=$_=~/(\w+?)(?:_t|)\s*\{\s*$/){
   $in_struct=1;
   $struct_def="struct {\n";
   $init_struct="struct {\n";

   $szfun=<<"END";
size_t
${struct_name}_sz(${struct_name}_meminit_t *si)
{
 size_t sz = 0;
END

   $alloc_fun=<<"END"
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
  }
 }
}
