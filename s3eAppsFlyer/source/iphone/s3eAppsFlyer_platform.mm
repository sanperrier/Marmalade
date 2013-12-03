/*
 * iphone-specific implementation of the s3eAppsFlyer extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eAppsFlyer_internal.h"

#include "s3eEdk.h"
#include "IwDebug.h"
#include "s3eConfig.h"

#include "AppsFlyer/AppsFlyerTracker.h"
#import <UIKit/UIKit.h>


const char* g_s3eAppsFlyerUDUID_str;
s3eAppsFlyerConversionData* g_conversionData;

static void onCallbackCompleted(uint32 deviceID, int notification, void* systemData, void* instance, int returnCode,  void* completeData);
void free_g_conversionData();
s3eResult s3eAppsFlyerStartSession_internal(const char* _appleAppId, const char* _devKey, const char* _currencyCode, s3eBool _isHTTPS, const char* _customerUserID, s3eBool checkAutoStart);


@interface MyDelegate : NSObject<AppsFlyerTrackerDelegate>
    - (void) onConversionDataReceived:(NSDictionary*) installData;
    - (void) onConversionDataRequestFailure:(NSError *)error;
@end

@implementation MyDelegate
    - (void) onConversionDataReceived:(NSDictionary*) installData {
        NSLog(@"%@",installData);
        
        free_g_conversionData();

        g_conversionData = (s3eAppsFlyerConversionData*)s3eEdkMallocOS(sizeof(s3eAppsFlyerConversionData), 0);
        if (g_conversionData) {
            g_conversionData->m_Num    = [installData count];
//            IwTrace(APPSFLYER_VERBOSE, ("count of entries in conversion data: %d", g_conversionData->m_Num));

            g_conversionData->m_Keys   = (char**)s3eEdkMallocOS(sizeof(char*) * g_conversionData->m_Num, 0);
            g_conversionData->m_Values = (char**)s3eEdkMallocOS(sizeof(char*) * g_conversionData->m_Num, 0);
            NSArray* keys = [installData allKeys];
//            NSLog(@"%@", keys);
            NSUInteger keysCount = [keys count];

            NSUInteger i = 0;
            for(NSString* key in keys) {
                IwTrace(APPSFLYER_VERBOSE, ("i: %d", i));
//                NSLog(@"%@", key);
                
                NSString* value =  [installData objectForKey:key];
//                NSLog(@"%@", value);
                g_conversionData->m_Keys[i]    = strdup([key   UTF8String]);
                g_conversionData->m_Values[i]  = strdup([value UTF8String]);
                i += 1;
            }
        
            s3eEdkCallbacksEnqueue(S3E_EXT_APPSFLYER_HASH, S3E_APPSFLYER_GOT_CONVERSION_DATA, g_conversionData, sizeof(s3eAppsFlyerConversionData),
                NULL, false, onCallbackCompleted, NULL);
        }

    }

    - (void) onConversionDataRequestFailure:(NSError *)error {
        NSLog(@"%@", error);
        
        free_g_conversionData();
        g_conversionData = (s3eAppsFlyerConversionData*)s3eEdkMallocOS(sizeof(s3eAppsFlyerConversionData), 0);
        g_conversionData->m_Num    = 0;
        g_conversionData->m_Keys   = 0;
        g_conversionData->m_Values = 0;

        s3eEdkCallbacksEnqueue(S3E_EXT_APPSFLYER_HASH, S3E_APPSFLYER_GOT_CONVERSION_DATA, g_conversionData, sizeof(s3eAppsFlyerConversionData),
                NULL, false, onCallbackCompleted, NULL);

    }
@end


MyDelegate* g_MyDelegate;

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


// free callback data
static void onCallbackCompleted(uint32 deviceID, int notification, void* systemData, void* instance, int returnCode,  void* completeData)
{
/*
    CCompletion* cp = (CCompletion*) completeData;
    if (cp == NULL)
    {
        IwTrace(WINDOWSPHONESTOREBILLING, ("onCallbackCompleted: called without completion data."));
    }
    else
    {
        IwTrace(WINDOWSPHONESTOREBILLING, ("onCallbackCompleted: deleting callback data."));
        delete cp;
    }
    return;
*/
}


int32 applicationDidBecomeActiveCallback(void* systemData, void* userData)
{
    //IwTrace(APPSFLYER_VERBOSE, ("applicationDidBecomeActiveCallback-----------------------------"));
    [[AppsFlyerTracker sharedTracker] trackAppLaunch];
    return 0;
}


