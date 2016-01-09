package an.qt.imageProcessor;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.util.Log;
import android.net.Uri;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;
import java.io.File;
import android.provider.MediaStore;

public class ImageProcessorActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private final static int CAPTURE_IMAGE_REQUEST_CODE = 2;
    private static ImageProcessorActivity m_instance;
    private final static String TAG = "ImageProcessorActivity";
    public String m_imagePath;

    public ImageProcessorActivity(){
        m_instance = this;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        switch(requestCode){
        case CAPTURE_IMAGE_REQUEST_CODE:
            new ImageCaptureNative().OnImageCaptured(resultCode, m_imagePath);
            break;
        default:
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

   public static String getSdcardPath(){
       File sdDir = null;
       boolean sdCardExist = Environment.getExternalStorageState()
                       .equals(android.os.Environment.MEDIA_MOUNTED); 
       if(sdCardExist)
       {
            sdDir = Environment.getExternalStorageDirectory();
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
       intent.putExtra(MediaStore.EXTRA_OUTPUT, uri);
       m_instance.startActivityForResult(intent, CAPTURE_IMAGE_REQUEST_CODE);
   }
}
