#include <stdio.h>
#include <string.h>
#include <regex.h>

int
main(void)
{
    regex_t re;
    regmatch_t rm;

    char *pat = "\$\{[A-Za-z0-9_][]\[A-Za-z0-9_]*\}";
    //char *pat = "\$\{[A-Za-z0-9_][A-Za-z0-9_\[\]]*\}";
    char *str = "hello ${match69_[21]}abc${[nomatch]} ${no match} ${_420MATCH}";
    //char *str = "hello ${match69_}abc${[nomatch]} ${no match} ${_420MATCH}";
    if(regcomp(&re,pat,0)) {
        fprintf(stderr,"Error compiling.\n");
        return -1;
    }
    char *ptr = str;
    while (1) {
        if(regexec(&re,ptr,1,&rm,0)) {
            fprintf(stderr,"Error matching.\n");
            break;
        }
        size_t len = rm.rm_eo - rm.rm_so;
        char buf[len + 1];
        memcpy(buf,ptr+rm.rm_so,sizeof(char)*len);
        buf[len]='\0';
        printf("%s\n",buf);
        ptr += rm.rm_eo;
    }
    return 0;
}
