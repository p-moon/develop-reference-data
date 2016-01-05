#include "editdialog.h"
#include <QMessageBox>
int uniqueCarId;
int uniqueFactoryId;

Dialog::Dialog(QSqlRelationalTableModel *cars,QSqlTableModel *factory,QDomDocument details,QFile *output,QWidget *parent) :
    QDialog(parent)
{
    carModel = cars;
    factoryModel = factory;
    carDetails = details;
    outputFile = output;

    QGroupBox *inputWidgetBox = createInputWidgets();
    QDialogButtonBox *buttonBox = createButtons();

    //界面布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(inputWidgetBox);
    layout->addWidget(buttonBox);
    setLayout(layout);

    setWindowTitle(tr("添加产品"));
}

void Dialog::submit()
{
    QString factory = factoryEditor->text();
    QString address = addressEditor->text();
    QString name = carEditor->text();

    if (factory.isEmpty() || address.isEmpty()||name.isEmpty())
    {
        QString message(tr("请输入厂名、厂址和商品名称！"));
        QMessageBox::information(this, tr("添加产品"), message);
    }
    else
    {
        int factoryId = findFactoryId(factory);
        if(factoryId == -1)
        {
            factoryId = addNewFactory(factory,address);
        }
        int carId = addNewCar(name, factoryId);
        QStringList attribs;
        attribs = attribEditor->text().split(";",QString::SkipEmptyParts);
        addAttribs(carId, attribs);
        accept();
    }
}

int Dialog::findFactoryId(const QString &factory)
{
    int row = 0;
    while (row < factoryModel->rowCount())
    {
        QSqlRecord record = factoryModel->record(row);
        if (record.value("manufactory") == factory)
            return record.value("id").toInt();
        else
            row++;
    }
    return -1;
}

int Dialog::addNewFactory(const QString &factory,const QString &address)
{
    QSqlRecord record;

    int id = generateFactoryId();

    QSqlField f1("id", QVariant::Int);
    QSqlField f2("manufactory", QVariant::String);
    QSqlField f3("address", QVariant::String);

    f1.setValue(QVariant(id));
    f2.setValue(QVariant(factory));
    f3.setValue(QVariant(address));
    record.append(f1);
    record.append(f2);
    record.append(f3);
    factoryModel->insertRecord(-1, record);
    return id;
}

int Dialog::addNewCar(const QString &name, int factoryId)
{
    int id = generateCarId();
    QSqlRecord record;

    QSqlField f1("carid", QVariant::Int);
    QSqlField f2("name", QVariant::String);
    QSqlField f3("factoryid", QVariant::Int);
    QSqlField f4("year", QVariant::Int);

    f1.setValue(QVariant(id));
    f2.setValue(QVariant(name));
    f3.setValue(QVariant(factoryId));
    f4.setValue(QVariant(yearEditor->value()));
    record.append(f1);
    record.append(f2);
    record.append(f3);
    record.append(f4);

    carModel->insertRecord(-1, record);
    return id;
}

void Dialog::addAttribs(int carId, QStringList attribs)
{
    QDomElement carNode = carDetails.createElement("car");
    carNode.setAttribute("id", carId);
    for (int i = 0; i < attribs.count(); i++)
    {
        QString attribNumber = QString::number(i+1);
        if (i < 10)
            attribNumber.prepend("0");
        QDomText textNode = carDetails.createTextNode(attribs.at(i));
        QDomElement attribNode = carDetails.createElement("attrib");
        attribNode.setAttribute("number", attribNumber);
        attribNode.appendChild(textNode);
        carNode.appendChild(attribNode);
    }
    QDomNodeList archive = carDetails.elementsByTagName("archive");
    archive.item(0).appendChild(carNode);
    if (!outputFile->open(QIODevice::WriteOnly))
    {
        return;
    }
    else
    {
        QTextStream stream(outputFile);
        archive.item(0).save(stream, 4);
        outputFile->close();
    }
}

void Dialog::revert()
{
    factoryEditor->clear();
    addressEditor->clear();
    carEditor->clear();
    yearEditor->setValue(QDate::currentDate().year());
    attribEditor->clear();
}

QGroupBox *Dialog::createInputWidgets()
{
    QGroupBox *box = new QGroupBox(tr("添加产品"));

    QLabel *factoryLabel = new QLabel(tr("制造商:"));
    QLabel *addressLabel = new QLabel(tr("厂址:"));
    QLabel *carLabel = new QLabel(tr("品牌:"));
    QLabel *yearLabel = new QLabel(tr("上市时间:"));
    QLabel *attribLabel = new QLabel(tr("产品属性 (由分号;隔开):"));

    factoryEditor = new QLineEdit;
    carEditor = new QLineEdit;
    addressEditor = new QLineEdit;
    yearEditor = new QSpinBox;
    yearEditor->setMinimum(1900);
    yearEditor->setMaximum(QDate::currentDate().year());
    yearEditor->setValue(yearEditor->maximum());
    yearEditor->setReadOnly(false);
    attribEditor = new QLineEdit;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(factoryLabel, 0, 0);
    layout->addWidget(factoryEditor, 0, 1);
    layout->addWidget(addressLabel, 1, 0);
    layout->addWidget(addressEditor, 1, 1);

    layout->addWidget(carLabel, 2, 0);
    layout->addWidget(carEditor, 2, 1);
    layout->addWidget(yearLabel, 3, 0);
    layout->addWidget(yearEditor, 3, 1);
    layout->addWidget(attribLabel, 4, 0, 1, 2);
    layout->addWidget(attribEditor, 5, 0, 1, 2);
    box->setLayout(layout);

    return box;
}

QDialogButtonBox *Dialog::createButtons()
{
    QPushButton *closeButton = new QPushButton(tr("关闭"));
    QPushButton *revertButton = new QPushButton(tr("撤销"));
    QPushButton *submitButton = new QPushButton(tr("提交"));

    closeButton->setDefault(true);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(revert()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(submitButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);

    return buttonBox;
}

int Dialog::generateFactoryId()
{
    uniqueFactoryId += 1;
    return uniqueFactoryId;
}

int Dialog::generateCarId()
{
    uniqueCarId += 1;
    return uniqueCarId;
}