s3eResult s3eAppsFlyerInit_platform()
{
    IwTrace(APPSFLYER_VERBOSE, ("s3eAppsFlyerInit_platform"));
    // Add any platform-specific initialisation code here
    g_s3eAppsFlyerUDUID_str = NULL;
    g_conversionData = NULL;
    g_MyDelegate = [[MyDelegate alloc] init];

    s3eEdkCallbacksRegisterInternal(S3E_EDK_INTERNAL, S3E_EDK_CALLBACK_MAX, S3E_EDK_IPHONE_APPLICATIONDIDBECOMEACTIVE,
                (s3eCallback)applicationDidBecomeActiveCallback,
                NULL, S3E_FALSE);

    s3eAppsFlyerStartSession_internal(NULL, NULL, NULL, S3E_FALSE, NULL, S3E_TRUE);
    
    return S3E_RESULT_SUCCESS;
}



void s3eAppsFlyerTerminate_platform()
{
    IwTrace(APPSFLYER_VERBOSE, ("s3eAppsFlyerTerminate_platform"));
    // Add any platform-specific termination code here
    if (g_s3eAppsFlyerUDUID_str)
        free((void*)g_s3eAppsFlyerUDUID_str);
//    if (g_s3eAppsFlyerCONVERSION_str)
//        free((void*)g_s3eAppsFlyerCONVERSION_str);
    free_g_conversionData();

    s3eEdkCallbacksUnRegister(S3E_EDK_INTERNAL, S3E_EDK_CALLBACK_MAX, S3E_EDK_IPHONE_APPLICATIONDIDBECOMEACTIVE,
                (s3eCallback)applicationDidBecomeActiveCallback);

}

s3eResult s3eAppsFlyerStartSession_platform(const char* _appleAppId, const char* _devKey, const char* _currencyCode, s3eBool _isHTTPS, const char* _customerUserID)
{
    IwTrace(APPSFLYER_VERBOSE, ("s3eAppsFlyerStartSession_platform"));
    return s3eAppsFlyerStartSession_internal(_appleAppId, _devKey, _currencyCode, _isHTTPS, _customerUserID, S3E_FALSE);
}

s3eResult s3eAppsFlyerStartSession_internal(const char* _appleAppId, const char* _devKey, const char* _currencyCode, s3eBool _isHTTPS, const char* _customerUserID, s3eBool checkAutoStart)
{
    IwTrace(APPSFLYER_VERBOSE, ("s3eAppsFlyerStartSession_platform"));
    s3eBool isHTTPS = _isHTTPS;

    if (checkAutoStart) {
        char tempStr[S3E_CONFIG_STRING_MAX+8] = {'\0'};
        if (s3eConfigGetString("APPSFLYER", "autoStart", tempStr) == S3E_RESULT_ERROR || strcmp(tempStr, "true") != 0) {
            return S3E_RESULT_SUCCESS;
        }
    }

    char appleAppId[S3E_CONFIG_STRING_MAX+8] = {'\0'};
    if (s3eConfigGetString("APPSFLYER", "appleAppId", appleAppId) == S3E_RESULT_ERROR)
    {
        //An error occurred during reading of the configuration file
        //strcpy(g_DisplayString, "`x666666"); //Make the string grey
        //strcat(g_DisplayString, s3eConfigGetErrorString());
//        IwTrace(APPSFLYER_VERBOSE, ("Error reading appleAppId. Please provie apple app id in app.icf to use AppsFlyer SDK."));
    }
    else
    {
        //Reading of configuration successful
        //strcpy(g_DisplayString, "`x666666"); //Make the string grey
        //strcat(g_DisplayString, TempString); //Add the result of s3eConfigGetString
    }

    char devKey[S3E_CONFIG_STRING_MAX+8] = {'\0'};
    if (s3eConfigGetString("APPSFLYER", "devKey", devKey) == S3E_RESULT_ERROR)
    {
        //An error occurred during reading of the configuration file
        //strcpy(g_DisplayString, "`x666666"); //Make the string grey
        //strcat(g_DisplayString, s3eConfigGetErrorString());
//        IwTrace(APPSFLYER_VERBOSE, ("Error reading devKey. Please provie You developer key in app.icf to use AppsFlyer SDK."));
    }
    else
    {
        //Reading of configuration successful
        //strcpy(g_DisplayString, "`x666666"); //Make the string grey
        //strcat(g_DisplayString, TempString); //Add the result of s3eConfigGetString
    }
    char tempStr[S3E_CONFIG_STRING_MAX+8] = {'\0'};
    if (s3eConfigGetString("APPSFLYER", "isHTTPS", tempStr) != S3E_RESULT_ERROR)
    {
        if (strcmp(tempStr, "true") == 0) {
            isHTTPS = true;
        }
    }
    if ( (appleAppId[0] != 0 || _appleAppId != NULL) && (devKey[0] != 0 || _devKey != NULL )) {
        const char* _ai = _appleAppId == NULL ? appleAppId : _appleAppId;
        const char* _dk = _devKey == NULL ? devKey : _devKey;
        IwTrace(APPSFLYER_VERBOSE, ("Initializing AppsFlyer SDK with '%s', '%s'.", _ai, _dk));
        NSString* nsAppleAppId = [NSString stringWithUTF8String:_ai];
        NSString* nsDevKey = [NSString stringWithUTF8String:_dk];

        [AppsFlyerTracker sharedTracker].appsFlyerDevKey = nsDevKey;
        [AppsFlyerTracker sharedTracker].appleAppID = nsAppleAppId;
        if (isHTTPS)
        {
            [AppsFlyerTracker sharedTracker].isHTTPS = YES; 
        }
        if (_currencyCode != NULL) {
            NSString* nsCurrencyCode = [NSString stringWithUTF8String:_currencyCode];
            [AppsFlyerTracker sharedTracker].currencyCode = nsCurrencyCode;
        }
        if (_customerUserID != NULL) {
            NSString* nsCustomerUserID = [NSString stringWithUTF8String:_customerUserID];
            [AppsFlyerTracker sharedTracker].customerUserID = nsCustomerUserID;
        }


        [[AppsFlyerTracker sharedTracker] trackAppLaunch];
        

        return S3E_RESULT_SUCCESS;
    } 
    else
    {
        IwTrace(APPSFLYER_VERBOSE, ("No configuration data for AppsFlyer SDK found."));
    }

    return S3E_RESULT_ERROR;
}

