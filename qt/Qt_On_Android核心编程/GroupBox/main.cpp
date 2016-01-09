#include <QApplication>
#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *window = new QWidget;

    QVBoxLayout *windowLayout = new QVBoxLayout(window);

    QGroupBox *language = new QGroupBox("Language");
    language->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *languageLayout = new QVBoxLayout(language);
    QCheckBox *check1 = new QCheckBox;
    check1->setChecked(true);
    check1->setText("Java is good!");
    QCheckBox *check2 = new QCheckBox("C++ is good!");
    QCheckBox *check3 = new QCheckBox("Nothing is good!");
    check3->setTristate(true);

    languageLayout->addWidget(check1);
    languageLayout->addWidget(check2);
    languageLayout->addWidget(check3);
    windowLayout->addWidget(language);

    QGroupBox *os = new QGroupBox("Mobile OS");
    os->setAlignment(Qt::AlignRight);
    QVBoxLayout *osLayout = new QVBoxLayout;
    osLayout->addWidget(new QRadioButton("Android"));
    osLayout->addWidget(new QRadioButton("Windows Phone"));
    osLayout->addWidget(new QRadioButton("iOS"));
    osLayout->addStretch();
    os->setLayout(osLayout);
    windowLayout->addWidget(os);

    QGroupBox *phone = new QGroupBox("Smart Phone");
    phone->setCheckable(true);
    phone->setChecked(false);
    QHBoxLayout *phoneLayout = new QHBoxLayout(phone);
    phoneLayout->addWidget(new QCheckBox("Sumsung"));
    phoneLayout->addWidget(new QCheckBox("Xiao Mi"));
    phoneLayout->addWidget(new QCheckBox("HTC"));
    windowLayout->addWidget(phone);

    windowLayout->addStretch(1);

    window->show();

    return a.exec();
}
