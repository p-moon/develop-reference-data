#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QTreeWidgetItem>

class ContactStreamReader
{
public:
    ContactStreamReader(QTreeWidget * tree);
    ~ContactStreamReader();

    bool readFile(const QString & fileName);

private:
    void readPeoplesElement();
    void readPeopleElement(QTreeWidgetItem * parent);
    void readMobileElement(QTreeWidgetItem * parent);
    void skipUnknownElement();

    QXmlStreamReader m_reader;
    QTreeWidget * m_tree;
};

class ContactDOMReader
{
public:
    ContactDOMReader(QTreeWidget * tree);
    ~ContactDOMReader();

    bool readFile(const QString & fileName);

private:
    QTreeWidget * m_tree;
};

#endif // WIDGET_H
