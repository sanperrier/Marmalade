/*
Generic implementation of the s3eAppsFlyer extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eAppsFlyer_internal.h"
s3eResult s3eAppsFlyerInit()
{
    //Add any generic initialisation code here
    return s3eAppsFlyerInit_platform();
}

void s3eAppsFlyerTerminate()
{
    //Add any generic termination code here
    s3eAppsFlyerTerminate_platform();
}

s3eResult s3eAppsFlyerStartSession(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID)
{
	return s3eAppsFlyerStartSession_platform(appleAppId, devKey, currencyCode, isHTTPS, customerUserID);
}

void s3eAppsFlyerSetHTTPS(s3eBool isHTTPS)
{
	s3eAppsFlyerSetHTTPS_platform(isHTTPS);
}

void s3eAppsFlyerSetTrackingDisable(s3eBool _disable)
{
	s3eAppsFlyerSetTrackingDisable_platform(_disable);
}

void s3eAppsFlyerSetIsDebug(s3eBool _isDebug)
{
	s3eAppsFlyerSetIsDebug_platform(_isDebug);
}

void s3eAppsFlyerSetCurrencyCode(const char* currencyCode)
{
	s3eAppsFlyerSetCurrencyCode_platform(currencyCode);
}

void s3eAppsFlyerSetCustomerUserID(const char* customerUserID)
{
	s3eAppsFlyerSetCustomerUserID_platform(customerUserID);
}

void s3eAppsFlyerTrackEvent(const char* eventName, const char* value)
{
	s3eAppsFlyerTrackEvent_platform(eventName, value);
}

const char * getAppsFlyerUID()
{
	return getAppsFlyerUID_platform();
}

void s3eAppsFlyerLoadConversionData()
{
	s3eAppsFlyerLoadConversionData_platform();
}
