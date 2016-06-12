
PHP_ARG_ENABLE(site_builder, whether to enable site_builder support,
Make sure that the comment is aligned:
[  --enable-site_builder           Enable site_builder support])

dnl PHP_ADD_INCLUDE(/include)
PHP_ADD_LIBRARY_WITH_PATH(uuid, $SITE_BUILDER_DIR, SITE_BUILDER_SHARED_LIBADD)
PHP_SUBST(SITE_BUILDER_SHARED_LIBADD)

if test "$PHP_SITE_BUILDER" != "no"; then
	PHP_NEW_EXTENSION(site_builder,
	lg_list.c	\
	async.c		\
	hiredis.c	\
	lg_redis.c	\
	net.c		\
	sds.c		\
	site_builder.c, $ext_shared)
fi

