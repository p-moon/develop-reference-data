#ifndef LINEARLABEL_H
#define LINEARLABEL_H
#include <QWidget>
#include <QColor>
#include <QPixmap>
#include <QLinearGradient>
#include <QSize>

class LinearLabel : public QWidget
{
    Q_OBJECT
public:
    LinearLabel(const QString &text, QWidget *parent = 0);
    ~LinearLabel();

    void setBackgroundColor(const QColor & color)
    {
        m_backgroundColor = color;
    }
    void setBackgroundImage(const QPixmap &pixmap)
    {
        m_backgroundPixmap = pixmap;
    }
    void setTextColor(const QColor &color)
    {
        m_textColor = color;
    }

    void setColorAt(qreal position, const QColor & color);
    void clearGradient();

    void setAlignment(Qt::AlignmentFlag flag)
    {
        m_flags = flag;
    }

    void setPenWidth(qreal width)
    {
        m_penWidth = width;
    }

    QSize sizeHint() const;

public slots:
    void setText(const QString &text)
    {
        m_text = text;
        update();
    }

protected:
    void paintEvent(QPaintEvent *e);

private:
    QString m_text;
    QColor m_backgroundColor;
    QPixmap m_backgroundPixmap;
    QColor m_textColor;
    QLinearGradient *m_linearGradient;
    int m_flags;
    qreal m_penWidth;
};

#endif // LINEARLABEL_H
