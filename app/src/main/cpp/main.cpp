#include <jni.h>
#include <string>
#include <android/telephony/TelephonyManager.h>
#include "zygisk.hpp"

using namespace std;

const char *FAKE_IMEI1 = "352097001122334";
const char *FAKE_IMEI2 = "352098556677889";

static void (*original_getDeviceId)(void *, JNIEnv *, jobject);
static void (*original_getImei)(void *, JNIEnv *, jobject, jint);

static jstring fake_getDeviceId(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(FAKE_IMEI1);
}

static jstring fake_getImei(JNIEnv *env, jobject thiz, jint slotIndex) {
    return env->NewStringUTF(slotIndex == 0 ? FAKE_IMEI1 : FAKE_IMEI2);
}

extern "C" void [[gnu::visibility("default")]] onLoad(JavaVM *vm, JNIEnv *env) {
    JNINativeMethod methods[] = {
        {"getDeviceId", "()Ljava/lang/String;", (void *)fake_getDeviceId},
        {"getImei", "(I)Ljava/lang/String;", (void *)fake_getImei}
    };
    
    jclass telephonyClass = env->FindClass("android/telephony/TelephonyManager");
    env->RegisterNatives(telephonyClass, methods, sizeof(methods)/sizeof(JNINativeMethod));
}
