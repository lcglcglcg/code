
#ifndef	__CONTROL_CENTER_H__
#define	__CONTROL_CENTER_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern void license_15talk_drivers_cpuid(char *cpu_id);
    extern void license_15talk_drivers_mac_addr(char *mac_addr);
    extern int license_15talk_drivers_licence(const char *path, char *licence);

    extern void license_15talk_key_coding(char *buffer, int size);
    extern void license_15talk_key_decoding(char *buffer, int size);

    extern void license_15talk_printf(JNIEnv *env, char *format, ...);
    extern void license_15talk_thrown(JNIEnv *env, char *format, ...);
    extern void license_15talk_run(JNIEnv *env, char *buffer, size_t size);
    extern void license_15talk_complate(JNIEnv *env);
    extern void license_15talk_check(JNIEnv *env);

#ifdef __cplusplus
}
#endif
#endif

