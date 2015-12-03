#include <QDebug>
#include <QPainter>
#include "jobitemdelegate.h"
#include "jobitem.h"

using namespace FreelanceNavigator::Widgets;

JobItemDelegate::JobItemDelegate(QWidget * parent) : QStyledItemDelegate(parent)
{
}

void JobItemDelegate::paint(QPainter * painter,
                            const QStyleOptionViewItem & option,
                            const QModelIndex & index) const
{
    // Paint background.
    QStyledItemDelegate::paint(painter, option, index);

    QSharedPointer<JobItem> jobItem = qvariant_cast<QSharedPointer<JobItem>>(index.data());
    Q_ASSERT(jobItem);

    jobItem->paint(painter, option);
}

QSize JobItemDelegate::sizeHint(const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    QSharedPointer<JobItem> jobItem = qvariant_cast<QSharedPointer<JobItem>>(index.data());
    Q_ASSERT(jobItem);

    return jobItem->sizeHint(option);
}
