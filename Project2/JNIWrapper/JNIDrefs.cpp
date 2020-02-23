#include "JNIDrefs.h"

std::shared_ptr<DrefStore> JNIDrefFunctions::getDrefStore()
{

	return PluginRunner::getGlobalPlugin()->getDrefStore();
	
}

void JNIDrefFunctions::registerDref(JNIEnv * e, jobject o, jstring name, jint type)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	int drefType = type;

	string drefTypeName;

	switch (drefType) {
	case 0:
		drefTypeName = "integer";
		break;

	case 1:
		drefTypeName = "decimal";
		break;

	case 2:
		drefTypeName = "string";
		break;

	case 3:
		drefTypeName = "decimal_array";
		break;

	case 4:
		drefTypeName = "int_array";
		break;
	}

	PluginRunner::getGlobalPlugin()->getDrefStore()->add(drefTypeName, drefName, drefName);
}

jfloat JNIDrefFunctions::getFloatRef(JNIEnv* e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	float value = getDrefStore()->getDecimal(drefName);

	return (jfloat)value;
}

jint JNIDrefFunctions::getIntRef(JNIEnv* e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	int value = getDrefStore()->getInt(drefName);

	return (jint)value;
}

jstring JNIDrefFunctions::getStringRef(JNIEnv* e, jobject o, jstring name)
{

	string drefName = e->GetStringUTFChars(name, nullptr);
	string value = getDrefStore()->getString(drefName);

	jstring result = e->NewStringUTF(value.c_str());

	return result;
	
}

jfloatArray JNIDrefFunctions::getFloatArrayRef(JNIEnv* e, jobject o, jstring name)
{

	string drefName = e->GetStringUTFChars(name, nullptr);
	shared_ptr<Dref> drefToGet = getDrefStore()->getDrefReference(drefName);

	int lenOfArray = drefToGet->getArraySize();

	jfloatArray arr = e->NewFloatArray(lenOfArray);

	double* arrayValues = getDrefStore()->getDecimalArray(drefName);

	for (int i = 0; i < lenOfArray; i++) {
		jfloat value = (jfloat)arrayValues[i];
		e->SetFloatArrayRegion(arr, i, 1, &value);
	}

	return arr;
	
}

jintArray JNIDrefFunctions::getIntArrayRef(JNIEnv* e, jobject o, jstring name)
{

	string drefName = e->GetStringUTFChars(name, nullptr);
	shared_ptr<Dref> drefToGet = getDrefStore()->getDrefReference(drefName);

	int lenOfArray = drefToGet->getArraySize();

	jintArray arr = e->NewIntArray(lenOfArray);

	int* arrayValues = drefToGet->getAsIntArray();

	for (int i = 0; i < lenOfArray; i++) {
		jint value = (jint)arrayValues[i];
		e->SetIntArrayRegion(arr, i, 1, &value);
	}

	return arr;
	
}

void JNIDrefFunctions::setFloatRef(JNIEnv* e, jobject o, jstring name, jfloat value)
{

	string drefName = e->GetStringUTFChars(name, nullptr);

	float valueToSet = value;

	getDrefStore()->setDecimal(drefName, valueToSet);
	
}

void JNIDrefFunctions::setIntRef(JNIEnv* e, jobject o, jstring name, jint value)
{

	string drefName = e->GetStringUTFChars(name, nullptr);
	int valueToSet = value;

	getDrefStore()->setInt(drefName, valueToSet);
	
}

void JNIDrefFunctions::setStringRef(JNIEnv* e, jobject o, jstring name, jstring value)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	string valueToSet = e->GetStringUTFChars(value, nullptr);

	getDrefStore()->setString(drefName, valueToSet);
}

void JNIDrefFunctions::setFloatArrayRef(JNIEnv* e, jobject o, jstring name, jobjectArray values)
{
}

void JNIDrefFunctions::setIntArrayRef(JNIEnv* e, jobject o, jstring name, jobjectArray values)
{
}
