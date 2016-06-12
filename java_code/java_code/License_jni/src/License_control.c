
#include <stdlib.h>
#include "License.h"

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1start
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1end
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1callback
(JNIEnv *env, jobject obj)
{

    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_init_1kill
(JNIEnv *env, jobject obj)
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

    const char *mode_str = (*env)->GetStringUTFChars(env, mode, 0);
    const char *licence_path_str = (*env)->GetStringUTFChars(env, licence_path, 0);

    if (License_packet_key_check(licence_path_str))
	exit(-1);

    if (strcmp(mode_str, "ONLINE") == 0) {

	com_sinotawa_framework_util_errorenum_init(env);
	com_sinotawa_framework_event_eventmanager_starteventmanager(env);
	com_sinotawa_framework_event_eventthreadpool_init(env);
	com_sinotawa_common_context_configcontext_init(env);
	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);
	com_sinotawa_socket_jms_socketjmscontext_init(env);

    } else if (strcmp(mode_str, "MANAGER") == 0) {

	com_sinotawa_framework_util_errorenum_init(env);
	com_sinotawa_framework_event_eventmanager_starteventmanager(env);
	com_sinotawa_framework_event_eventthreadpool_init(env);
	com_sinotawa_common_context_configcontext_init(env);
	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);

    } else if (strcmp(mode_str, "WEBMANAGER") == 0) {

	com_sinotawa_framework_util_errorenum_init(env);
	com_sinotawa_common_context_configcontext_init(env);
	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);

    } else if (strcmp(mode_str, "JOB") == 0) {

	com_sinotawa_framework_util_errorenum_init(env);
	com_sinotawa_framework_event_eventmanager_starteventmanager(env);
	com_sinotawa_common_context_configcontext_init(env);
	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);
	com_sinotawa_framework_event_eventthreadpool_init(env);

    } else if (strcmp(mode_str, "BACK") == 0) {

	com_sinotawa_framework_util_errorenum_init(env);
	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);

    } else if (strcmp(mode_str, "CAPS") == 0) {

	com_sinotawa_message_framework_config_messagecontext_initconnections(env);
	com_sinotawa_jndi_common_memercontext_init(env);
    }

    (*env)->ReleaseStringUTFChars(env, mode, mode_str);
    (*env)->ReleaseStringUTFChars(env, licence_path, licence_path_str);
    return 0;
}

    JNIEXPORT jint JNICALL Java_com_sinotawa_framework_config_StringUtil_findAll
(JNIEnv *env, jobject obj)
{

    return 0;
}

