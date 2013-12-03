/*
 * Internal header for the s3eAppsFlyer extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef S3EAPPSFLYER_INTERNAL_H
#define S3EAPPSFLYER_INTERNAL_H

#include "s3eTypes.h"
#include "s3eAppsFlyer.h"
#include "s3eAppsFlyer_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult s3eAppsFlyerInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult s3eAppsFlyerInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void s3eAppsFlyerTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void s3eAppsFlyerTerminate_platform();
s3eResult s3eAppsFlyerStartSession_platform(const char* appleAppId, const char* devKey, const char* currencyCode, s3eBool isHTTPS, const char* customerUserID);

void s3eAppsFlyerSetHTTPS_platform(s3eBool isHTTPS);

void s3eAppsFlyerSetTrackingDisable_platform(s3eBool _disable);

void s3eAppsFlyerSetIsDebug_platform(s3eBool _isDebug);

void s3eAppsFlyerSetCurrencyCode_platform(const char* currencyCode);

void s3eAppsFlyerSetCustomerUserID_platform(const char* customerUserID);

void s3eAppsFlyerTrackEvent_platform(const char* eventName, const char* value);

const char * getAppsFlyerUID_platform();

void s3eAppsFlyerLoadConversionData_platform();


#endif /* !S3EAPPSFLYER_INTERNAL_H */