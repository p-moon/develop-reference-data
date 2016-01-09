#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QComboBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onGet();
    void onMetaDataChanged();
    void onDownloadProgress(qint64 received, qint64 total);
    void onReadyRead();
    void onError(QNetworkReply::NetworkError code);
    void onFinished();
    void onAuthenticationRequired(QNetworkReply *reply,
                                  QAuthenticator *authenticator);

private:
    void resetState();
    void disableControls();
    void enableControls();
    QString requestString(QNetworkRequest *req);
    QString responseString(QNetworkReply *reply);
    bool openFile(bool save, QString &errorString);

private:
    QLineEdit *m_userEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_urlEdit;
    QPushButton *m_getButton;
    QProgressBar *m_progress;
    QTextEdit *m_state;
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;
    QString m_strFilePath;
    QFile *m_file;
    QString m_user;
    QString m_password;
};

#endif // WIDGET_H
