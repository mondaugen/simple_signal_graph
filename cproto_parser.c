/* cproto_parser.c */
#include <stdio.h>                           
int main (void)                              
{                                            
    int c;                                   
    int infundef = 0;                        
    int nb = 0,                              
        np = 0;                              
    while((c=getc(stdin))!=EOF){             
        if(c=='{'){                          
            if((np==0)&&(nb==0)){infundef=1;}
            nb++;                            
        }                                    
        if (infundef==0) {putc(c,stdout);}   
        if(c=='}'){                          
            if((np==0)&&(nb==1)){infundef=0;}
            nb--;                            
        }                                    
        if(c=='('){np++;}                    
        if(c==')'){np--;}                    
    }                                        
    return 0;                                
}     
