#include "log.h"
#include <QDateTime>

//
// class Logger
//

static const char * g_levelString[log_level_count] = {
    "CRITICAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
    "DEBUG2"
};

Logger::Logger(const QString &strLogFile, int level)
    : m_loglevel(level)
{
    m_file = new QFile(strLogFile);
    if(m_file && !m_file->open(
                QFile::WriteOnly | QFile::Append))
    {
        delete m_file;
        m_file = 0;
    }
}

Logger::~Logger()
{
    if(m_file)
    {
        m_file->close();
        delete m_file;
    }
}

bool Logger::isValid()
{
    return m_file != 0;
}

void Logger::setLogLevel(int level)
{
    if(level < log_critical)
    {
        m_loglevel = log_critical;
    }
    else if(level > log_debug_2)
    {
        m_loglevel = log_debug_2;
    }
    else
    {
        m_loglevel = level;
    }
}

void Logger::log(int level, const char *fmt, ...)
{
    if(level > m_loglevel || !m_file) return;

    char buffer[MAX_LOG_STRING_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, MAX_LOG_STRING_LEN - 1, fmt, args);
    va_end(args);

    log2file(level, buffer);
}

void Logger::log2file(int level, char *szLog)
{
    QString strLogInfo = QString("%1[%2]: %3\r\n")
            .arg(QDateTime::currentDateTime()
                 .toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(g_levelString[level]).arg(szLog);

    if(m_file->size() >= 1048576 )
    {
        m_file->resize(0);
    }
    m_file->write(strLogInfo.toLatin1());
    m_file->flush();
}
