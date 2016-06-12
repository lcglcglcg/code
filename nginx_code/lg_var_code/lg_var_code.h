
#ifndef __LG_VAR_CODE_H__
#define __LG_VAR_CODE_H__


#ifdef __cplusplus
extern "C" { 
#endif

    typedef struct lg_var_code_t {

	char key[5];
	char *buffer;
	int size;

    } lg_var_code_t;

    extern int lg_var_code_init(char *symbols);
    extern int lg_var_code_create(lg_var_code_t *p);


#ifdef __cplusplus
}
#endif
#endif


