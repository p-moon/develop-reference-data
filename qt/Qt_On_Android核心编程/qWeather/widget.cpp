#include "widget.h"
#include "../simpleCustomEvent.h"
#include <QGridLayout>
#include <QXmlStreamReader>
#include <QStringList>
#include <QCoreApplication>
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#endif
#include <QStringRef>
#include <QDate>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QMessageBox>
#include <QDebug>

#define API_GEOCODER   1
#define API_WEATHER    2
#define API_IMAGE      3

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_nam(this)
    , m_bWeatherValid(false)
{
    connect(&m_nam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply*)));
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setColumnStretch(3, 1);
    m_title = new QLabel();
    mainLayout->addWidget(m_title, 0, 0, 1, 4);

    QFontMetrics fm = fontMetrics();
    int colWidth = fm.width("周六")+8;
    for(int i = 0, row = 1; i < 4; ++i, ++row)
    {
        QWeather & weather = m_weathers[i];
        weather.m_view.m_date = new QLabel;
        weather.m_view.m_date->setFixedWidth(colWidth);
        mainLayout->addWidget(weather.m_view.m_date, row, 0);

        weather.m_view.m_dayPicture = new QLabel;
        weather.m_view.m_dayPicture->setFixedWidth(46);
        mainLayout->addWidget(weather.m_view.m_dayPicture, row, 1);

        weather.m_view.m_nightPicture = new QLabel;
        weather.m_view.m_nightPicture->setFixedWidth(46);
        mainLayout->addWidget(weather.m_view.m_nightPicture, row, 2);

        weather.m_view.m_weather = new QLabel;
        mainLayout->addWidget(weather.m_view.m_weather, row, 3);
    }
    m_refreshButton = new QPushButton("刷新");
    m_refreshButton->setFocusPolicy(Qt::NoFocus);
    connect(m_refreshButton, SIGNAL(clicked()),
            this, SLOT(onRefreshButton()));
    mainLayout->addWidget(m_refreshButton, 5, 0, Qt::AlignLeft | Qt::AlignBottom);

    m_state = new QLabel();
    if(!isNetworkConnected())
    {
        m_state->setText("网络断开");
    }
    mainLayout->addWidget(m_state, 5, 1, 1, 3, Qt::AlignLeft | Qt::AlignBottom);
    mainLayout->setRowStretch(5, 1);
    mainLayout->setColumnStretch(3, 1);

    watchNetwork();
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    if(e->type() == SimpleCustomEvent::eventType())
    {
        e->accept();
        SimpleCustomEvent *sce = (SimpleCustomEvent*)e;
        qDebug() << "Widget event - " << sce->m_arg1;
        switch(sce->m_arg1)
        {
        case 1: // gps location available
        {
            QStringList values = sce->m_arg2.split(',');
            m_longitude = values[0].toDouble();
            m_latitude = values[1].toDouble();
            m_state->setText("正在查询天气...");
            queryWeather();
        }
            break;
        case -1: // gps location error
            m_state->setText("无法获取位置信息");
            break;
        case 2: // network available
            m_state->setText("网络可用啦");
            break;
        case -2: // network error
            m_state->setText("网络断开");
            break;
        }
        return true;
    }
    return QWidget::event(e);
}

void Widget::onRefreshButton()
{
#ifdef Q_OS_ANDROID

    if(isNetworkConnected())
    {
        getLocationByGps();
    }
    else if(QMessageBox::Yes == QMessageBox::question(this, "没有网络", "是否连接网络？"))
    {
        configureNetwork();
    }
#else

    QString info = QString("109.00247,34.29876");
    QCoreApplication::postEvent(this, new SimpleCustomEvent(1, info));
#endif
}

void Widget::onFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    int api = reply->property("api_type").toInt();
    if(reply->error() != QNetworkReply::NoError)
    {
        if(api == API_WEATHER) m_state->setText(reply->errorString());
        return;
    }

    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //qDebug() << "IpQuery, status - " << status ;
    if(status != 200)
    {
        if(api == API_WEATHER) m_state->setText(QString("http status - %d").arg(status));
        return;
    }

    QByteArray data = reply->readAll();
    switch(api)
    {
    case API_GEOCODER:
        parseGeocoder(data);
        queryWeather();
        break;
    case API_WEATHER:
        parseWeather(data);
        break;
    case API_IMAGE:
        int row = reply->property("item_row").toInt();
        int col = reply->property("item_col").toInt();
        updateWeatherIcon(data, row, col);
        break;
    }
}

QNetworkReply * Widget::download(const QString &strUrl)
{
    QUrl url(strUrl);
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36");
    return m_nam.get(req);
}

void Widget::queryGeocoder()
{
    QString strUrl = QString("http://api.map.baidu.com/geocoder?location=%1,%2&output=xml&key=28bcdd84fae25699606ffad27f8da77b")
            .arg(m_latitude).arg(m_longitude);

    QNetworkReply * reply = download(strUrl);
    reply->setProperty("api_type", API_GEOCODER);
    qDebug() << "queryGeocoder - " << strUrl;
}

void Widget::parseGeocoder(QByteArray &data)
{
    QXmlStreamReader reader;
    reader.addData(data);
    QStringRef elementName;
    QString value;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            elementName = reader.name();
            if(elementName == "status")
            {
                value = reader.readElementText().trimmed();
                qDebug() << "parseGeocoder, status - " << value;
                if(value.compare("OK", Qt::CaseInsensitive))
                {
                    break;
                }
            }
            else if(elementName == "formatted_address")
            {
                value = reader.readElementText();
                m_title->setText(value);
                return;
            }
        }
    }
    //TODO: show error
    qDebug() << "formatted_address not found";
}

