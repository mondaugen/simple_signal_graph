my$in_struct=1;
my@fields;
while(<>){
 if($in_struct){
  if(!/^ *#/){
   $n=@line=/(\w+) */g;
    given($n){
     if($n<=1){next;}
     if($n>=2){
      # Interpreted as type and name
      # This field will just pass unchanged to output
      push@fields,[@line,undef,undef,undef];
     }
     if($n>=3){
      $fields[-1][2]=$line[2];
     }
     if($n>=4){
      $fields[-1][3]=$line[3];
     }else{
      $fields[-1][3]
      given($line[2]){
       when("a"){
        # array, each item fixed size determined by sizeof
       }
       when("d"){
        # array, each item dynamic size determined by 
        # type_sz(&si->name). Remember to allocate both array of pointers to
        # structures and the structures themselves
       }
       when("s"){
        # single item but with dynamic size
        # no need to allocate array of pointers, just structure whose address
        # stored in this field.
       }
      }
      






}
}
else{
if(($struct_name)=$_=~/(\w+) \{/){$in_struct=1;next;}
}
}
