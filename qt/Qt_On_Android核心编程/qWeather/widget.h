#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>

class WeatherRow
{
public:
    WeatherRow()
        : m_date(0), m_dayPicture(0)
        , m_nightPicture(0), m_weather(0)
    {

    }

    QLabel *m_date;
    QLabel *m_dayPicture;
    QLabel *m_nightPicture;
    QLabel *m_weather;
};

class QWeather
{
public:
    QString m_date;
    QString m_dayPicture;
    QString m_nightPicture;
    QString m_weather;
    QString m_wind;
    QString m_temperature;
    WeatherRow m_view;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool event(QEvent *e);

protected slots:
    void onRefreshButton();
    void onFinished(QNetworkReply * reply);

private:
    QNetworkReply *download(const QString &strUrl);
    void queryGeocoder();
    void parseGeocoder(QByteArray &data);
    void queryWeather();
    void parseWeather(QByteArray &data);
    void downloadWeatherIcon(const QString &url, int row, int col);
    void updateWeatherIcon(const QByteArray &data, int row, int col);
    bool isNetworkConnected();
    void getLocationByGps();
    void configureNetwork();
    void watchNetwork();

protected:
    QPushButton *m_refreshButton;
    QLabel *m_title;
    QLabel *m_state;
    QNetworkAccessManager m_nam;
    double m_longitude;
    double m_latitude;
    QWeather m_weathers[4];
    bool m_bWeatherValid;
};

#endif // WIDGET_H
