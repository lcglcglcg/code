
#include <jni.h>

#ifndef _Included_app_daemon
#define _Included_app_daemon

#ifdef __cplusplus
extern "C" {
#endif

    extern void app_daemon_process();
    JNIEXPORT void JNICALL Java_app_1daemon_run(JNIEnv *env, jclass cls);

#ifdef __cplusplus
}
#endif
#endif

