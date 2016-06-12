#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GetStringUTFChars jstring转UTF-8格式的char*
// ReleaseStringUTFChars 释放UTF-8格式的char*
// NewStringUTF 创建UTF-8格式的String对象
// GetStringUTFLength 获取UTF-8格式的char*长度

// GetStringChars jstring转Unicode格式的char*
// ReleaseStringChars 释放Unicode格式的char*
// NewString 创建Unicode格式的String对象
// GetStringLength 获取Unicode格式的char*长度

static char *strbuffer = NULL;
static void *bitbuffer = NULL;

JNIEXPORT void JNICALL Java_demo_SetString(JNIEnv *env, jobject obj, jstring str)
{

    const char *temp = (*env)->GetStringUTFChars(env, str, 0);
    int size = (*env)->GetStringUTFLength(env, str);

    strbuffer = malloc(size + 1);
    memcpy(strbuffer, temp, size);
    strbuffer[size] = 0;

    (*env)->ReleaseStringUTFChars(env, str, temp);
}

JNIEXPORT jstring JNICALL Java_demo_GetString(JNIEnv *env, jobject obj)
{

    return (*env)->NewStringUTF(env, strbuffer);
}

JNIEXPORT jbyteArray JNICALL Java_demo_SetBytes(JNIEnv *env, jobject obj, jbyteArray buffer, jint len)
{

    const char *temp = (*env)->GetByteArrayElements(env, buffer, 0);
    fprintf(stdout, "%s\n", temp);
}


