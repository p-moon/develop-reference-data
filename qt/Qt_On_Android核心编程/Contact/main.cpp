#include <QtGui/QApplication>
#include <QTreeWidget>
#include "widget.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDomDocument>
#include <QDomNode>

bool writeXmlByStreamWriter(const QString & fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument(); //[1]
        writer.writeStartElement("peoples"); //[2]
            writer.writeStartElement("people");//[3]
                writer.writeAttribute("name", "Mary");//[4]
                writer.writeTextElement("mobile", "13502901234");
            writer.writeEndElement();//[3+]
        writer.writeEndElement(); //[2+]
    writer.writeEndDocument();//[1+]

    file.close();
}

bool writeXmlByDOM(const QString & fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream outStream(&file);

    QDomDocument doc;
    QDomNode node = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=UTF-8");
    doc.insertBefore(node, doc.firstChild());

    QDomElement peoples = doc.createElement("peoples");
    doc.appendChild(peoples);

    QDomElement people = doc.createElement("people");
    people.setAttribute("name", "Mary");
    peoples.appendChild(people);

    QDomElement mobile = doc.createElement("mobile");
    people.appendChild(mobile);

    QDomText text = doc.createTextNode("13502901234");
    mobile.appendChild(text);

    doc.save(outStream,4);
    file.close();

    return true;
}

bool writeXmlByHand(const QString & fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream outStream(&file);
    outStream.setCodec("UTF-8");

    outStream << "<?xml version=\"1.0\" encoding=UTF-8?>\n"
        << "<peoples>\n"
        << "    <people name=\"Mary\">\n"
        << "        <mobile>13502901234</mobile>\n"
        << "    </people>\n"
        << "</peoples>\n";

    file.close();
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTreeWidget tree;
    tree.setColumnCount(2);
    tree.setHeaderHidden(true);
#if 1
    ContactStreamReader reader(&tree);
    reader.readFile(argv[1]);
#else
    ContactDOMReader reader(&tree);
    reader.readFile(argv[1]);
#endif
    tree.show();
    //writeXmlByStreamWriter("demo.xml");
    //writeXmlByDOM("demo2.xml");
    writeXmlByHand("demo3.xml");
    return a.exec();
}
