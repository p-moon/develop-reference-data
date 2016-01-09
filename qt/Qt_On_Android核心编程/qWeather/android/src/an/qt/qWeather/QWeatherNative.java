package an.qt.qWeather;

public class QWeatherNative
{
    public static native void OnLocationReady(int result, double longitude, double latitude);
    public static native void OnNetworkStateChanged(int state);
}
