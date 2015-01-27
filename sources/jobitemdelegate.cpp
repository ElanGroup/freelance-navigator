#include <QDebug>
#include <QPainter>
#include <QPalette>
#include "jobitemdelegate.h"
#include "ielancejob.h"

using namespace FreelanceNavigator;

JobItemDelegate::JobItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent)
{
}

JobItemDelegate::~JobItemDelegate()
{
}

void JobItemDelegate::paint(QPainter * painter,
                            const QStyleOptionViewItem & option,
                            const QModelIndex & index) const
{
    QSharedPointer<IElanceJob> job = qvariant_cast<QSharedPointer<IElanceJob> >(index.data());
    Q_ASSERT(job);

    painter->save();

    painter->setPen(option.palette.windowText().color());
    paintName(painter, option, job);
    paintBudget(painter, option, job);
    paintDescription(painter, option, job);

    painter->restore();
}

QSize JobItemDelegate::sizeHint(const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    QFontMetrics descriptionFontMetrics = option.fontMetrics;
    int minWidth = m_minDescriptionCharactersCount * descriptionFontMetrics.averageCharWidth();
    QSize defaultSize = QStyledItemDelegate::sizeHint(option, index);
    int width = qMax(minWidth, defaultSize.width());
    int height = nameHeight(option) + budgetHeight(option);
    height += descriptionFontMetrics.height() * m_descriptionLinesCount;
    height += m_itemBottomMargin;
    return QSize(width, height);
}

QFont JobItemDelegate::nameFont(const QStyleOptionViewItem & option)
{
    QFont nameFont = option.font;
    nameFont.setPointSizeF(QFontInfo(nameFont).pointSizeF() + 1);
    nameFont.setBold(true);
    return nameFont;
}

QFont JobItemDelegate::budgetFont(const QStyleOptionViewItem & option)
{
    QFont budgetFont = option.font;
    budgetFont.setPointSizeF(QFontInfo(budgetFont).pointSizeF() - 1);
    budgetFont.setWeight(QFont::DemiBold);
    return budgetFont;
}

int JobItemDelegate::nameHeight(const QStyleOptionViewItem & option)
{
    return QFontMetrics(nameFont(option)).height() + m_nameBottomMargin;
}

int JobItemDelegate::budgetHeight(const QStyleOptionViewItem & option)
{
    return QFontMetrics(budgetFont(option)).height() + m_budgetBottomMargin;
}

void JobItemDelegate::paintName(QPainter * painter,
                                const QStyleOptionViewItem & option,
                                const QSharedPointer<IElanceJob> & job) const
{
    QFont defaultFont = painter->font();
    QFont font = nameFont(option);
    QRectF rect = option.rect;
    rect.setHeight(QFontMetrics(font).height());
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, job->name());
    painter->setFont(defaultFont);
}

void JobItemDelegate::paintBudget(QPainter * painter,
                                  const QStyleOptionViewItem & option,
                                  const QSharedPointer<IElanceJob> & job) const
{
    QFont defaultFont = painter->font();
    QFont font = budgetFont(option);
    QRectF rect = option.rect;
    rect.translate(0, nameHeight(option));
    rect.setHeight(QFontMetrics(font).height());
    painter->setFont(font);
    QString text = QString("%1: %2").arg(job->isHourly() ? tr("Hourly Rate") : tr("Fixed Price"),
                                         job->budget());
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, text);
    painter->setFont(defaultFont);
}

void JobItemDelegate::paintDescription(QPainter * painter,
                                       const QStyleOptionViewItem & option,
                                       const QSharedPointer<IElanceJob> & job) const
{
    QRectF rect = option.rect;
    rect.translate(0, nameHeight(option) + budgetHeight(option));
    rect.setHeight(option.fontMetrics.height() * m_descriptionLinesCount);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, job->description());
}
