#include <QCoreApplication>
#include "log.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger logger("simplelog.log");
    if(logger.isValid())
    {
        logger.log(log_info, "started.");
        logger.log(log_info, "int max = %d", INT_MAX);
        logger.log(log_warn, "this just a test app");
        logger.log(log_info, "int64 max = " INT64_FMT, LONG_LONG_MAX);
    }
    return a.exec();
}
