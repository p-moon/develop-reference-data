#ifndef QDEBUG2LOGCAT_H
#define QDEBUG2LOGCAT_H

#ifdef ANDROID
void installLogcatMessageHandler(const char *TAG);
#else
#define installLogcatMessageHandler(TAG)
#endif

#endif // QDEBUG2LOGCAT_H
