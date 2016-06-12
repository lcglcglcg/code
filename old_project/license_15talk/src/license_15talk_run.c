
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "license_15talk.h"

char *lg_strtok_r(char *str, char delim, char **saveptr)
{

    if (saveptr && *saveptr)
	str = *saveptr;

    if (str == NULL)
	return NULL;

    for (; *str; str++)
	if (*str != delim)
	    break;

    char *p = NULL;
    for (p = str; *p; p++) {

	if (*p == delim) {

	    *p = 0;
	    for (p++; *p; p++) {
		if (*p != delim) {

		    *saveptr = p;
		    return str;
		}
	    }
	}
    }

    if (str || *saveptr) {

	*saveptr = NULL;
	return str;
    }   

    return str;
}

void license_15talk_thrown(JNIEnv *env, char *format, ...)
{

    va_list arg_list;
    va_start(arg_list, format);

    size_t size = 0;
    char buffer[4096] = {0};

    for( ; *format; format++) {

	if (*format == '%') {

	    format++;
	    if (!(*format))
		return;

	    if (*format == 'd') {

		int value = va_arg(arg_list, int);
		size += sprintf(&buffer[size], "%d", value);

	    } else if (*format == 's') {

		char *value = va_arg(arg_list, char *);
		size += sprintf(&buffer[size], "%s", value);
	    }

	} else {

	    buffer[size++] = *format;
	}
    }

#ifdef WIN32
    (*env)->ExceptionDescribe(env);
#endif
    (*env)->ExceptionClear(env);

    jclass newExcCls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (newExcCls == NULL)
	return;

    (*env)->ThrowNew(env, newExcCls, buffer);
}

void license_15talk_run_call(JNIEnv *env, char *cls_str, char *method_str)
{

    jclass cls = (*env)->FindClass(env, cls_str);
    if (cls == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "FindClass %s err!", cls_str);
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, method_str, "()V");
    if (method == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "GetStaticMethodID %s err!", method_str);
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    (*env)->CallStaticVoidMethod(env, cls, method);
}

void license_15talk_run_parse(JNIEnv *env, char *buffer)
{

    char *saveptr = NULL;
    char *cls_str = lg_strtok_r(buffer, ',',  &saveptr);
    char *method_str = lg_strtok_r(NULL, ',',  &saveptr);
    if (lg_strtok_r(NULL, ',',  &saveptr)) {

	license_15talk_thrown(env, "parse packet err!");
	return;
    }

    if (!cls_str || !method_str) {

	license_15talk_thrown(env, "parse packet err!");
	return;
    }

    license_15talk_run_call(env, cls_str, method_str);
}

void license_15talk_run(JNIEnv *env, char *buffer, size_t size)
{
    license_15talk_thrown(env, buffer);

    char *saveptr = NULL;
    char *str = lg_strtok_r(buffer, '|',  &saveptr);
    while(str) {

	license_15talk_run_parse(env, str);
	str = lg_strtok_r(NULL, '|',  &saveptr);
    }
}

void license_15talk_complate(JNIEnv *env)
{

    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/config/StringUtil");
    if (cls == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "FindClass com/sinotawa/framework/config/StringUtil err!");
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "complate", "()V");
    if (method == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "GetStaticMethodID com/sinotawa/framework/config/StringUtil.complate err!");
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    (*env)->CallStaticVoidMethod(env, cls, method);
}

void license_15talk_check(JNIEnv *env)
{

    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/config/StringUtil");
    if (cls == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "FindClass com/sinotawa/framework/config/StringUtil err!");
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "reload", "()V");
    if (method == NULL) {

#ifdef WIN32
	license_15talk_thrown(env, "GetStaticMethodID com/sinotawa/framework/config/StringUtil.complate err!");
#else
	license_15talk_thrown(env, "code from c err!");
#endif
	return;
    }

    (*env)->CallStaticVoidMethod(env, cls, method);
}

