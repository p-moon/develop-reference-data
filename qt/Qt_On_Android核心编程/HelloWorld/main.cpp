#include <QApplication>
#include <QLabel>
#include <stdio.h>
#include <stdlib.h>

static FILE * g_log_fp = 0;
static void closeLogFile()
{
    fclose(g_log_fp);
}
static void myMessageOutput(QtMsgType type,
    const QMessageLogContext &context,
    const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(g_log_fp, "Debug: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file, context.line,
                context.function);
        break;
    case QtWarningMsg:
        fprintf(g_log_fp, "Warning: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtCriticalMsg:
        fprintf(g_log_fp, "Critical: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtFatalMsg:
        fprintf(g_log_fp, "Fatal: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    g_log_fp = fopen("my.log", "wt");
    atexit(closeLogFile);
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);

    QLabel label(QObject::tr("Hello World!"));
    label.show();

    return a.exec();
}