void Widget::queryWeather()
{
    QString strUrl = QString("http://api.map.baidu.com/telematics/v3/weather?location=%1,%2&output=xml&ak=7fa612aa9f51f2dab4e685404afdcf25")
            .arg(m_longitude).arg(m_latitude);
    QNetworkReply * reply = download(strUrl);
    reply->setProperty("api_type", API_WEATHER);
}

void Widget::parseWeather(QByteArray &data)
{
    QXmlStreamReader reader;
    reader.addData(data);
    QStringRef elementName;
    QString value;
    QDate date;
    int index = -1;
    bool inWeatherData = false;
    const char *week_days[] = {
        "周一","周二","周三","周四","周五","周六","周日"
    };
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            elementName = reader.name();
            if(!inWeatherData)
            {
                if(elementName == "status")
                {
                    value = reader.readElementText().trimmed();
                    qDebug() << "parseWeather, status - " << value;
                    if(value.compare("success", Qt::CaseInsensitive))
                    {
                        m_bWeatherValid = false;
                        break;
                    }
                    m_bWeatherValid = true;
                }
                else if(elementName == "date")
                {
                    value = reader.readElementText();
                    date = QDate::fromString(value, "yyyy-MM-dd");
                    qDebug() << "got date - " << date;
                }
                else if(elementName == "currentCity")
                {
                    value = reader.readElementText();
                    QString info = QString("%1 %2(%3)").arg(value)
                            .arg(date.toString("yyyy-MM-dd"))
                            .arg(week_days[date.dayOfWeek() - 1]);
                    m_title->setText(info);
                }
                else if(elementName == "weather_data")
                {
                    inWeatherData = true;
                }
            }
            else
            {
                if(elementName == "date")
                {
                    ++index;
                    if(index == 4) break;
                    QDate currentDate = date.addDays(index);
                    m_weathers[index].m_date = QString("%1")
                            .arg(week_days[currentDate.dayOfWeek() - 1]);
                }
                else if(elementName == "dayPictureUrl")
                {
                    m_weathers[index].m_dayPicture = reader.readElementText();
                }
                else if(elementName == "nightPictureUrl")
                {
                    m_weathers[index].m_nightPicture = reader.readElementText();
                }
                else if(elementName == "weather")
                {
                    m_weathers[index].m_weather = reader.readElementText();
                }
                else if(elementName == "wind")
                {
                    m_weathers[index].m_wind = reader.readElementText();
                }
                else if(elementName == "temperature")
                {
                    m_weathers[index].m_temperature = reader.readElementText();
                }
            }
        }
    }
    if(index >= 3)
    {
        for(int i = 0; i < 4; i++)
        {
            QWeather & weather = m_weathers[i];
            weather.m_view.m_date->setText(weather.m_date);
            downloadWeatherIcon(weather.m_dayPicture, i, 1);
            downloadWeatherIcon(weather.m_nightPicture, i, 2);
            QString weatherInfo = QString("%1\n%2\n%3")
                    .arg(weather.m_weather)
                    .arg(weather.m_wind)
                    .arg(weather.m_temperature);
            qDebug() << weatherInfo;
            weather.m_view.m_weather->setText(weatherInfo);
        }
        m_state->clear();
    }
    else
    {
        m_state->setText("查询天气失败");
    }
}

void Widget::downloadWeatherIcon(const QString &url, int row, int col)
{
    QNetworkReply * reply = download(url);
    reply->setProperty("api_type", API_IMAGE);
    reply->setProperty("item_row", row);
    reply->setProperty("item_col", col);
}

void Widget::updateWeatherIcon(const QByteArray &data, int row, int col)
{
    QPixmap pixmap;
    if(pixmap.loadFromData(data))
    {
        QWeather &weather = m_weathers[row];
        switch(col)
        {
        case 1:
            weather.m_view.m_dayPicture->setPixmap(pixmap);
            break;
        case 2:
            weather.m_view.m_nightPicture->setPixmap(pixmap);
            break;
        }
    }
}

bool Widget::isNetworkConnected()
{
#ifdef ANDROID
    jint state = QAndroidJniObject::callStaticMethod<jint>("an/qt/qWeather/QWeatherActivity",
                                       "networkState",
                                       "()I");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    return state == 1;
#endif
}

void Widget::getLocationByGps()
{
#ifdef ANDROID
    m_state->setText("正在获取位置信息……");
    QAndroidJniObject::callStaticMethod<void>("an/qt/qWeather/QWeatherActivity",
                                              "getLocation");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void Widget::configureNetwork()
{
#ifdef ANDROID
    QAndroidJniObject javaAction = QAndroidJniObject::fromString("android.settings.WIRELESS_SETTINGS");
    QAndroidJniObject::callStaticMethod<void>("an/qt/qWeather/QWeatherActivity",
                                              "launchActivity",
                                              "(Ljava/lang/String;)V",
                                              javaAction.object<jstring>());
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void Widget::watchNetwork()
{
#ifdef ANDROID
    QAndroidJniObject::callStaticMethod<void>("an/qt/qWeather/QWeatherActivity",
                                              "watchNetwork");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}
