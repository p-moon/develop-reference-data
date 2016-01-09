package an.qt.extendsQtWithJava;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.location.LocationManager;
import android.location.Criteria;
import android.provider.Settings;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationProvider;
import java.lang.ClassLoader;
import dalvik.system.DexClassLoader;
import java.lang.reflect.Field;
import android.os.Bundle;
import android.os.Environment;
import java.io.File;
import android.provider.MediaStore;

public class ExtendsQtWithJava extends org.qtproject.qt5.android.bindings.QtActivity
{
    private final static int GET_LOCATION_REQUEST_CODE = 1;
    private final static int CAPTURE_IMAGE_REQUEST_CODE = 2;
    private static NotificationManager m_notificationManager;
    private static ExtendsQtWithJava m_instance;
    private static Class m_nativeClass = null;
    private final static String TAG = "extendsQt";
    public LocationManager m_lm = null;
    public String m_imagePath;
    private static Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case 1:
                String qtVersion = ExtendsQtNative.GetQtVersion();
                String toastText = String.format("%s,by %s", (String)msg.obj, qtVersion);
                Toast toast = Toast.makeText(m_instance, toastText, Toast.LENGTH_LONG);
                toast.show();
                break;
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
            ExtendsQtNative.OnLocationReady(RESULT_OK, longitude, latitude);
            m_lm.removeUpdates(m_locationListener);
        }

        public void onProviderDisabled(String provider){
            Log.d(TAG, "onProviderDisabled - " + provider);
            ExtendsQtNative.OnLocationReady(-1, 0, 0);
        }

        public void onProviderEnabled(String provider){
            Log.d(TAG, "onProviderEnabled - " + provider);
        }
        public void onStatusChanged(String provider, int status, Bundle extras){
            Log.d(TAG, "onStatusChanged - " + provider + " status - " + status);
            if(status == LocationProvider.OUT_OF_SERVICE)
                ExtendsQtNative.OnLocationReady(-1, 0, 0);
        }
    };

    public ExtendsQtWithJava(){
        m_instance = this;
    }

    /**
     * 获得类的成员变量值，包括私有成员
     * @param instance 被调用的类
     * @param variableName 成员变量名
     *
     */
    public static Object get(Object instance, String variableName)
    {
        Class targetClass = instance.getClass().getSuperclass();
        org.qtproject.qt5.android.bindings.QtActivity superInst = (org.qtproject.qt5.android.bindings.QtActivity)targetClass.cast(instance);
        Log.d(TAG, "super class name - " + targetClass.getName() + " super instance -" + superInst);
        Field field;
        try {
            field = targetClass.getDeclaredField(variableName);
            Log.d(TAG, "field name - " + field.getName());
            field.setAccessible(true);//访问私有必须调用
            return field.get(superInst);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static void loadExtendsQtNative(){
    if(m_nativeClass == null){
        DexClassLoader loader = (DexClassLoader)get(m_instance, "m_classLoader");
        Log.d(TAG, "loader - " + loader);
        if(loader != null){
            try{
                m_nativeClass = loader.loadClass("an.qt.extendsQtWithJava.ExtendsQtNative");
                Log.d(TAG, "load ExtendsQtNative OK!");
            }catch(ClassNotFoundException e){
                Log.d(TAG, "load ExtendsQtNative failed");
            }
        }
    }
    }

    public static int networkState(){
        ConnectivityManager conMan = (ConnectivityManager) m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
        return conMan.getActiveNetworkInfo() == null ? 0 : 1;
    }

    public static void notify(String s){
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
        }

        Notification n = new Notification(R.drawable.icon, "A Message from Qt", System.currentTimeMillis());
        n.flags = Notification.FLAG_AUTO_CANCEL;
        Intent i = new Intent(m_instance, ExtendsQtWithJava.class);
        i.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);

        PendingIntent contentIntent = PendingIntent.getActivity(
            m_instance,
            R.string.app_name,
            i,
            PendingIntent.FLAG_UPDATE_CURRENT);

        n.setLatestEventInfo(
            m_instance,
            "A Message from Qt",
            s,
            contentIntent);
        m_notificationManager.notify(R.string.app_name, n);
    }
    
    public static void makeToast(String s){
        m_handler.sendMessage(m_handler.obtainMessage(1, s));
    }

    public static void launchActivity(String action){
        m_instance.startActivity(new Intent(action));
    }

    public static void emitBroadcast(String action, String extraName, int param){
        Intent i = new Intent(action);
        i.putExtra(extraName, param);
        m_instance.sendBroadcast(i);
        Log.d(TAG,
            String.format("broadcast - %s extra name - %s value - %d", action, extraName, param));
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        switch(requestCode){
        case GET_LOCATION_REQUEST_CODE:
            if(m_instance.m_lm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)){
                doGetLocation();
            }
            break;
        case CAPTURE_IMAGE_REQUEST_CODE:
            ExtendsQtNative.OnImageCaptured(resultCode, m_imagePath);
            break;
        default:
            super.onActivityResult(requestCode, resultCode, data);
        }
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
        //LocationManager lm = (LocationManager)getSystemService(Context.LOCATION_SERVICE);
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); // 高精度
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setPowerRequirement(Criteria.POWER_LOW); // 低功耗

        String provider = m_lm.getBestProvider(criteria, true); // 获取GPS信息
        Log.d(TAG, "provider - " + provider);
        m_lm.requestLocationUpdates(provider, 1000, 1, m_locationListener);
        //ExtendsQtNative.OnLocationReady(0, 0, 0);
        //ExtendsQtNative.OnImageCaptured(0);
        Log.d(TAG, "after call requestLocationUpdates");
        /*
        Location location = m_lm.getLastKnownLocation(provider); // 通过GPS获取位置
        if(location != null){
            double longitude = location.getLongitude();
            double latitude = location.getLatitude();
            Log.d(TAG, "before call native OnLocationReady, longitude - " + longitude + " latitude - " + latitude);
            ExtendsQtNative.OnLocationReady(RESULT_OK, longitude, latitude);
        }else{
            Log.d(TAG, "no location");
        }
        */
   }

   public static String getSdcardPath(){
       File sdDir = null;
       boolean sdCardExist = Environment.getExternalStorageState()
                       .equals(android.os.Environment.MEDIA_MOUNTED);   //判断sd卡是否存在
       if(sdCardExist)
       {
            sdDir = Environment.getExternalStorageDirectory();//获取跟目录
            return sdDir.toString();
       }
       return "";
   }

   public void initCaptureImagePath(){
       if( m_imagePath == null ){
           String sdPath = getSdcardPath();
           if(sdPath.isEmpty()){
               sdPath = "/sdcard";
           }
           m_imagePath = String.format("%s/qtimagecap", sdPath);
           File imageDir = new File(m_imagePath);
           if(!imageDir.exists()){
               imageDir.mkdirs();
           }
           m_imagePath = String.format("%s/qtimagecap/cap.jpg", sdPath);
           Log.d(TAG, "capture to - " + m_imagePath);
       }
   }

   public static void captureImage(){
       m_instance.initCaptureImagePath();
       File imageFile = new File(m_instance.m_imagePath);
       Uri uri = Uri.fromFile(imageFile);
       Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
       intent.putExtra(MediaStore.EXTRA_OUTPUT, uri);//
       m_instance.startActivityForResult(intent, CAPTURE_IMAGE_REQUEST_CODE);
   }
}
