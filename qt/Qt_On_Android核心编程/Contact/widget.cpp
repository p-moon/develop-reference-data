#include "widget.h"
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

ContactStreamReader::ContactStreamReader(QTreeWidget *tree) : m_tree(tree)
{}

ContactStreamReader::~ContactStreamReader()
{}

bool ContactStreamReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))  return false;
    m_reader.setDevice(&file);
    m_reader.readNext();
    while(!m_reader.atEnd()){
        if(m_reader.isStartElement()){
            if(m_reader.name()=="peoples"){
                readPeoplesElement();
            }else{
                 m_reader.raiseError("not a contact file");
            }
        }else{
            m_reader.readNext();
        }
    }
    file.close();
    return (!m_reader.hasError() && file.error() == QFile::NoError);

}

void ContactStreamReader::readPeoplesElement()
{
    m_reader.readNext();
    while(!m_reader.atEnd())
    {
        if(m_reader.isEndElement())
        {
            m_reader.readNext();
            break;
        }

        if(m_reader.isStartElement())
        {
            if(m_reader.name() == "people")
            {
                readPeopleElement(m_tree->invisibleRootItem());
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            m_reader.readNext();
        }
    }
}

void ContactStreamReader::readPeopleElement(QTreeWidgetItem *parent)
{
    QTreeWidgetItem * peopleItem = new QTreeWidgetItem(parent);
    peopleItem->setText(0, m_reader.attributes().value("name").toString());

    m_reader.readNext();
    while(!m_reader.atEnd())
    {
        if(m_reader.isEndElement())
        {
            m_reader.readNext();
            break;
        }

        if(m_reader.isStartElement())
        {
            if(m_reader.name() == "mobile")
            {
                readMobileElement(peopleItem);
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            m_reader.readNext();
        }
    }
}

void ContactStreamReader::readMobileElement(QTreeWidgetItem *parent)
{
    QString strMobile = m_reader.readElementText();
    qDebug() << "mobile - " << strMobile;
    QTreeWidgetItem * mobileItem = new QTreeWidgetItem(parent);
    mobileItem->setText(0, "Mobile");
    mobileItem->setText(1,strMobile);
    if(m_reader.isEndElement())
    {
        m_reader.readNext();
    }
}

void ContactStreamReader::skipUnknownElement()
{
    m_reader.readNext();
    while (!m_reader.atEnd()) {
        if (m_reader.isEndElement()) {
            m_reader.readNext();
            break;
        }

        if (m_reader.isStartElement()) {
            skipUnknownElement();
        } else {
            m_reader.readNext();
        }
    }
}

//
// class ContactDOMReader
//
ContactDOMReader::ContactDOMReader(QTreeWidget *tree) : m_tree(tree)
{}

ContactDOMReader::~ContactDOMReader()
{}

bool ContactDOMReader::readFile(const QString &fileName)
{
    QDomDocument doc;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file - " << fileName << " failed";
        return false;
    }

    QString strError;
    int errLine, errColumn;
    if( !doc.setContent(&file, &strError, & errLine, &errColumn) )
    {
        qDebug() << "parse xml error : " << strError << " line " << errLine << " col " << errColumn;
        file.close();
        return false;
    }
    file.close();

    QDomElement element = doc.documentElement().firstChildElement("people");
    while(!element.isNull())
    {
        QTreeWidgetItem * item = new QTreeWidgetItem(m_tree->invisibleRootItem());
        item->setText(0, element.attribute("name"));

        QDomElement child = element.firstChildElement("mobile");
        if(!child.isNull())
        {
            QTreeWidgetItem * mobileItem = new QTreeWidgetItem(item);
            mobileItem->setText(0, "mobile");
            mobileItem->setText(1, child.text());
        }

        element = element.nextSiblingElement("people");
    }
}
