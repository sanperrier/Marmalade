/*
java implementation of the s3eAppsFlyer extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;
import com.appsflyer.AppsFlyerLib;
import android.util.Log;
import android.content.BroadcastReceiver;
import java.util.ArrayList;
import java.util.Map;
import android.content.SharedPreferences;
import com.appsflyer.AttributionIDNotReady;
import com.appsflyer.AppsFlyerConversionListener;


class s3eAppsFlyer
{
    public static final String TAG = "s3eAppsFlyer";

    public int s3eAppsFlyerStartSession(String appleAppId, String devKey, String currencyCode, boolean isHTTPS, String customerUserID)
    {
        if (devKey == null || devKey == "") {
            Log.i("s3eAppsFlyer", "s3eAppsFlyerStartSession - no dev key specified");
            return 1; // error
        }
        AppsFlyerLib.setAppsFlyerKey(devKey);
        if (customerUserID != null && customerUserID != "") {
            AppsFlyerLib.setAppUserId(customerUserID);
        }
        if (currencyCode != null && currencyCode != "") {
            AppsFlyerLib.setCurrencyCode(currencyCode);
        }
        AppsFlyerLib.setUseHTTPFalback(!isHTTPS);

        Log.i("s3eAppsFlyer", "sendTracking with " + devKey);
        AppsFlyerLib.sendTracking(LoaderActivity.m_Activity.getApplication());
        //  S3E_RESULT_SUCCESS = 0,  //!< The operation completed successfully
        //  S3E_RESULT_ERROR = 1     //!< An error occurred during the operation
        return 0;
    }

    public void s3eAppsFlyerSetHTTPS(boolean isHTTPS)
    {
        AppsFlyerLib.setUseHTTPFalback(!isHTTPS);
    }

    public void s3eAppsFlyerSetTrackingDisable(boolean _disable)
    {
        AppsFlyerLib.setDeviceTrackingDisabled(_disable);
    }

    public void s3eAppsFlyerSetCollectIMEI(boolean _disable)
    {
        AppsFlyerLib.setCollectIMEI(_disable);
    }

    public void s3eAppsFlyerSetCollectAndroidID(boolean _disable)
    {
        AppsFlyerLib.setCollectAndroidID(_disable);
    }

    public void s3eAppsFlyerSetCollectMACAddress(boolean _disable)
    {
        AppsFlyerLib.setCollectMACAddress(_disable);
    }

    public void s3eAppsFlyerSetIsDebug(boolean _isDebug)
    {

    }

    public void s3eAppsFlyerSetCurrencyCode(String currencyCode)
    {
        if (currencyCode != null && currencyCode != "") {
            AppsFlyerLib.setCurrencyCode(currencyCode);
        }
    }

    public void s3eAppsFlyerSetCustomerUserID(String customerUserID)
    {
        if (customerUserID != null && customerUserID != "") {
            AppsFlyerLib.setAppUserId(customerUserID);
        }
    }

    public void s3eAppsFlyerTrackEvent(String eventName, String value)
    {
        String v;
        if (value == null)
        {
            v = "";
        }
        else
        {
            v = value;
        }
        Log.i(TAG, "sendTrackingWithEvent(" + eventName + ", " + v + ")");
        AppsFlyerLib.sendTrackingWithEvent(LoaderActivity.m_Activity.getApplication(), eventName, v);
    }

    public String getAppsFlyerUID()
    {
        return AppsFlyerLib.getAppsFlyerUID(LoaderActivity.m_Activity.getApplication());

    }

    public void s3eAppsFlyerLoadConversionData()
    {
        if(LoaderActivity.m_Activity.getApplication() != null)
        {
            try {
                sendOnInstallRefererRecievedCallback( AppsFlyerLib.getConversionData(LoaderActivity.m_Activity.getApplication()) );
            } catch (AttributionIDNotReady e) {
                AppsFlyerLib.registerConversionListener(LoaderActivity.m_Activity.getApplication(), new AppsFlyerConversionListener() {

                    public void onInstallConversionDataLoaded(Map<String, String> conversionData)
                    {
                        Log.i(TAG, "onInstallConversionDataLoaded");
                        for (String attrName : conversionData.keySet()) {
                            Log.d(TAG,"attribute: "+attrName+" = "+conversionData.get(attrName));
                        }
                        sendOnInstallRefererRecievedCallback(conversionData);
                    }

                    public void onInstallConversionFailure(String errorMessage)
                    {
                        Log.i(TAG, "onInstallConversionFailure:" + errorMessage);
                        
                    }

                    public void onCurrentAttributionDataLoaded(Map<String, String> map)
                    {
                        Log.i(TAG, "onCurrentAttributionDataLoaded");
                        for (String attrName : map.keySet()) {
                            Log.d(TAG,"attribute: "+attrName+" = "+map.get(attrName));
                        }
                        sendOnInstallRefererRecievedCallback(map);
                    }
                });
            }
        }
    }

    private void sendOnInstallRefererRecievedCallback(Map<String, String> conversionData) {
        ArrayList<String> keysArray   = new ArrayList<String>(0);
        ArrayList<String> valuesArray = new ArrayList<String>(0);
        for (String attrName : conversionData.keySet()) {
            keysArray.add(attrName);
            valuesArray.add(conversionData.get(attrName));
        }
        try
        {
            if (keysArray.size() > 0) {
                String[] kStringArray = new String[keysArray.size()];
                String[] vStringArray = new String[keysArray.size()];
                native_onInstallRefererRecievedCallback(keysArray.toArray(kStringArray), valuesArray.toArray(vStringArray));
            } else {
                Log.i(TAG, "No conversion data");
            }
        }
        catch (UnsatisfiedLinkError e)
        {
            Log.i(TAG, "Failed to send InstallRefererRecievedCallback");
        }
    }

    private static native void native_onInstallRefererRecievedCallback(String[] keys, String[] values);

}


