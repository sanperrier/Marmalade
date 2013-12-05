/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */

#include "s3eExt.h"
#include "IwDebug.h"
#include "s3eDevice.h"


#include "s3eAppsFlyer.h"


#ifndef S3E_EXT_SKIP_LOADER_CALL_LOCK
// For MIPs (and WP8) platform we do not have asm code for stack switching
// implemented. So we make LoaderCallStart call manually to set GlobalLock
#if defined __mips || defined S3E_ANDROID_X86 || (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP))
#define LOADER_CALL_LOCK
#endif
#endif

/**
 * Definitions for functions types passed to/from s3eExt interface
 */
typedef  s3eResult(*s3eAppsFlyerRegister_t)(s3eAppsFlyerCallback cbid, s3eCallback fn, void* userData);
typedef  s3eResult(*s3eAppsFlyerUnRegister_t)(s3eAppsFlyerCallback cbid, s3eCallback fn);
typedef  s3eResult(*s3eAppsFlyerStartSession_t)(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID);
typedef       void(*s3eAppsFlyerSetHTTPS_t)(s3eBool isHTTPS);
typedef       void(*s3eAppsFlyerSetTrackingDisable_t)(s3eBool _disable);
typedef       void(*s3eAppsFlyerSetIsDebug_t)(s3eBool _isDebug);
typedef       void(*s3eAppsFlyerSetCurrencyCode_t)(const char* currencyCode);
typedef       void(*s3eAppsFlyerSetCustomerUserID_t)(const char* customerUserID);
typedef       void(*s3eAppsFlyerTrackEvent_t)(const char* eventName, const char* value);
typedef const char *(*getAppsFlyerUID_t)();
typedef       void(*s3eAppsFlyerLoadConversionData_t)();

/**
 * struct that gets filled in by s3eAppsFlyerRegister
 */
typedef struct s3eAppsFlyerFuncs
{
    s3eAppsFlyerRegister_t m_s3eAppsFlyerRegister;
    s3eAppsFlyerUnRegister_t m_s3eAppsFlyerUnRegister;
    s3eAppsFlyerStartSession_t m_s3eAppsFlyerStartSession;
    s3eAppsFlyerSetHTTPS_t m_s3eAppsFlyerSetHTTPS;
    s3eAppsFlyerSetTrackingDisable_t m_s3eAppsFlyerSetTrackingDisable;
    s3eAppsFlyerSetIsDebug_t m_s3eAppsFlyerSetIsDebug;
    s3eAppsFlyerSetCurrencyCode_t m_s3eAppsFlyerSetCurrencyCode;
    s3eAppsFlyerSetCustomerUserID_t m_s3eAppsFlyerSetCustomerUserID;
    s3eAppsFlyerTrackEvent_t m_s3eAppsFlyerTrackEvent;
    getAppsFlyerUID_t m_getAppsFlyerUID;
    s3eAppsFlyerLoadConversionData_t m_s3eAppsFlyerLoadConversionData;
} s3eAppsFlyerFuncs;

static s3eAppsFlyerFuncs g_Ext;
static bool g_GotExt = false;
static bool g_TriedExt = false;
static bool g_TriedNoMsgExt = false;

static bool _extLoad()
{
    if (!g_GotExt && !g_TriedExt)
    {
        s3eResult res = s3eExtGetHash(0xc0bbea26, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        else
            s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE,                 "error loading extension: s3eAppsFlyer");

        g_TriedExt = true;
        g_TriedNoMsgExt = true;
    }

    return g_GotExt;
}

static bool _extLoadNoMsg()
{
    if (!g_GotExt && !g_TriedNoMsgExt)
    {
        s3eResult res = s3eExtGetHash(0xc0bbea26, &g_Ext, sizeof(g_Ext));
        if (res == S3E_RESULT_SUCCESS)
            g_GotExt = true;
        g_TriedNoMsgExt = true;
        if (g_TriedExt)
            g_TriedExt = true;
    }

    return g_GotExt;
}

s3eBool s3eAppsFlyerAvailable()
{
    _extLoadNoMsg();
    return g_GotExt ? S3E_TRUE : S3E_FALSE;
}

s3eResult s3eAppsFlyerRegister(s3eAppsFlyerCallback cbid, s3eCallback fn, void* userData)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[0] func: s3eAppsFlyerRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    s3eResult ret = g_Ext.m_s3eAppsFlyerRegister(cbid, fn, userData);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

s3eResult s3eAppsFlyerUnRegister(s3eAppsFlyerCallback cbid, s3eCallback fn)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[1] func: s3eAppsFlyerUnRegister"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    s3eResult ret = g_Ext.m_s3eAppsFlyerUnRegister(cbid, fn);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

s3eResult s3eAppsFlyerStartSession(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[2] func: s3eAppsFlyerStartSession"));

    if (!_extLoad())
        return S3E_RESULT_ERROR;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    s3eResult ret = g_Ext.m_s3eAppsFlyerStartSession(appleAppId, devKey, currencyCode, isHTTPS, customerUserID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void s3eAppsFlyerSetHTTPS(s3eBool isHTTPS)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[3] func: s3eAppsFlyerSetHTTPS"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerSetHTTPS(isHTTPS);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void s3eAppsFlyerSetTrackingDisable(s3eBool _disable)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[4] func: s3eAppsFlyerSetTrackingDisable"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerSetTrackingDisable(_disable);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void s3eAppsFlyerSetIsDebug(s3eBool _isDebug)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[5] func: s3eAppsFlyerSetIsDebug"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerSetIsDebug(_isDebug);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void s3eAppsFlyerSetCurrencyCode(const char* currencyCode)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[6] func: s3eAppsFlyerSetCurrencyCode"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerSetCurrencyCode(currencyCode);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void s3eAppsFlyerSetCustomerUserID(const char* customerUserID)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[7] func: s3eAppsFlyerSetCustomerUserID"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerSetCustomerUserID(customerUserID);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

void s3eAppsFlyerTrackEvent(const char* eventName, const char* value)
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[8] func: s3eAppsFlyerTrackEvent"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerTrackEvent(eventName, value);

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}

const char * getAppsFlyerUID()
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[9] func: getAppsFlyerUID"));

    if (!_extLoad())
        return NULL;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    const char * ret = g_Ext.m_getAppsFlyerUID();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return ret;
}

void s3eAppsFlyerLoadConversionData()
{
    IwTrace(APPSFLYER_VERBOSE, ("calling s3eAppsFlyer[10] func: s3eAppsFlyerLoadConversionData"));

    if (!_extLoad())
        return;

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallStart(S3E_TRUE, NULL);
#endif

    g_Ext.m_s3eAppsFlyerLoadConversionData();

#ifdef LOADER_CALL_LOCK
    s3eDeviceLoaderCallDone(S3E_TRUE, NULL);
#endif

    return;
}
