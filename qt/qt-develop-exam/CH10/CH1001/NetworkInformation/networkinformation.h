#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QHostInfo>
#include <QNetworkInterface>

class NetworkInformation : public QWidget
{
    Q_OBJECT
    
public:
    NetworkInformation(QWidget *parent = 0);
    ~NetworkInformation();
    void getHostInformation();
public slots:
    void slotDetail();
private:
    QLabel *hostLabel;
    QLineEdit *LineEditLocalHostName;
    QLabel *ipLabel;
    QLineEdit *LineEditAddress;

    QPushButton *detailBtn;

    QGridLayout *mainLayout;
};

#endif // NETWORKINFORMATION_H
