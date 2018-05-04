#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_jeffboody_GearsES2eclair_GearsES2eclair_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