void s3eAppsFlyerSetHTTPS_platform(s3eBool isHTTPS)
{
    [AppsFlyerTracker sharedTracker].isHTTPS = isHTTPS ? YES : NO;
}

void s3eAppsFlyerSetCurrencyCode_platform(const char *currencyCode)
{
    if (currencyCode != 0)
    {
        NSString* nsCurrencyCode = [NSString stringWithUTF8String:currencyCode];
        [AppsFlyerTracker sharedTracker].currencyCode = nsCurrencyCode;
    }
}

void s3eAppsFlyerSetCustomerUserID_platform(const char *customerUserID)
{
    if (customerUserID != 0)
    {
        NSString* nsCustomerUserID = [NSString stringWithUTF8String:customerUserID];
        [AppsFlyerTracker sharedTracker].customerUserID = nsCustomerUserID;
    }
}

const char * getAppsFlyerUID_platform()
{
    NSString* s1 = [[AppsFlyerTracker sharedTracker] getAppsFlyerUID];
    if (g_s3eAppsFlyerUDUID_str)
        free((void*)g_s3eAppsFlyerUDUID_str);
    g_s3eAppsFlyerUDUID_str = strdup([s1 UTF8String]);

    IwTrace(APPSFLYER_VERBOSE, ("AppsFlyerUID returned %s", g_s3eAppsFlyerUDUID_str));
    
    return g_s3eAppsFlyerUDUID_str;
}

void s3eAppsFlyerSetTrackingDisable_platform(s3eBool _disable)
{
    [AppsFlyerTracker sharedTracker].deviceTrackingDisabled = _disable ? YES : NO;
}

void s3eAppsFlyerSetIsDebug_platform(s3eBool _isDebug)
{
    [AppsFlyerTracker sharedTracker].isDebug = _isDebug ? YES : NO;
}

void s3eAppsFlyerTrackEvent_platform(const char* eventName, const char* value)
{
    if (eventName != NULL)
    {
        NSString* nsEventName = [NSString stringWithUTF8String:eventName];
        if (value == NULL)
        {
            [[AppsFlyerTracker sharedTracker] trackEvent:nsEventName withValue:@""];
        } else {
            NSString* nsEventValue = [NSString stringWithUTF8String:value];
            [[AppsFlyerTracker sharedTracker] trackEvent:nsEventName withValue:nsEventValue];
        }
    }
}

void s3eAppsFlyerLoadConversionData_platform()
{
    [[AppsFlyerTracker sharedTracker] loadConversionDataWithDelegate : g_MyDelegate];
}
