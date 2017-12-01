#include <stdio.h>
#include <string.h>

static char *
_compress_store_var(
        char *str, /* String to "compress" */
        /* The number of characters to discard before variable name, e.g., for
           ${varname} this is 2 */
        size_t lead, 
        /* The number of characters to discard before variable name, e.g., for
           ${varname} this is 1. Should be positive (not 0). */
        size_t tail,
        /* The offset from the beginning of str of the
           first character of the whole variable string, e.g., for abc${varname}
           the offset is 3.
         */
        size_t off,
        /* The entire variable length including lead
           and tail. */
        size_t varlen)
{
    size_t innerlen = varlen - lead - tail;
    char buf[innerlen];
    char *ret = str + strlen(str) - innerlen;
    memcpy(buf,str+off+lead,innerlen);
    /* +1 to include \0 */
    memmove(str+off,str+off+varlen,strlen(str+off+varlen)+1);
    memcpy(ret,buf,innerlen);
    return ret;
}

void
print_force(char *str, size_t len)
{
    while (len--) {
        if (*str == '\0') {
            putc('*',stdout);
        } else {
            putc(*str,stdout);
        }
        str++;
    }
    putc('\n',stdout);
}

int
main(void)
{
    char *_str = "abc${xyz}o${x}y";
    size_t slen = strlen(_str);
    char str[slen+1];
    memcpy(str,_str,slen);
    str[slen]='\0';
    int offs[] = {3,4,-1};
    int vlens[] = {6,4};
    int *po = offs;
    int *pv = vlens;
    printf("original: %s\n",str);
    while (*po >= 0) {
        char *c;
        c = _compress_store_var(str,2,1,*po++,*pv++);
        printf("%s: ",c);
        print_force(str,slen);
    }
    return 0;
}
