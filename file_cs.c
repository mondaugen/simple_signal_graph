#include <stdlib.h>
#include <stdio.h>
#include "file_cs.h"

struct file_cs_t {
    char_stream_t super;
    FILE *f;
};

static cs_int_t
_get_chr(char_stream_t *cs, char *pch)
{
    file_cs_t *fcs = (file_cs_t*)cs;
    char tmp;
    if ((tmp = fgetc(fcs->f)) == EOF) {
        return 1;
    }
    *pch = tmp;
    return 0;
}

static char_stream_vt_t _file_cs_vt = {
    .get_chr = _get_chr,
};

file_cs_t *
file_cs_new_from_path(char *path)
{
    FILE *f = fopen(path,"r");
    if (!f) {
        return NULL;
    }
    file_cs_t *ret = calloc(1,sizeof(file_cs_t));
    if (!ret) {
        return NULL;
    }
    ((char_stream_t*)ret)->_vt = &_file_cs_vt;
    ret->f = f;
    return ret;
}

void
file_cs_free(file_cs_t *fc)
{
    if (fc->f) {
        fclose(fc->f);
    }
    free(fc);
}
