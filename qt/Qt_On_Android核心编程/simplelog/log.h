#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <QFile>
#include <QString>

enum
{
    log_critical,
    log_error,
    log_warn,
    log_info,
    log_debug,
    log_debug_2,

    log_level_count
};

#define MAX_LOG_STRING_LEN  4096

class Logger
{
public:
    Logger(const QString & strLogFile
           , int level = log_info);
    ~Logger();

    bool isValid();
    void setLogLevel(int level);
    void log(int level, const char * fmt, ...);

protected:
    void log2file(int level, char * szLog);

protected:
    int m_loglevel;
    QFile * m_file;
};

#ifdef Q_OS_WIN
#define INT64_FMT "%I64d"
#else
#define INT64_FMT "%lld"
#endif

#endif // SIMPLE_LOG_H
