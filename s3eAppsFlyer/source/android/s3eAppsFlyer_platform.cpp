/*
 * android-specific implementation of the s3eAppsFlyer extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eAppsFlyer_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include "s3eConfig.h"
#include <jni.h>
#include "IwDebug.h"

static jobject g_Obj;
static jmethodID g_s3eAppsFlyerStartSession;
static jmethodID g_s3eAppsFlyerSetHTTPS;
static jmethodID g_s3eAppsFlyerSetTrackingDisable;
static jmethodID g_s3eAppsFlyerSetIsDebug;
static jmethodID g_s3eAppsFlyerSetCurrencyCode;
static jmethodID g_s3eAppsFlyerSetCustomerUserID;
static jmethodID g_s3eAppsFlyerTrackEvent;
static jmethodID g_getAppsFlyerUID;
static jmethodID g_s3eAppsFlyerLoadConversionData;

s3eResult s3eAppsFlyerStartSession_platform_2(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID, s3eBool checkAutoStart);
void free_g_conversionData();
void JNICALL AppsFlyer_onInstallRefererRecievedCallback(JNIEnv* env,jobject obj, jobjectArray keysArray, jobjectArray valuesArray);


const char* g_s3eAppsFlyerUDUID_str;

s3eAppsFlyerConversionData* g_conversionData;



s3eResult s3eAppsFlyerInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;
    g_s3eAppsFlyerUDUID_str = NULL;
    g_conversionData = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("s3eAppsFlyer");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_s3eAppsFlyerStartSession = env->GetMethodID(cls, "s3eAppsFlyerStartSession", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;)I");
    if (!g_s3eAppsFlyerStartSession)
        goto fail;

    g_s3eAppsFlyerSetHTTPS = env->GetMethodID(cls, "s3eAppsFlyerSetHTTPS", "(Z)V");
    if (!g_s3eAppsFlyerSetHTTPS)
        goto fail;

    g_s3eAppsFlyerSetTrackingDisable = env->GetMethodID(cls, "s3eAppsFlyerSetTrackingDisable", "(Z)V");
    if (!g_s3eAppsFlyerSetTrackingDisable)
        goto fail;

    g_s3eAppsFlyerSetIsDebug = env->GetMethodID(cls, "s3eAppsFlyerSetIsDebug", "(Z)V");
    if (!g_s3eAppsFlyerSetIsDebug)
        goto fail;

    g_s3eAppsFlyerSetCurrencyCode = env->GetMethodID(cls, "s3eAppsFlyerSetCurrencyCode", "(Ljava/lang/String;)V");
    if (!g_s3eAppsFlyerSetCurrencyCode)
        goto fail;

    g_s3eAppsFlyerSetCustomerUserID = env->GetMethodID(cls, "s3eAppsFlyerSetCustomerUserID", "(Ljava/lang/String;)V");
    if (!g_s3eAppsFlyerSetCustomerUserID)
        goto fail;

    g_s3eAppsFlyerTrackEvent = env->GetMethodID(cls, "s3eAppsFlyerTrackEvent", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_s3eAppsFlyerTrackEvent)
        goto fail;

    g_getAppsFlyerUID = env->GetMethodID(cls, "getAppsFlyerUID", "()Ljava/lang/String;");
    if (!g_getAppsFlyerUID)
        goto fail;

    g_s3eAppsFlyerLoadConversionData = env->GetMethodID(cls, "s3eAppsFlyerLoadConversionData", "()V");
    if (!g_s3eAppsFlyerLoadConversionData)
        goto fail;

    {
        jclass receiver = s3eEdkAndroidFindClass("com/appsflyer/extension/s3eAppsFlyerInstallReceiver/s3eAppsFlyerInstallReceiver");
        if (!receiver)
            goto fail;
        static const JNINativeMethod methods[]=
        {
            {"native_onInstallRefererRecievedCallback","([Ljava/lang/String;[Ljava/lang/String;)V",(void*)&AppsFlyer_onInstallRefererRecievedCallback}
        };
        // Register the native hooks
        if (env->RegisterNatives(receiver, methods, sizeof(methods)/sizeof(methods[0])))
            goto fail;
    }
    {
        jclass receiver = s3eEdkAndroidFindClass("s3eAppsFlyer");
        if (!receiver)
            goto fail;
        static const JNINativeMethod methods[]=
        {
            {"native_onInstallRefererRecievedCallback","([Ljava/lang/String;[Ljava/lang/String;)V",(void*)&AppsFlyer_onInstallRefererRecievedCallback}
        };
        // Register the native hooks
        if (env->RegisterNatives(receiver, methods, sizeof(methods)/sizeof(methods[0])))
            goto fail;
    }


    IwTrace(APPSFLYER, ("APPSFLYER init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    s3eAppsFlyerStartSession_platform_2(NULL, NULL, NULL, S3E_TRUE, NULL, S3E_TRUE);

    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(s3eAppsFlyer, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}



void s3eAppsFlyerTerminate_platform()
{
    // Add any platform-specific termination code here
    if (g_s3eAppsFlyerUDUID_str)
        free((void*)g_s3eAppsFlyerUDUID_str);

    free_g_conversionData();
}

s3eResult s3eAppsFlyerStartSession_platform(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID)
{
    return s3eAppsFlyerStartSession_platform_2(appleAppId, devKey, currencyCode, isHTTPS, customerUserID, S3E_FALSE);
}

s3eResult s3eAppsFlyerStartSession_platform_2(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID, s3eBool checkAutoStart)
{
    const char* _appleAppId = appleAppId;
    const char* _devKey = devKey;
    const char* _currencyCode = currencyCode;
    const char* _customerUserID = customerUserID;
    s3eBool _isHTTPS = isHTTPS;

    if (checkAutoStart) {
        char tempStr[S3E_CONFIG_STRING_MAX+8] = {'\0'};
        if (s3eConfigGetString("APPSFLYER", "autoStart", tempStr) == S3E_RESULT_ERROR || strcmp(tempStr, "true") != 0) {
            // No autostart
            return S3E_RESULT_SUCCESS;
        }
    }


        //const char* currencyCodeStr = NULL;
        char currencyCodeBuf[S3E_CONFIG_STRING_MAX+8] = {'\0'};
        if (s3eConfigGetString("APPSFLYER", "currencyCode", currencyCodeBuf) != S3E_RESULT_ERROR) {
            _currencyCode = currencyCodeBuf;
        }

        //const char * devKeyStr = NULL;
        char devKeyBuf[S3E_CONFIG_STRING_MAX+8] = {'\0'};
        if (s3eConfigGetString("APPSFLYER", "devKey", devKeyBuf) == S3E_RESULT_ERROR)
        {
            //An error occurred during reading of the configuration file
            return S3E_RESULT_SUCCESS;
        }
        else
        {
            //Reading of configuration successful
            _devKey = devKeyBuf;
        }
        char tempStr[S3E_CONFIG_STRING_MAX+8] = {'\0'};
        if (s3eConfigGetString("APPSFLYER", "isHTTPS", tempStr) != S3E_RESULT_ERROR) {
            if (strcmp(tempStr, "true") == 0) {
                _isHTTPS = true;
            } else {
                _isHTTPS = false;
            }
        }
        IwTrace(APPSFLYER_VERBOSE, ("Initializing AppsFlyer SDK with '%s'.", _devKey));



    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring appleAppId_jstr = env->NewStringUTF(_appleAppId);
    jstring devKey_jstr = env->NewStringUTF(_devKey);
    jstring currencyCode_jstr = env->NewStringUTF(_currencyCode);
    jstring customerUserID_jstr = env->NewStringUTF(_customerUserID);
    return (s3eResult)env->CallIntMethod(g_Obj, g_s3eAppsFlyerStartSession, appleAppId_jstr, devKey_jstr, currencyCode_jstr, _isHTTPS, customerUserID_jstr);
}

void s3eAppsFlyerSetHTTPS_platform(s3eBool isHTTPS)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerSetHTTPS, isHTTPS);
}

void s3eAppsFlyerSetTrackingDisable_platform(s3eBool _disable)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerSetTrackingDisable, _disable);
}

void s3eAppsFlyerSetIsDebug_platform(s3eBool _isDebug)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerSetIsDebug, _isDebug);
}

void s3eAppsFlyerSetCurrencyCode_platform(const char* currencyCode)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring currencyCode_jstr = env->NewStringUTF(currencyCode);
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerSetCurrencyCode, currencyCode_jstr);
}

void s3eAppsFlyerSetCustomerUserID_platform(const char* customerUserID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring customerUserID_jstr = env->NewStringUTF(customerUserID);
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerSetCustomerUserID, customerUserID_jstr);
}

void s3eAppsFlyerTrackEvent_platform(const char* eventName, const char* value)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring eventName_jstr = env->NewStringUTF(eventName);
    jstring value_jstr = env->NewStringUTF(value);
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerTrackEvent, eventName_jstr, value_jstr);
}

const char * getAppsFlyerUID_platform()
{
    if (g_s3eAppsFlyerUDUID_str)
        free((void*)g_s3eAppsFlyerUDUID_str);
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring returnString = (jstring)env->CallObjectMethod(g_Obj, g_getAppsFlyerUID);
    if (!returnString) return NULL;
    const char *temp = env->GetStringUTFChars(returnString, NULL);
    g_s3eAppsFlyerUDUID_str = strdup(temp);
    env->ReleaseStringUTFChars(returnString, temp);
    return g_s3eAppsFlyerUDUID_str;
}

void s3eAppsFlyerLoadConversionData_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eAppsFlyerLoadConversionData);
    /*
    if (g_conversionData) {
        s3eEdkCallbacksEnqueue(S3E_EXT_APPSFLYER_HASH, S3E_APPSFLYER_GOT_CONVERSION_DATA, g_conversionData, sizeof(s3eAppsFlyerConversionData),
            NULL, false, NULL, NULL);
    }
    */
}

