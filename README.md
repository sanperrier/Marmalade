s3eAppsFlyer
============

Marmalade extension for http://appsflyer.com SDK.
Supports Android and iOS platforms.


On iOS used AppsFlyer iOS SDK v2.5.3.3
On Android used AppsFlyer Android SDK v1.3.19



HOW TO USE
---------

Add the following to the MKB file for your project:

    options 
    {
        #Adjust path as necessary:
        module_path="../s3eAppsFlyer" 
    }

    subprojects
    {
        s3eAppsFlyer
    }

Then add to your `app.icf`

    [APPSFLYER]
    appleAppId="Add your App Store application id here (for android this not needed)"
    devKey="Add your AppsFlyer dev key here"
    isHTTPS=true
    autoStart=true


Where 
`isHTTPS` key is optional. Default is false.
`autoStart` key is optional. Default is false.


If you set `autoStart` to true then this is all you need to start tracking installs, updates ans seesions. 
(For Android you must also make sure you using right AndroidManifest.xml).
If you wish track additional in-app events beyond app installs (including in-app purchases, game levels, etc.) 
then you must use [API](#API Reference).



API Reference
-------------

### Data Structures


```c++
typedef struct s3eAppsFlyerConversionData
{
    /// Keys from dictionary returned to onConversionDataReceived. The pointer is valid until your callback returns.
    char** m_Keys;

    /// Values from dictionary returned to onConversionDataReceived. The pointer is valid until your callback returns.
    char** m_Values;

    /// Number of keys and values in m_Keys and m_Values arrays. Will be zero if there was error.
    int m_Num;


};
```

This struct stores conversion data. This data kept valid until your callback returns.


### Methods


```c++
s3eBool s3eAppsFlyerAvailable();
```
Returns S3E_TRUE if the AppsFlyer extension is available.

<br/>


```c++
s3eResult s3eAppsFlyerStartSession(const char* appleAppId S3E_DEFAULT(NULL), const char* devKey S3E_DEFAULT(NULL), const char* currencyCode S3E_DEFAULT(NULL), s3eBool isHTTPS S3E_DEFAULT(S3E_FALSE), const char* customerUserID S3E_DEFAULT(NULL) );
```
Must be first method called. Initializes SDK. Called automatically if `autoStart` set to `true`.


<br/>


```c++
void s3eAppsFlyerTrackEvent(const char* eventName, const char* value S3E_DEFAULT(NULL));
```
Allows you to send in-app events to AppsFlyer analytics. This method allows you to add events dynamically 
by adding them directly to the application code. 
Params:

- eventName is any string to define the event name . For example: “registration” or “purchase”
- value is the sales value. For example: "0.99" or "0.79"


<br/>

