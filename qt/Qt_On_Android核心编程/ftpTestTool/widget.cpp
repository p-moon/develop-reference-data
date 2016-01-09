#include "widget.h"
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QInputDialog>

Widget::Widget(QWidget *parent): QWidget(parent)
    , m_nam(0), m_reply(0) , m_file(0)
{
    QGridLayout *layout = new QGridLayout(this);
    QLabel *label = new QLabel("user:");
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(label, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    m_userEdit = new QLineEdit();
    layout->addWidget(m_userEdit, 0, 1);

    label = new QLabel("password:");
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(label, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    m_passwordEdit = new QLineEdit();
    layout->addWidget(m_passwordEdit, 1, 1);

    label = new QLabel("ftp url:");
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(label, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    m_urlEdit = new QLineEdit();
    layout->addWidget(m_urlEdit, 2, 1);

    m_getButton = new QPushButton("Get");
    connect(m_getButton, SIGNAL(clicked()), this, SLOT(onGet()));
    layout->addWidget(m_getButton, 3, 1, Qt::AlignRight | Qt::AlignVCenter);

    m_progress = new QProgressBar();
    m_progress->setRange(0, 100);
    layout->addWidget(m_progress, 4, 0, 1, 2);

    m_state = new QTextEdit();
    m_state->setReadOnly(true);
    layout->addWidget(m_state, 5, 0, 1, 2);

    layout->setRowStretch(5,1);
    layout->setColumnStretch(1, 1);
}

Widget::~Widget()
{

}

void Widget::onGet()
{
    disableControls();
    m_state->clear();
    m_progress->setValue(0);

    m_user = m_userEdit->text();
    m_password = m_passwordEdit->text();

    QString strURL = m_urlEdit->text();
    QUrl qurl(strURL);
    qurl.setUserName(m_user);
    qurl.setPassword(m_password);
    QNetworkRequest req(qurl);

    m_strFilePath = QFileDialog::getSaveFileName(this);

    QString errorString;
    if(!openFile(true, errorString))
    {
        QMessageBox::warning(this, tr("Save File Error")
                             , errorString
                             , QMessageBox::Ok);
        resetState();
        return;
    }
    m_nam = new QNetworkAccessManager();
    m_reply = m_nam->get(req);

    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(m_nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(onAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(metaDataChanged()), this, SLOT(onMetaDataChanged()));

    m_state->setText(requestString(&req));

    m_state->append("\n==waiting response==\n");
}

void Widget::onMetaDataChanged()
{
    m_state->append("\n----\n");
    m_state->append(responseString(m_reply));
}

void Widget::onDownloadProgress(qint64 received, qint64 total)
{
    if(total > 0)
    {
        int value = (received * 100) / total;
        m_progress->setValue(value);
    }
}

void Widget::onReadyRead()
{
    QByteArray data = m_reply->readAll();
    m_state->append(QString("read %1 bytes\n").arg(data.length()));
    if(m_file)
    {
        m_file->write(data);
    }
}

void Widget::onError(QNetworkReply::NetworkError code)
{
    m_reply->disconnect(this);
    onFinished();
}

void Widget::onFinished()
{
    QString strState = m_reply->error() ==
            QNetworkReply::NoError ? "success"
                                   : m_reply->errorString();
    m_state->append(QString("\nftp request done - %1\n")
                        .arg(strState));
    m_state->append("\n-------\n");
    resetState();
}

void Widget::onAuthenticationRequired(QNetworkReply *reply,
                              QAuthenticator *authenticator)
{
    //get user && password
    m_password = QInputDialog::getText(this,
                                       "请输入密码","密码：");
    if(!m_password.isEmpty())
    {
        authenticator->setUser(
                    m_user.isEmpty() ? "anonymous" : m_user);
        authenticator->setPassword(m_password);
    }
}

void Widget::resetState()
{
    if(m_file)
    {
        m_file->close();
        delete m_file;
        m_file = 0;
    }
    if(m_reply)
    {
        m_reply->disconnect(this);
        m_reply->deleteLater();
        m_reply = 0;
    }
    if(m_nam)
    {
        m_nam->deleteLater();
        m_nam = 0;
    }

    enableControls();
}

void Widget::disableControls()
{
    m_userEdit->setDisabled(true);
    m_passwordEdit->setDisabled(true);
    m_urlEdit->setDisabled(true);
    m_getButton->setDisabled(true);
}

void Widget::enableControls()
{
    m_userEdit->setEnabled(true);
    m_passwordEdit->setEnabled(true);
    m_urlEdit->setEnabled(true);
    m_getButton->setEnabled(true);
}

QString Widget::requestString(QNetworkRequest *req)
{
    QString joinedHeader;
    QVariant variant = req->header(QNetworkRequest::ContentTypeHeader);
    if(variant.isValid())
    {
        joinedHeader += "content-type: ";
        joinedHeader += variant.toString();
        joinedHeader += "\n";
    }

    QList<QByteArray> headerList = req->rawHeaderList();
    int count = headerList.size();
    QByteArray value;
    for(int i = 0; i < count; i++)
    {
        const QByteArray &tag = headerList.at(i);
        value = req->rawHeader(tag);
        joinedHeader += QString("%1: %2\n").arg(tag.data()).arg(value.data());
    }

    return joinedHeader;
}

QString Widget::responseString(QNetworkReply *reply)
{
    QString joinedHeader;
    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(variant.isValid())
    {
        joinedHeader = QString("STATUS: %1\n").arg(variant.toInt());
    }

    variant = reply->header(QNetworkRequest::ContentTypeHeader);
    if(variant.isValid())
    {
        joinedHeader += "content-type: ";
        joinedHeader += variant.toString();
        joinedHeader += "\n";
    }

    variant = reply->header(QNetworkRequest::ContentLengthHeader);
    if(variant.isValid())
    {
        joinedHeader += "content-length: ";
        joinedHeader += variant.toString();
        joinedHeader += "\n";
    }

    QList<QByteArray> headerList = reply->rawHeaderList();
    int count = headerList.size();
    QByteArray value;
    for(int i = 0; i < count; i++)
    {
        const QByteArray &tag = headerList.at(i);
        value = reply->rawHeader(tag);
        joinedHeader += QString("%1: %2\n").arg(tag.data()).arg(value.data());
    }

    return joinedHeader;
}

bool Widget::openFile(bool save, QString &errorString)
{
    if(m_strFilePath.isEmpty())
    {
        errorString = "No File Selected";
        return false;
    }
    if(m_file)
    {
        delete m_file;
        m_file = 0;
    }
    m_file = new QFile(m_strFilePath);
    if(!m_file->open(save ? QFile::WriteOnly : QFile::ReadOnly))
    {
        errorString = m_file->errorString();
        delete m_file;
        m_file = 0;
        return false;
    }
    return true;
}