void JNICALL AppsFlyer_onInstallRefererRecievedCallback(JNIEnv* env,jobject obj, jobjectArray keysArray, jobjectArray valuesArray)
{
    free_g_conversionData();

    g_conversionData = (s3eAppsFlyerConversionData*)s3eEdkMallocOS(sizeof(s3eAppsFlyerConversionData), 0);
    if (g_conversionData) {

        int nKeys = env->GetArrayLength(keysArray);
        g_conversionData->m_Num = nKeys;
        g_conversionData->m_Keys   = (char**)s3eEdkMallocOS(sizeof(char*) * g_conversionData->m_Num, 0);
        g_conversionData->m_Values = (char**)s3eEdkMallocOS(sizeof(char*) * g_conversionData->m_Num, 0);

        //jobject j_order=env->GetObjectArrayElement(orders,i);
        for (int i = 0; i < nKeys; ++i)
        {
            jstring keyStr   = (jstring)env->GetObjectArrayElement(keysArray, i);
            jstring valueStr = (jstring)env->GetObjectArrayElement(valuesArray, i);
            if (!keyStr) return;
            if (!valueStr) return;

            const char *temp = env->GetStringUTFChars(keyStr, NULL);
            g_conversionData->m_Keys[i]    = strdup(temp);
            env->ReleaseStringUTFChars(keyStr, temp);

            temp = env->GetStringUTFChars(valueStr, NULL);
            g_conversionData->m_Values[i]  = strdup(temp);
            env->ReleaseStringUTFChars(valueStr, temp);
        }

        s3eEdkCallbacksEnqueue(S3E_EXT_APPSFLYER_HASH, S3E_APPSFLYER_GOT_CONVERSION_DATA, g_conversionData, sizeof(s3eAppsFlyerConversionData),
            NULL, false, NULL, NULL);
    }


}

void free_g_conversionData() {
    if (g_conversionData) {
        for (int i = 0; i < g_conversionData->m_Num; i++) {
            if (g_conversionData->m_Keys[i]) {
                free(g_conversionData->m_Keys[i]);
                g_conversionData->m_Keys[i] = 0;
            }
            if (g_conversionData->m_Values[i]) {
                free(g_conversionData->m_Values[i]);
                g_conversionData->m_Values[i] = 0;
            }
        }
        if (g_conversionData->m_Keys) {
            s3eEdkFreeOS(g_conversionData->m_Keys);
            s3eEdkFreeOS(g_conversionData->m_Values);
        }

        //free(g_conversionData);
        s3eEdkFreeOS(g_conversionData);
        g_conversionData = NULL;
    }
}
