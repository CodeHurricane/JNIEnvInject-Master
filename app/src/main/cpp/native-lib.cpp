#include <jni.h>
#include <string>
JavaVM*  javaVM;
static char sig_buffer[512];
static const char JCLASS_LOADER[] = "Ljava/lang/ClassLoader;";
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
    javaVM = vm;
    return JNI_VERSION_1_4;
}

static jobject getSystemClassLoader(JNIEnv *jni_env){
    jclass class_loader_claxx = jni_env->FindClass("java/lang/ClassLoader");
    jmethodID getSystemClassLoader_method = jni_env->GetStaticMethodID(class_loader_claxx, "getSystemClassLoader", sig_buffer);
    return jni_env->CallStaticObjectMethod(class_loader_claxx, getSystemClassLoader_method);
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_user_jnienvinject_1master_MainActivity_dexFromNative(JNIEnv *jni_env,jobject /* this */){

    jstring apk_path = jni_env->NewStringUTF("/data/local/tmp/DemoInject2.apk");
    jstring dex_out_path = jni_env->NewStringUTF("/data/data/com.demo.host/dexout");
    jclass dexloader_claxx = jni_env->FindClass("dalvik/system/DexClassLoader");
    jmethodID dexloader_init_method = jni_env->GetMethodID(dexloader_claxx, "<init>",sig_buffer);
    jmethodID loadClass_method = jni_env->GetMethodID(dexloader_claxx, "loadClass", sig_buffer);

    jobject class_loader = getSystemClassLoader(jni_env);

    jobject dex_loader_obj = jni_env->NewObject(dexloader_claxx, dexloader_init_method, apk_path, dex_out_path, NULL, class_loader);
    jstring class_name = jni_env->NewStringUTF("com.example.user.jnienvinject_master.EntryClass");
    jclass entry_class = static_cast<jclass>(jni_env->CallObjectMethod(dex_loader_obj, loadClass_method, class_name));

    jmethodID invoke_method = jni_env->GetStaticMethodID(entry_class, "invoke", "(I)[Ljava/lang/Object;");

    jobjectArray objectarray = (jobjectArray) jni_env->CallStaticObjectMethod(entry_class, invoke_method, 0);

}
