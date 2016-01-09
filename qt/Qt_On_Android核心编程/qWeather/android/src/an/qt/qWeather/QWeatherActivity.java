package an.qt.qWeather;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.app.PendingIntent;
import android.util.Log;
import android.net.Uri;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import java.io.File;
import android.location.LocationManager;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationProvider;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.content.BroadcastReceiver;

public class QWeatherActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private final static int GET_LOCATION_REQUEST_CODE = 1;
    private static QWeatherActivity m_instance;
    private final static String TAG = "qWeather";
    public LocationManager m_lm = null;
    private static Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case 2:
                m_instance.doGetLocation();
                break;
            };
        }
    };
    private LocationListener m_locationListener = new LocationListener(){
        public void onLocationChanged(Location location){
            double longitude = location.getLongitude();
            double latitude = location.getLatitude();
            Log.d(TAG, "longitude - " + longitude + " latitude - " + latitude);
            QWeatherNative.OnLocationReady(RESULT_OK, longitude, latitude);
            m_lm.removeUpdates(m_locationListener);
        }

        public void onProviderDisabled(String provider){
            Log.d(TAG, "onProviderDisabled - " + provider);
            QWeatherNative.OnLocationReady(0, 0, 0);
        }

        public void onProviderEnabled(String provider){
            Log.d(TAG, "onProviderEnabled - " + provider);
        }
        public void onStatusChanged(String provider, int status, Bundle extras){
            Log.d(TAG, "onStatusChanged - " + provider + " status - " + status);
            if(status == LocationProvider.OUT_OF_SERVICE)
                QWeatherNative.OnLocationReady(0, 0, 0);
        }
    };

    public BroadcastReceiver networkReceiver;

    public QWeatherActivity(){
        m_instance = this;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(networkReceiver != null)
            unregisterReceiver(networkReceiver);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        switch(requestCode){
        case GET_LOCATION_REQUEST_CODE:
            if(m_instance.m_lm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)){
                doGetLocation();
            }
            break;
        default:
            super.onActivityResult(requestCode, resultCode, data);
        }
    }
    
    public static int networkState(){
        ConnectivityManager mgr = (ConnectivityManager) m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = mgr.getActiveNetworkInfo();
        return (networkInfo != null && networkInfo.isAvailable()) ? 1 : 0;
    }

    public static void watchNetwork(){
        if(m_instance.networkReceiver == null){
            Log.d(TAG, "install network receiver");
            m_instance.networkReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    String action = intent.getAction();
                    if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
                        ConnectivityManager mgr = (ConnectivityManager)
                                m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
                        NetworkInfo networkInfo = mgr.getActiveNetworkInfo();
                        if (networkInfo != null && networkInfo.isAvailable()) {
                            Log.d(TAG, "network connected!");
                            QWeatherNative.OnNetworkStateChanged(1);
                        } else {
                            QWeatherNative.OnNetworkStateChanged(0);
                        }
                    }
                }
            };
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
            m_instance.registerReceiver(m_instance.networkReceiver, intentFilter);
        }
    }

    public static void launchActivity(String action){
        m_instance.startActivity(new Intent(action));
    }

    public static void getLocation(){
        Log.d(TAG, "getLocation");
        if(m_instance.m_lm == null)
        m_instance.m_lm = (LocationManager)m_instance.getSystemService(Context.LOCATION_SERVICE);
        if(m_instance.m_lm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)){
            m_instance.m_handler.sendMessage(m_handler.obtainMessage(2));
        }else{
            // open gps
            Intent intent = new Intent(Settings.ACTION_SECURITY_SETTINGS);
            m_instance.startActivityForResult(intent, GET_LOCATION_REQUEST_CODE);
        }
    }

    private void doGetLocation(){
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); // 高精度
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setPowerRequirement(Criteria.POWER_LOW); // 低功耗

        String provider = m_lm.getBestProvider(criteria, true); // 获取GPS信息
        Log.d(TAG, "provider - " + provider);
        m_lm.requestLocationUpdates(provider, 1000, 1, m_locationListener);
   }
}
