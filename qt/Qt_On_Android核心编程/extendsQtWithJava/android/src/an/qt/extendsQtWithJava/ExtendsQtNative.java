package an.qt.extendsQtWithJava;
import java.lang.String;

public class ExtendsQtNative
{
    public static native String GetQtVersion();
    public static native void OnLocationReady(int result, double longitude, double latitude);
    public static native void OnImageCaptured(int result, String filePath);
}
