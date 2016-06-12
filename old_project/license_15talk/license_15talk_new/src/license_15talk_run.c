
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

void license_15talk_thrown(JNIEnv *env, char *buffer)
{

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

	license_15talk_thrown(env, "code from c err!");
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, method_str, "()V");
    if (method == NULL) {

	license_15talk_thrown(env, "code from c err!");
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

#if DEBUG
    license_15talk_printf(env, "%s->%s", cls_str, method_str);
#endif
    license_15talk_run_call(env, cls_str, method_str);
}

void license_15talk_run(JNIEnv *env, char *buffer, size_t size)
{

    char *saveptr = NULL;
    char *str = lg_strtok_r(buffer, '|',  &saveptr);
    while(str) {

#if DEBUG
	license_15talk_printf(env, str);
#endif
	license_15talk_run_parse(env, str);
	str = lg_strtok_r(NULL, '|',  &saveptr);
    }
}

void license_15talk_complate(JNIEnv *env)
{

#if TEST
    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/config/StringUtil");
#else
    jclass cls = (*env)->FindClass(env, "StringUtil");
#endif
    if (cls == NULL) {

	license_15talk_thrown(env, "code from c err!");
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "complate", "()V");
    if (method == NULL) {

	license_15talk_thrown(env, "code from c err!");
	return;
    }

    (*env)->CallStaticVoidMethod(env, cls, method);
}


