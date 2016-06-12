
#ifndef	__LICENSE_H__
#define	__LICENSE_H__

#include <jni.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern int com_sinotawa_framework_util_errorenum_init(JNIEnv *env);
    extern int com_sinotawa_framework_event_eventmanager_starteventmanager(JNIEnv *env);
    extern int com_sinotawa_framework_event_eventthreadpool_init(JNIEnv *env);
    extern int com_sinotawa_common_context_configcontext_init(JNIEnv *env);
    extern int com_sinotawa_message_framework_config_messagecontext_initconnections(JNIEnv *env);
    extern int com_sinotawa_jndi_common_memercontext_init(JNIEnv *env);
    extern int com_sinotawa_socket_jms_socketjmscontext_init(JNIEnv *env);

    extern int License_packet_key_check(const char *licence_path);
    extern int get_License_drivers_licence(const char *path, char *licence);
    extern int get_License_drivers_mac_addr(char *mac_addr);
    extern int get_License_drivers_cpuid(char *cpu_id_str);
    extern int get_License_drivers_cpuinfo(char *cpuinfo_str);

#ifdef __cplusplus
}
#endif
#endif


