package an.qt.imageProcessor;
import java.lang.String;

public class ImageCaptureNative
{
    public native void OnImageCaptured(int result, String filePath);
}
