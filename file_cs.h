#ifndef FILE_CS_H
#define FILE_CS_H 

#include "char_stream.h"

typedef struct file_cs_t file_cs_t;
file_cs_t *
file_cs_new_from_path(char *path);
void
file_cs_free(file_cs_t *fc);

#endif /* FILE_CS_H */
