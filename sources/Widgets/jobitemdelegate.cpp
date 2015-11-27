#include <QDebug>
#include <QPainter>
#include "jobitemdelegate.h"
#include "job.h"

using namespace FreelanceNavigator::Widgets;

const QFont JobItemDelegate::m_titleFont(QFont("Helvetica", 12, QFont::Bold));

JobItemDelegate::JobItemDelegate(QWidget * parent) : QStyledItemDelegate(parent)
{
}

void JobItemDelegate::paint(QPainter * painter,
                            const QStyleOptionViewItem & option,
                            const QModelIndex & index) const
{
    QSharedPointer<Job> job = qvariant_cast<QSharedPointer<Job> >(index.data());
    Q_ASSERT(job);

    paintTitle(painter, option, job->title());
}

QSize JobItemDelegate::sizeHint(const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    QSharedPointer<Job> job = qvariant_cast<QSharedPointer<Job> >(index.data());
    Q_ASSERT(job);

    return QSize(option.rect.width(), titleHeight(option, job->title()) + 2 * m_itemPadding);
}

void JobItemDelegate::paintTitle(QPainter * painter,
                                 const QStyleOptionViewItem & option,
                                 const QString & title) const
{
    QFontMetrics titleFontMetrics(m_titleFont, qobject_cast<QWidget *>(parent()));
    /*
     * Note that the given height is 0. That is because boundingRect() will return
     * the suitable height if the given geometry does not fit. And this is exactly
     * what we want.
     */
    QRect titleRect = titleFontMetrics.boundingRect(option.rect.left() + m_itemPadding,
                                                    option.rect.top() + m_itemPadding,
                                                    option.rect.width() - 2 * m_itemPadding,
                                                    0,
                                                    titleFlags(),
                                                    title);
    painter->save();
    painter->setFont(m_titleFont);
    painter->drawText(titleRect, titleFlags(), title);
    painter->restore();
}

int JobItemDelegate::titleHeight(const QStyleOptionViewItem & option, const QString & title) const
{
    QFontMetrics titleFontMetrics(m_titleFont, qobject_cast<QWidget *>(parent()));
    /*
     * Note that the given height is 0. That is because boundingRect() will return
     * the suitable height if the given geometry does not fit. And this is exactly
     * what we want.
     */
    QRect titleRect = titleFontMetrics.boundingRect(0,
                                                    0,
                                                    option.rect.width() - 2 * m_itemPadding,
                                                    0,
                                                    titleFlags(),
                                                    title);
    return titleRect.height();
}

int JobItemDelegate::titleFlags()
{
    return Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
}
