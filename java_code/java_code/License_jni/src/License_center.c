
#include "License.h"

int com_sinotawa_framework_util_errorenum_init(JNIEnv *env)
{
    //fprintf(stdout, "com_sinotawa_framework_util_errorenum_init");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/util/ErrorEnum");
    if ((*env)->ExceptionCheck(env))
	return -11;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "init", "()V");
    if ((*env)->ExceptionCheck(env))
	return -12;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -13;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;
}

int com_sinotawa_framework_event_eventmanager_starteventmanager(JNIEnv *env)
{

    //fprintf(stdout, "com_sinotawa_framework_event_eventmanager_starteventmanager");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/event/EventManager");
    if ((*env)->ExceptionCheck(env))
	return -21;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "startEventManager", "()V");
    if ((*env)->ExceptionCheck(env))
	return -22;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -23;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;

}

int com_sinotawa_framework_event_eventthreadpool_init(JNIEnv *env)
{

    //fprintf(stdout, "com_sinotawa_framework_event_eventthreadpool_init");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/framework/event/EventThreadPool");
    if ((*env)->ExceptionCheck(env))
	return -31;

    //fprintf(stdout, "FindClass");

    jmethodID getInstance = (*env)->GetStaticMethodID(env, cls, "getInstance", "()Lcom/sinotawa/framework/event/EventThreadPool;");
    if ((*env)->ExceptionCheck(env))
	return -32;

    //fprintf(stdout, "GetStaticMethodID");

    jobject obj = (*env)->CallStaticObjectMethod(env, cls, getInstance);
    if ((*env)->ExceptionCheck(env))
	return -33;

    //fprintf(stdout, "CallStaticObjectMethod");

    jmethodID init = (*env)->GetMethodID(env, cls, "init", "()V");
    if ((*env)->ExceptionCheck(env))
	return -32;

    //fprintf(stdout, "GetMethodID");

    (*env)->CallVoidMethod(env, obj, init);
    if ((*env)->ExceptionCheck(env))
	return -34;

    //fprintf(stdout, "CallVoidMethod");
    return 0;
}

int com_sinotawa_common_context_configcontext_init(JNIEnv *env)
{
    //fprintf(stdout, "com_sinotawa_common_context_configcontext_init");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/common/context/ConfigContext");
    if ((*env)->ExceptionCheck(env))
	return -41;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "init", "()V");
    if ((*env)->ExceptionCheck(env))
	return -42;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -43;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;
}

int com_sinotawa_message_framework_config_messagecontext_initconnections(JNIEnv *env)
{
    //fprintf(stdout, "com_sinotawa_message_framework_config_messagecontext_initconnections");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/message/framework/config/MessageContext");
    if ((*env)->ExceptionCheck(env))
	return -51;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "initConnections", "()V");
    if ((*env)->ExceptionCheck(env))
	return -52;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -53;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;
}

int com_sinotawa_jndi_common_memercontext_init(JNIEnv *env)
{
    //fprintf(stdout, "com_sinotawa_jndi_common_memercontext_init");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/jndi/common/MemerContext");
    if ((*env)->ExceptionCheck(env))
	return -61;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "init", "()V");
    if ((*env)->ExceptionCheck(env))
	return -62;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -63;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;
}

int com_sinotawa_socket_jms_socketjmscontext_init(JNIEnv *env)
{

    //fprintf(stdout, "com_sinotawa_socket_jms_socketjmscontext_init");

    jclass cls = (*env)->FindClass(env, "com/sinotawa/socket/jms/SocketJMSContext");
    if ((*env)->ExceptionCheck(env))
	return -71;

    //fprintf(stdout, "FindClass");

    jmethodID method = (*env)->GetStaticMethodID(env, cls, "init", "()V");
    if ((*env)->ExceptionCheck(env))
	return -72;

    //fprintf(stdout, "GetStaticMethodID");

    (*env)->CallStaticVoidMethod(env, cls, method);
    if ((*env)->ExceptionCheck(env))
	return -73;

    //fprintf(stdout, "CallStaticVoidMethod");
    return 0;
}



