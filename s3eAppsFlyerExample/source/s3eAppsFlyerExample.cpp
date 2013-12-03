/*
 *
 * (C) 2013 AppsFlyer. All Rights Reserved.
 *
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

/**
 * @page s3eAppsFlyer AppsFlyer Example
 *
 * The following example demonstrates how to use the s3eAppsFlyer
 * functionality.
 *
 * @include s3eAppsFlyerExample.cpp
 */

// Marmalade headers
#include "s3eAppsFlyer.h"
#include "ExamplesMain.h"


// Status string to print at bottom of screen
static char g_StatusStr[8192]; // Can be quite a long string!

// Buttons
static Button* g_StartSession = NULL;
static Button* g_getAppsFlyerUID = NULL;
static Button* g_loadConversionData = NULL;
static Button* g_FireEventRegistration = NULL;
static Button* g_FireEventHotelBooked = NULL;
static Button* g_UseHTTPS = NULL;
static Button* g_SetIsDebug = NULL;
static Button* g_DisableTracking = NULL;

static bool gb_UseHTTPS = false;
static bool gb_Disabled = false;
static bool gb_IsDebug = false;

// Sets a string to be displayed beneath the buttons
static void SetStatus(const char* statusStr, ...)
{
    va_list args;
    va_start(args, statusStr);
    strcpy(g_StatusStr, "`x000000");
    vsprintf(g_StatusStr+strlen(g_StatusStr), statusStr, args);
    va_end(args);
}

// Appends to the string to be displayed beneath the buttons
static void AddStatus(const char* statusStr, ...)
{
    va_list args;
    va_start(args, statusStr);
    int currentLen = strlen(g_StatusStr);
    g_StatusStr[currentLen] = '\n';
    vsprintf(g_StatusStr + currentLen + 1, statusStr, args);
    va_end(args);
}

void ResetButtons()
{
    DeleteButtons();
    g_getAppsFlyerUID = 0;
    g_loadConversionData = 0;
    g_FireEventRegistration = 0;
    g_FireEventHotelBooked = 0;
    g_StartSession = 0;
    g_UseHTTPS = 0;
    g_DisableTracking = 0;
    g_SetIsDebug = 0;
}


int32 ConversionDataLoadedCallback(s3eAppsFlyerConversionData* data, void* userData)
{
    if (data) {
        if (!data->m_Num) {
            AddStatus("Error: no conversion data returned - zero dictionary");
        } else {
            for (int i = 0; i < data->m_Num; i++) {
                AddStatus("%s : %s", data->m_Keys[i], data->m_Values[i]);
            }
        }
    } else {
        AddStatus("Error: no conversion data returned");
    }
    return 0;
}



void ExampleInit()
{
    SetStatus("AppsFlyer Test");

    if (!s3eAppsFlyerAvailable())
    {
        SetStatus("AppsFlyer not supported on this platform");
        return;
    }

    // Create buttons
    ResetButtons();
    g_getAppsFlyerUID = NewButton("Get AppsFlyerUID");
    g_loadConversionData = NewButton("Load conversion data");
    g_FireEventHotelBooked = NewButton("Event Hotel booked");
    g_FireEventRegistration = NewButton("Event registration");
    g_StartSession = NewButton("Start session");
    g_UseHTTPS = NewButton("Toggle HTTPS");
    g_SetIsDebug = NewButton("Toggle Debug");
    g_DisableTracking = NewButton("Disable tracking");

    s3eAppsFlyerRegister(S3E_APPSFLYER_GOT_CONVERSION_DATA, (s3eCallback)ConversionDataLoadedCallback, NULL);
}

void ExampleTerm()
{
//    IwBilling::Terminate();
}

bool ExampleUpdate()
{
    Button* pressed = GetSelectedButton();

    if (pressed)
    {
        if (g_getAppsFlyerUID == pressed)
        {
            const char *appsFlyerUID = getAppsFlyerUID();
            AddStatus("UID = %s", appsFlyerUID);
        } 
        else if (g_FireEventHotelBooked == pressed)
        {
            s3eAppsFlyerTrackEvent("hotel-booked", "200.5");
            AddStatus("Fired event 'hotel-booked' with value 200.5");
        }
        else if (g_FireEventRegistration == pressed)
        {
            s3eAppsFlyerTrackEvent("registration");
            AddStatus("Fired event 'registartion' without value");
        }
        else if (g_StartSession == pressed)
        {
            s3eAppsFlyerStartSession();
            AddStatus("s3eAppsFlyerStartSession called.");
        }
        else if (g_SetIsDebug == pressed)
        {
            gb_IsDebug = !gb_IsDebug;
            s3eAppsFlyerSetIsDebug(gb_IsDebug? S3E_TRUE : S3E_FALSE);
            AddStatus("IsDebug set to %s.", gb_IsDebug? "true" : "false" );
        }
        else if (g_UseHTTPS == pressed)
        {
            gb_UseHTTPS = !gb_UseHTTPS;
            s3eAppsFlyerSetHTTPS(gb_UseHTTPS? S3E_TRUE : S3E_FALSE);
            AddStatus("UseHTTPS set to %s.", gb_UseHTTPS? "true" : "false" );
        }
        else if (g_DisableTracking == pressed)
        {
            gb_Disabled = !gb_Disabled;
            s3eAppsFlyerSetTrackingDisable(gb_Disabled? S3E_TRUE : S3E_FALSE);
            AddStatus("Tracking disabled set to %s.", gb_Disabled? "true" : "false" );
        }
        else if (g_loadConversionData == pressed)
        {
            
            
            s3eAppsFlyerLoadConversionData();
        }
    }

    return true;
}

void ExampleRender()
{
    // Print status string just below the buttons
    s3eDebugPrint(0, GetYBelowButtons(), g_StatusStr, S3E_TRUE);
}

