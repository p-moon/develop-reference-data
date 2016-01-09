#include "ipQuery.h"
#include <QJsonDocument>
#include <QByteArray>
#include <QHostAddress>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QTextCodec>
#include <QDebug>
/*
 * http://opendata.baidu.com/api.php?query=36.57.177.187&co=&resource_id=6006&t=1401346439107&ie=utf8&oe=gbk&cb=op_aladdin_callback&format=json&tn=baidu&cb=jQuery11020322038795100525_1401343864465&_=1401343864504
 */

/*
 * jQuery11020322038795100525_1401343864465
( --remove
  { --> object
    "status":"0",
    "t":"1401346439107",
    "data":[ -->array
      { --> object
        "location":"安徽省宿州市 电信",
        "titlecont":"IP地址查询",
        "origip":"36.57.177.187",
        "origipquery":"36.57.177.187",
        "showlamp":"1",
        "showLikeShare":1,
        "shareImage":1,
        "ExtendedLocation":"",
        "OriginQuery":"36.57.177.187",
        "tplt":"ip",
        "resourceid":"6006",
        "fetchkey":"36.57.177.187",
        "appinfo":"", "role_id":0, "disp_type":0
      }
    ]
  }
);
 */
IpQuery::IpQuery(QObject *parent)
    : QObject(parent)
    , m_nam(this)
{
    connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));
}

IpQuery::~IpQuery()
{

}

void IpQuery::query(const QString &ip)
{
    QString strUrl = QString("http://opendata.baidu.com/api.php?query=%1&resource_id=6006&ie=utf8&format=json").arg(ip);
    QUrl url(strUrl);
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    QNetworkReply *reply = m_nam.get(req);
    reply->setProperty("string_ip", ip);
}

void IpQuery::query(quint32 ip)
{
    QHostAddress addr(ip);
    query(addr.toString());
}

void IpQuery::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    QString strIp = reply->property("string_ip").toString();
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "IpQuery, error - " << reply->errorString();
        emit finished(false, strIp, m_emptyString);
        return;
    }

    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //qDebug() << "IpQuery, status - " << status ;
    if(status != 200)
    {
        emit finished(false, strIp, m_emptyString);
        return;
    }

    QByteArray data = reply->readAll();
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    //qDebug() << "contentType - " << contentType;
    int charsetIndex = contentType.indexOf("charset=");
    if(charsetIndex > 0)
    {
        charsetIndex += 8;
        QString charset = contentType.mid(charsetIndex).trimmed().toLower();
        if(charset.startsWith("gbk") || charset.startsWith("gb2312"))
        {
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            if(codec)
            {
                data = codec->toUnicode(data).toUtf8();
            }
        }
    }

    int parenthesisLeft = data.indexOf('(');
    int parenthesisRight = data.lastIndexOf(')');
    if(parenthesisLeft >=0 && parenthesisRight >=0)
    {
        parenthesisLeft++;
        data = data.mid(parenthesisLeft, parenthesisRight - parenthesisLeft);
    }
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug() << "IpQuery, json error - " << err.errorString();
        emit finished(false, strIp, m_emptyString);
        return;
    }
    QJsonObject obj = json.object();
    QJsonObject::const_iterator it = obj.find("data");
    if(it != obj.constEnd())
    {
        QJsonArray dataArray = it.value().toArray();
        QJsonObject info = dataArray.first().toObject();
        QString area = info.find("location").value().toString();
        emit finished(true, strIp, area);
    }
}
