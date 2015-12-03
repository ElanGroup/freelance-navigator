#ifndef JOBITEM_H
#define JOBITEM_H

#include <QSharedPointer>
#include <QFont>
#include <QFontMetrics>
#include <QPen>
#include <QRect>

class QStyleOptionViewItem;
class QPainter;

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobItem
{
public:
    JobItem(const QSharedPointer<Job> job, QPaintDevice * paintDevice);
    JobItem(const JobItem &) = delete;
    JobItem & operator=(const JobItem &) = delete;

    QSharedPointer<Job> job() const;
    QSize sizeHint(const QStyleOptionViewItem & option);
    void paint(QPainter * painter, const QStyleOptionViewItem & option);

private:
    int titleHeight(const QStyleOptionViewItem & option);
    int infoRowHeight(const QStyleOptionViewItem & option);
    int descriptionHeight(const QStyleOptionViewItem & option);
    void paintTitle(QPainter * painter, const QStyleOptionViewItem & option);
    void paintInfoRow(QPainter * painter, const QStyleOptionViewItem & option);
    void paintDescription(QPainter * painter, const QStyleOptionViewItem & option);
    static int titleFlags();
    static int descriptionFlags();
    QString infoRowText();

    QSharedPointer<Job> m_job;
    QPaintDevice * m_paintDevice;

    static const QFont m_titleFont;
    static const QFont m_infoRowFont;
    static const QFont m_descriptionFont;

    static const QPen m_infoRowPen;

    const QFontMetrics m_titleFontMetrics;
    const QFontMetrics m_infoRowFontMetrics;
    const QFontMetrics m_descriptionFontMetrics;

    static const int m_itemPadding = 10;
    static const int m_infoRowPadding = 5;
    static const int m_descriptionLineCount = 3;

    QRect m_titleRect;
    QRect m_infoRowRect;
    QRect m_descriptionRect;

    QString m_infoRowText;
    QString m_descriptionText;

    QSize m_size;
};

} // namespace Widgets
} // namespace FreelanceNavigator

Q_DECLARE_METATYPE(QSharedPointer<FreelanceNavigator::Widgets::JobItem>)

#endif // JOBITEM_H
