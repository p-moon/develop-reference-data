#include "viewWidget.h"
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include <QSize>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QRgb>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>
#include <QKeyEvent>
#include <QDebug>

NoteViewWidget::NoteViewWidget(QList<QNote *> &notes, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_note(0), m_notes(notes)
    , m_bTextChanged(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(false);
    setupGUI();
    setFocus();
}

NoteViewWidget::~NoteViewWidget()
{
}

void NoteViewWidget::setNote(QNote *note)
{
    m_note = note;
    m_titleLabel->setText(note->m_strTitle);
    m_edit->setPlainText(note->m_strContent);
    m_bTextChanged = false;
}

void NoteViewWidget::onThumbnails()
{
    checkChangeSave();
    close();
}

void NoteViewWidget::onSave()
{
    m_note->m_strContent = m_edit->toPlainText();
    if(-1 == m_notes.indexOf(m_note))
    {
        m_notes.prepend(m_note);
        emit notesUpdate();
    }
    QNote::saveNotes(m_notes);
    m_bTextChanged = false;
}

void NoteViewWidget::onDelete()
{
    if( m_notes.removeOne(m_note) )
    {
        QNote::saveNotes(m_notes);
        emit notesUpdate();
    }
    delete m_note;
    if(m_notes.size() > 0)
    {
        setNote(m_notes.at(0));
    }
    else
    {
        m_note = 0;
        m_titleLabel->setText("");
        m_edit->clear();
        m_bTextChanged = false;
    }
}

void NoteViewWidget::onAdd()
{
    checkChangeSave();
    bool ok;
    QString text = QInputDialog::getText(this, "新建笔记",
                                         "请输入标题:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        m_edit->clear();
        m_titleLabel->setText(text);
        m_note = new QNote(text, "");
    }
}

void NoteViewWidget::onTextChanged()
{
    m_bTextChanged = true;
}

void NoteViewWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QRect totalRect = rect();
    QLinearGradient gradient(0, 0, 0, totalRect.bottom());
    gradient.setColorAt(0, QColor::fromRgb(245,235,163));
    gradient.setColorAt(1, QColor::fromRgb(230,179,85));
    QBrush bkgnd(gradient);
    painter.setBrush(bkgnd);
    painter.drawRect(totalRect);

    //toolbar, 74 pixels
    QRect toolbar(totalRect);
    toolbar.setHeight(72);
    painter.setPen(Qt::NoPen);
    QBrush brush(QColor::fromRgb(40, 40, 40, 190));
    painter.setBrush(brush);
    painter.drawRect(toolbar);

    //bottom title 34 pixels
    QRect bottomTitle(totalRect);
    bottomTitle.setTop(bottomTitle.bottom() - 34);
    painter.drawRect(bottomTitle);
}

void NoteViewWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Back:
        e->accept();
        break;
    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

void NoteViewWidget::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Back:
        e->accept();
        onThumbnails();
        break;
    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

bool NoteViewWidget::eventFilter(QObject *obj, QEvent *event)
{
    QEvent::Type t = event->type();
    switch(t)
    {
    case QEvent::KeyPress:
        if((static_cast<QKeyEvent *>(event))->key() == Qt::Key_Back)
        {
            event->accept();
            return true;
        }
        break;
    case QEvent::KeyRelease:
        if((static_cast<QKeyEvent *>(event))->key() == Qt::Key_Back)
        {
            qDebug() << "event filter for - " << obj;
            event->accept();
            onThumbnails();
            return true;
        }
        break;
    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}

void NoteViewWidget::setupGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(4);
    mainLayout->setSpacing(4);
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    mainLayout->addLayout(toolbarLayout);
    toolbarLayout->addStretch(1);

    m_thumbnailsButton = new ImageButton(QPixmap(":/thumbnail.png"),
                                         QPixmap(":/thumbnail_pressed.png"));
    m_thumbnailsButton->installEventFilter(this);
    m_thumbnailsButton->setFixedSize(66, 66);
    //m_thumbnailsButton->setStyleSheet("*{background-color: transparent;}");
    connect(m_thumbnailsButton, SIGNAL(clicked()), this, SLOT(onThumbnails()));
    toolbarLayout->addWidget(m_thumbnailsButton);


    m_saveButton = new ImageButton(QPixmap(":/save.png"),
                                   QPixmap(":/save_pressed.png"));
    m_saveButton->installEventFilter(this);
    m_saveButton->setFixedSize(60, 60);
    //m_saveButton->setStyleSheet("*{background-color: transparent;}");
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(onSave()));
    toolbarLayout->addWidget(m_saveButton, 0, Qt::AlignCenter);

    m_deleteButton = new ImageButton(QPixmap(":/delete.png"),
                                     QPixmap(":/delete_pressed.png"));
    m_deleteButton->installEventFilter(this);
    m_deleteButton->setFixedSize(66, 66);
    //m_deleteButton->setStyleSheet("*{background-color: transparent;}");
    connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(onDelete()));
    toolbarLayout->addWidget(m_deleteButton);

    m_addButton = new ImageButton(QPixmap(":/add.png"),
                                  QPixmap(":/add_pressed.png"));
    m_addButton->installEventFilter(this);
    m_addButton->setFixedSize(66, 66);
    //m_addButton->setStyleSheet("*{background-color: transparent;}");
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(onAdd()));
    toolbarLayout->addWidget(m_addButton);

    m_edit = m_note ? new QTextEdit(m_note->m_strContent) : new QTextEdit();
    m_edit->installEventFilter(this);
    m_edit->setFrameShape(QFrame::NoFrame);
    connect(m_edit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    m_edit->setStyleSheet("*{color:black; background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(245,235,163), stop: 1 rgb(230,179,85));}");
    mainLayout->addWidget(m_edit, 1);

    m_titleLabel = m_note ? new QLabel(m_note->m_strTitle) : new QLabel();
    m_titleLabel->setStyleSheet("*{color: white; background-color: transparent}");
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_titleLabel->setFixedHeight(30);
    m_titleLabel->setAutoFillBackground(false);
    mainLayout->addWidget(m_titleLabel);
}

void NoteViewWidget::checkChangeSave()
{
    if(m_bTextChanged)
    {
        if( QMessageBox::question(this, "保存确认", "要保存修改吗?")
                == QMessageBox::Yes )
        {
            m_note->m_strContent = m_edit->toPlainText();
            if(-1 == m_notes.indexOf(m_note))
            {
                m_notes.prepend(m_note);
                qDebug() << "add one note, notify";
                emit notesUpdate();
            }
            QNote::saveNotes(m_notes);
        }
        else
        {
            if(-1 == m_notes.indexOf(m_note))
            {
                delete m_note;
                m_note = 0;
            }
        }
        m_bTextChanged = false;
    }
}
