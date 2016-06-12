
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "license_15talk.h"

#ifdef	WIN32
#include <windows.h>
#endif

static char *buffer = NULL;
static int rand_number = 147258;

void license_15talk_printf(JNIEnv *env, char *format, ...)
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

#if TEST
    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/config/StringUtil");
#else
    jclass cls = (*env)->FindClass(env, "StringUtil");
#endif
    if (cls == NULL) {

	license_15talk_thrown(env, "print class err!");
	return;
    }

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "printf", "(Ljava/lang/String;)V");
    if (method == NULL) {

	license_15talk_thrown(env, "print method err!");
	return;
    }

    (*env)->CallStaticVoidMethod(env, cls, method, (*env)->NewStringUTF(env, buffer));
}


//传入licence路径  返回加密后信息
#if TEST
    JNIEXPORT jbyteArray JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1start
(JNIEnv *env, jobject obj, jstring licence_path)
#else
    JNIEXPORT jbyteArray JNICALL Java_StringUtil_init_1start
(JNIEnv *env, jobject obj, jstring licence_path)
#endif
{
    const char *licence_path_str = (*env)->GetStringUTFChars(env, licence_path, 0);
    if (licence_path_str == NULL) {

	license_15talk_thrown(env, "licence path err!");
	return (*env)->NewStringUTF(env, "err");
    }

    if (buffer == NULL) {

	if (!(buffer = malloc(4096 + 1024))) {

	    license_15talk_thrown(env, "malloc err!");
	    return (*env)->NewStringUTF(env, "err"); 
	}
    }

    srand(time(NULL));
    rand_number = rand() % 88888888;
    jsize size = sprintf(buffer, "%08u", rand_number);

    license_15talk_drivers_cpuid(&buffer[8]);
    license_15talk_drivers_mac_addr(&buffer[43]);
    if (!(size = license_15talk_drivers_licence(licence_path_str, &buffer[60]))) {

	license_15talk_thrown(env, "read licence err!");
	return (*env)->NewStringUTF(env, "err");
    }

    size += 61;
    buffer[size] = 0;

    license_15talk_key_coding(buffer, size);
    jbyteArray buffer_byte = (*env)->NewByteArray(env, size);
    (*env)->SetByteArrayRegion(env, buffer_byte, 0, size, (jbyte*)buffer);
    return buffer_byte;
    //return (*env)->NewStringUTF(env, buffer);
}


    JNIEXPORT jstring JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1end
(JNIEnv *env, jobject obj, jstring str)
{

    return 0;
}

//传入服务器返回结果 并启动服务
#if TEST
    JNIEXPORT void JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1callback
(JNIEnv *env, jobject obj, jbyteArray buffer_byte)
#else
    JNIEXPORT void JNICALL Java_StringUtil_init_1callback
(JNIEnv *env, jobject obj, jbyteArray buffer_byte)
#endif
{

    char *pbuffer_byte = (char *) (*env)->GetByteArrayElements(env, buffer_byte, 0);
    size_t pbuffer_size = (*env)->GetArrayLength(env, buffer_byte);
    if (!pbuffer_byte || !pbuffer_size) {

	license_15talk_thrown(env, "read packet err!");
	return;
    }

    pbuffer_byte[pbuffer_size] = 0;
#if TEST
    license_15talk_key_decoding(pbuffer_byte, pbuffer_size);
#endif
    
#if DEBUG
    license_15talk_printf(env, "[%d]%s", rand_number, pbuffer_byte);
#endif

    //校验返回随机数
    char rand_number_str[10] = {0};
    memcpy(rand_number_str, pbuffer_byte, 8);
    if (atoi(rand_number_str) != rand_number) {

	license_15talk_thrown(env, "rand number err!");
	return;
    }

    license_15talk_run(env, &pbuffer_byte[8], pbuffer_size);
    license_15talk_complate(env);
}

    JNIEXPORT jstring JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1kill
(JNIEnv *env, jobject obj, jstring str)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_cmp
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_String
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_token
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_clear
(JNIEnv *env, jobject obj, jstring mode, jstring licence_path)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_findAll
(JNIEnv *env, jobject obj)
{

    return 0;
}



