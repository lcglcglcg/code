
PHP_ARG_ENABLE(code_parse, whether to enable code_parse support,
[  --enable-code_parse           Enable code_parse support])

dnl PHP_ADD_INCLUDE(/usr/local/include/)
dnl PHP_ADD_LIBRARY_WITH_PATH(m, $CODE_PARSE_DIR, CODE_PARSE_SHARED_LIBADD)
dnl PHP_ADD_LIBRARY_WITH_PATH(gd, /usr/local/lib, CODE_PARSE_SHARED_LIBADD)
dnl PHP_ADD_LIBRARY_WITH_PATH(jpeg, $CODE_PARSE_DIR, CODE_PARSE_SHARED_LIBADD)
dnl PHP_SUBST(CODE_PARSE_SHARED_LIBADD)

if test "$PHP_CODE_PARSE" != "no"; then
  PHP_NEW_EXTENSION(code_parse, code_parse.c lg_list.c bmp_code.c font_code.c parse_code.c, $ext_shared)
fi

