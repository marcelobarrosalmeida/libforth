#ifndef __HW_H__
#define __HW_H__

#ifdef __cplusplus
extern "C" {
#endif

int hw_fgetc ( FILE * stream );
int hw_fputc ( int character, FILE * stream );
FILE * hw_fopen ( const char * filename, const char * mode );
int hw_fget_word ( FILE * stream, char * word, int *n);
int hw_sget_word ( const char * s,  char * word, int *n);
int hw_fputs ( const char * str, FILE * stream );

void hw_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __HW_H__ */

