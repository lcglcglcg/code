#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "network.h"

    JNIEXPORT jint JNICALL Java_network_Network_1Init
(JNIEnv *env, jobject obj, jstring ipaddr, jint port)
{

    fprintf(stdout, "Network_Init\n");

    int Code;
    char Buffer[1024];
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(8000);
    int listfd = socket(AF_INET, SOCK_STREAM, 0);
    Code = bind(listfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    listen(listfd, 100);

    for (;;) {

	int sockfd = accept(listfd, NULL, NULL);
	Code = read(sockfd, Buffer, 1024);
	Code = write(1, Buffer, Code);
    }

    return 0;
}

    JNIEXPORT jint JNICALL Java_network_Network_1Connect
(JNIEnv *env, jobject obj, jstring jstr, jint port)
{

    fprintf(stdout, "Network_Connect\n");

    char* rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "utf-8");
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);

    if (alen > 0) {

	rtn = (char*)malloc(alen + 1);
	memcpy(rtn, ba, alen);
	rtn[alen] = 0;
    }

    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    fprintf(stdout, "%s\n", rtn);
    fprintf(stdout, "port=%d\n", port);

    return 1024;
}

    JNIEXPORT jint JNICALL Java_network_Network_1Recv
(JNIEnv *env, jobject obj, jstring buffer, jint size)
{

    fprintf(stdout, "Network_Recv\n");
    return 0;
}

    JNIEXPORT jstring JNICALL Java_network_Network_1String
(JNIEnv *env, jobject obj)
{
    char *string = malloc(1024);
    bzero(string, 1024);
    strcpy(string, "hello");
    jclass strClass = (*env)->FindClass(env, "Ljava/lang/String;");
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "", "([BLjava/lang/String;)V");
    jbyteArray bytes = (*env)->NewByteArray(env, strlen(string));
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(string), (jbyte*)string);
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
    return (jstring)(*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

    JNIEXPORT jint JNICALL Java_network_Network_1Send
(JNIEnv *env, jobject obj, jstring buffer, jint size)
{

    fprintf(stdout, "Network_Send\n");
    return 0;
}

int main(int argc, char **argv)
{

    JNIEnv *env;
    jobject obj;
    jstring string;
    jint num = 8000;
    Java_network_Network_1Init(env, obj, string, num);

    return 0;
}

