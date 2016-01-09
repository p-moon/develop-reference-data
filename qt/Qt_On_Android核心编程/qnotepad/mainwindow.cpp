#include "mainwindow.h"
#include <QFont>
#include <QInputDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_nStartNote(-1)
    , m_nEndNote(-1)
    , m_noteView(0)
    , m_framePen(QBrush(QColor::fromRgb(50, 50, 50)),1)
    , m_focusFramePen(QBrush(QColor::fromRgb(50, 50, 200)),4)
    , m_nPressRect(-1)
    , m_bMoving(false)
{
    QFont f(font());
    f.setPixelSize(16);
    setFont(f);
    m_background = new QPixmap(":/background.png");
    m_thumbBackground = new QPixmap(":/thumb_text_bg.png");
    m_thumbAdd = new QPixmap(":/thumb_add_new.png");
    QNote::loadNotes(m_notes);
    setContentsMargins(0,0,0,0);
}

MainWindow::~MainWindow()
{
    delete m_background;
    delete m_thumbAdd;
    delete m_thumbBackground;
    releaseGeometryRects();
    releaseNotes();
}

// reset to initial state
void MainWindow::onNotesUpdate()
{
    int count = m_thumbRects.size();
    int noteCount = m_notes.size();
    if(noteCount > 0){
        m_nStartNote = 0;
        m_nEndNote = qMin(noteCount - 1, count - 2);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    setupGeometry(e->size());
    if(m_noteView)m_noteView->setFixedSize(width(), height());
    QWidget::resizeEvent(e);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawBackground(painter);
    drawAddButton(painter);
    drawThumbnails(painter);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    e->accept();
    qDebug() << "mousePressEvent";
    m_pressedPos = e->pos();
    m_nPressRect = rectHitTest(m_pressedPos);
    if(m_nPressRect != -1) update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
    m_bMoving = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    QPoint releasePos = e->pos();
    if(!checkNoteAction(releasePos))
    {
        if(m_bMoving)
        {
            releasePos -= m_pressedPos;
            int xDistance = releasePos.x();
            int yDistance = releasePos.y();
            //qDebug() << "move distance - " << releasePos;
            if(xDistance >= 50 || yDistance >= 50)
            {
                // move to right or down
                goPrevPage();
            }
            else if(xDistance <= -50 || yDistance <= -50)
            {
                //move to left or up
                goNextPage();
            }
        }
    }
    m_bMoving = false;
    m_nPressRect = -1;
    update();
}

void MainWindow::releaseGeometryRects()
{
    int count = m_thumbRects.size();
    if(count > 0)
    {
        for(int i = 0; i < count; i++)
        {
            delete m_thumbRects.at(i);
        }
        m_thumbRects.clear();
    }
}

void MainWindow::setupGeometry(const QSize &size)
{
    releaseGeometryRects();
    int w = size.width();
    int h = size.height();
    int xStart;
    int yStart = 84;
    int thumbWidth = m_thumbBackground->width();
    int thumbHeight = m_thumbBackground->height();
    int colWidth = thumbWidth + 12;
    int rowHeight = thumbHeight + 10;
    int xEnd ;
    int yEnd = yStart + thumbHeight;
    QRect *pRect;
    do{
        xStart = 10;
        xEnd = xStart + thumbWidth;
        do{
            pRect = new QRect(xStart, yStart, thumbWidth, thumbHeight);
            m_thumbRects.append(pRect);
            xStart += colWidth;
            xEnd = xStart + thumbWidth;
        }while(xEnd <= w);

        yStart += rowHeight;
        yEnd = yStart + thumbHeight;
    }while(yEnd <= h);

    onNotesUpdate();
}

void MainWindow::drawBackground(QPainter &painter)
{
    int topHeight = 80;
    int widgetWidth = width();
    int widgetHeight = height();
    int imageWidth = m_background->width();
    int imageHeight = m_background->height();

    if(widgetWidth <= imageWidth )
    {
        if(widgetHeight <= imageHeight){
            painter.drawPixmap(0, 0, widgetWidth, widgetHeight,
                           *m_background, 0, 0, widgetWidth, widgetHeight);
        }else{
            painter.drawPixmap(0, 0, widgetWidth, topHeight,
                           *m_background, 0, 0, widgetWidth, topHeight);
            painter.drawPixmap(0, topHeight, widgetWidth, widgetHeight - topHeight,
                             *m_background, 0, topHeight, widgetWidth, imageHeight);
        }
    }else if(widgetWidth > imageWidth){
        //top
        int w = widgetWidth;
        int xStart = 0;
        do{
            painter.drawPixmap(xStart, 0, imageWidth, topHeight,
                               *m_background, 0, 0, imageWidth, topHeight);
            w -= imageWidth;
            xStart += imageWidth;
        }while(w > 0);

        int bottomHeight = widgetHeight - topHeight;
        if(widgetHeight <= imageHeight){
            painter.drawPixmap(0, topHeight, widgetWidth, bottomHeight,
                           *m_background, 0, topHeight, imageWidth, bottomHeight);
        }else{
            painter.drawPixmap(0, topHeight, widgetWidth, bottomHeight,
                             *m_background, 0, topHeight, imageWidth, imageHeight - topHeight);
        }
    }
}

void MainWindow::drawAddButton(QPainter &painter)
{
    if(m_thumbRects.size() == 0) return;
    QRect *pRect = m_thumbRects.at(0);
    painter.drawPixmap(*pRect, *m_thumbAdd);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(m_nPressRect == 0 ? m_focusFramePen : m_framePen);
    painter.drawRect(*pRect);
}

void MainWindow::drawThumbnails(QPainter &painter)
{
    if(m_nStartNote == -1) return;
    int size = m_thumbRects.size();
    QRect *pRect;
    QRect textRect;
    QRect titleRect;
    QNote * note;
    int j = m_nStartNote;
    for(int i = 1; i < size && j <= m_nEndNote; i++, j++)
    {
        pRect = m_thumbRects.at(i);
        painter.drawPixmap(*pRect, *m_thumbBackground);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(m_nPressRect == i ? m_focusFramePen : m_framePen);
        painter.drawRect(*pRect);

        note = m_notes.at(j);
        titleRect = *pRect;
        titleRect.setHeight(22);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(titleRect);
        painter.setPen(Qt::white);
        titleRect.adjust(2, 0, -2, 0);
        painter.drawText(titleRect,
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine
                         , note->m_strTitle);
        textRect = pRect->adjusted(2, 26, 2, 2);
        painter.setPen(Qt::black);
        painter.drawText(textRect
                         ,Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap
                         , note->m_strContent);

    }
}

void MainWindow::initializeNoteView()
{
    m_noteView = new NoteViewWidget(m_notes, this, Qt::FramelessWindowHint);
    m_noteView->setAttribute(Qt::WA_DeleteOnClose);
    m_noteView->setFixedSize(width(), height());
    connect(m_noteView, SIGNAL(notesUpdate()), this, SLOT(onNotesUpdate()));
}

int MainWindow::rectHitTest(QPoint &pos)
{
    int count = m_thumbRects.size();
    QRect * pRect = m_thumbRects.at(0);
    if(pRect->contains(pos))
    {
       return 0;
    }
    for(int i = 1, j = m_nStartNote; i < count && j <= m_nEndNote; i++, j++)
    {
        pRect = m_thumbRects.at(i);
        if(pRect->contains(pos))
        {
            return i;
        }
    }
    return -1;
}

void MainWindow::newNote()
{
    bool ok;
    QString text = QInputDialog::getText(this, "新建笔记",
                                            "请输入标题:", QLineEdit::Normal,
                                            "", &ok);
   if (ok && !text.isEmpty())
   {
       initializeNoteView();
       QNote * note = new QNote(text, "");
       m_noteView->setNote(note);
       m_noteView->show();
   }
}

void MainWindow::openNote(QNote *note)
{
    initializeNoteView();
    m_noteView->setNote(note);
    m_noteView->show();
}

bool MainWindow::checkNoteAction(QPoint &pos)
{
    int releaseRect = rectHitTest(pos);
    if(m_nPressRect == releaseRect)
    {
        if(m_nPressRect == 0)
        {
            newNote();
            return true;
        }
        else if(m_nPressRect > 0)
        {
            openNote(m_notes.at(m_nPressRect + m_nStartNote - 1));
            return true;
        }
    }
    return false;
}

bool MainWindow::goPrevPage()
{
    if(m_nStartNote > 0)
    {
        int itemsPerPage = m_thumbRects.size() - 1;
        m_nStartNote -= itemsPerPage;
        if(m_nStartNote < 0) m_nStartNote = 0;
        m_nEndNote = m_nStartNote + itemsPerPage - 1;
        return true;
    }
    return false;
}

bool MainWindow::goNextPage()
{
    int lastNoteIndex = m_notes.size() - 1;
    if(m_nEndNote < lastNoteIndex)
    {
        int itemsPerPage = m_thumbRects.size() - 1;
        m_nEndNote += itemsPerPage;
        if(m_nEndNote > lastNoteIndex ) m_nEndNote = lastNoteIndex;
        m_nStartNote = m_nEndNote - itemsPerPage + 1;
        return true;
    }
    return false;
}

void MainWindow::releaseNotes()
{
    int count = m_notes.size();
    for(int i = 0; i < count; i++)
    {
        delete m_notes.at(i);
    }
    m_notes.clear();
}
