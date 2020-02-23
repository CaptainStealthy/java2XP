#pragma once

#include <string>
#include "jni.h"
#include "JNIWrapper/JNIUtils.h"
#include "Datarefs/Dref.h"





namespace JNIDrefFunctions
{

	extern std::shared_ptr<DrefStore> getDrefStore();
	
	extern void registerDref(JNIEnv *e, jobject o, jstring name, jint type);

	extern jfloat getFloatRef(JNIEnv * e, jobject o, jstring name);

	extern jint getIntRef(JNIEnv * e, jobject o, jstring name);

	extern jstring getStringRef(JNIEnv * e, jobject o, jstring name);

	extern jfloatArray getFloatArrayRef(JNIEnv * e, jobject o, jstring name);

	extern jintArray getIntArrayRef(JNIEnv * e, jobject o, jstring name);

	extern void setFloatRef(JNIEnv * e, jobject o, jstring name, jfloat value);

	extern void setIntRef(JNIEnv * e, jobject o, jstring name, jint value);

	extern void  setStringRef(JNIEnv * e, jobject o, jstring name, jstring value);

	extern void  setFloatArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);

	extern void  setIntArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);

	
	
}
