#include "note.h"
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>

void QNote::loadNotes(QList<QNote *> &notes)
{
    QFile file(QString("%1/qnote.txt").arg(QDir::currentPath()));
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open qnote.txt for read - " << file.errorString();
        return;
    }
    QTextStream textStream(&file);
    textStream.setAutoDetectUnicode(true);
    QString line;
    QString title;
    QString content;
    QNote *note;
    while(!textStream.atEnd())
    {
        line = textStream.readLine();
        if(line.startsWith(NEW_NOTE_PREFIX))
        {
            if(!title.isEmpty() && !content.isEmpty())
            {
                note = new QNote(title.trimmed(), content);
                notes.append(note);
            }
            title.clear();
            content.clear();
            title = line.mid( NEW_NOTE_PREFIX_LEN );
            continue;
        }
        content += line;
        content += '\n';
    }
    if(!title.isEmpty() && !content.isEmpty())
    {
        note = new QNote(title.trimmed(), content);
        notes.append(note);
    }

    file.close();
}

void QNote::saveNotes(QList<QNote *> &notes)
{
    QFile file(QString("%1/qnote.txt").arg(QDir::currentPath()));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "can't open qnote.txt for write - " << file.errorString();
        return;
    }
    QTextStream textStream(&file);
    textStream.setCodec("utf-8");
    int count = notes.size();
    QNote *note;
    for(int i = 0 ; i < count; i++)
    {
        note = notes.at(i);
        textStream << NEW_NOTE_PREFIX << note->m_strTitle << "\n";
        textStream << note->m_strContent << "\n";
    }
    file.close();
}
