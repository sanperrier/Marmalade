package com.appsflyer.extension.s3eAppsFlyerInstallReceiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import java.util.ArrayList;
import android.util.Log;
import android.os.Bundle;
import android.content.SharedPreferences;

public class s3eAppsFlyerInstallReceiver extends BroadcastReceiver
{
    public static final String TAG = "s3eAppsFlyer";

    @Override
    public void onReceive(Context context, Intent intent)
    {
        Log.i(TAG, "Got intent " + intent);
        String action = intent.getAction();

        String referrer = intent.getStringExtra("referrer");
        Log.i(TAG, (new StringBuilder()).append("referrer=").append(referrer).toString());

        if (intent.getAction().equals("com.android.vending.INSTALL_REFERRER"))
        {
            Bundle b = intent.getExtras();
            //int bundleSize = b.size();
            //String[] keysArray = new String[bundleSize];
            //String[] valuesArray = new String[bundleSize];
            ArrayList<String> keysArray   = new ArrayList<String>(0);
            ArrayList<String> valuesArray = new ArrayList<String>(0);
            for (String k : b.keySet()) {
                String v = b.getString(k);
                if (v != null) {
                    keysArray.add(k);
                    valuesArray.add(v);
                    Log.i(TAG, (new StringBuilder()).append(k).append("=").append(v).toString());

                }
            }

            SharedPreferences sharedPreferences = context.getSharedPreferences("s3eappsflyer-data", 0);
            android.content.SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("referrer", referrer);
            editor.commit();

        }
    }

    private static native void native_onInstallRefererRecievedCallback(String[] keys, String[] values);

}
