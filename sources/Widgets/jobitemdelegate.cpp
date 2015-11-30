#include <QDebug>
#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include "jobitemdelegate.h"
#include "job.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

const QFont JobItemDelegate::m_titleFont(QStringLiteral("Helvetica"), 12, QFont::Bold);
const QFont JobItemDelegate::m_infoRowFont(QStringLiteral("Helvetica"), 9, QFont::DemiBold);
const QFont JobItemDelegate::m_descriptionFont(QStringLiteral("Helvetica"), 10);

const QPen JobItemDelegate::m_infoRowPen(Qt::gray);

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
    paintInfoRow(painter, option, job);
    paintDescription(painter, option);
}

QSize JobItemDelegate::sizeHint(const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    QSharedPointer<Job> job = qvariant_cast<QSharedPointer<Job> >(index.data());
    Q_ASSERT(job);

    int height = 2 * m_itemPadding + titleHeight(option, job->title());
    height += m_infoRowPadding + infoRowHeight(option);
    height += m_infoRowPadding + descriptionHeight(option, job->description());
    return QSize(option.rect.width(), height);
}

void JobItemDelegate::paintTitle(QPainter * painter,
                                 const QStyleOptionViewItem & option,
                                 const QString & title) const
{
    m_titleRect.translate(option.rect.left() + m_itemPadding,
                          option.rect.top() + m_itemPadding);

    painter->save();
    painter->setFont(m_titleFont);
    painter->drawText(m_titleRect, titleFlags(), title);
    painter->restore();
}

void JobItemDelegate::paintInfoRow(QPainter * painter,
                                   const QStyleOptionViewItem & option,
                                   const QSharedPointer<Job> & job) const
{
    m_infoRowRect.translate(option.rect.left() + m_itemPadding,
                            m_titleRect.bottom() + m_infoRowPadding);

    painter->save();
    painter->setFont(m_infoRowFont);
    painter->setPen(m_infoRowPen);
    QString text = job->type() == JobType::Fixed ? tr("Fixed Price") : tr("Hourly Rated");
    if (job->budget() > 0)
    {
        text += "    " + tr("Budget: $%1").arg(job->budget());
    }
    painter->drawText(m_infoRowRect, Qt::AlignLeft | Qt::AlignTop, text);
    painter->restore();
}

void JobItemDelegate::paintDescription(QPainter * painter, const QStyleOptionViewItem & option) const
{
    m_descriptionRect.translate(option.rect.left() + m_itemPadding,
                                m_infoRowRect.bottom() + m_infoRowPadding);

    painter->save();
    painter->setFont(m_descriptionFont);
    painter->drawText(m_descriptionRect, descriptionFlags(), m_descriptionText);
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
    m_titleRect = titleFontMetrics.boundingRect(0,
                                                0,
                                                option.rect.width() - 2 * m_itemPadding,
                                                0,
                                                titleFlags(),
                                                title);
    return m_titleRect.height();
}

int JobItemDelegate::infoRowHeight(const QStyleOptionViewItem & option) const
{
    QFontMetrics infoRowFontMetrics(m_infoRowFont, qobject_cast<QWidget *>(parent()));
    m_infoRowRect = QRect(0,
                          0,
                          option.rect.width() - 2 * m_itemPadding,
                          qMax(infoRowFontMetrics.height(), infoRowFontMetrics.lineSpacing()));
    return m_infoRowRect.height();
}

int JobItemDelegate::descriptionHeight(const QStyleOptionViewItem & option,
                                       const QString & description) const
{
    int textTotalWidth = 0;
    int lineWidth = option.rect.width() - 2 * m_itemPadding;
    QTextLayout descriptionLayout(description.simplified(),
                                  m_descriptionFont,
                                  qobject_cast<QWidget *>(parent()));
    descriptionLayout.beginLayout();
    for (int i = 0; i < m_descriptionLineCount - 1; ++i)
    {
        QTextLine line = descriptionLayout.createLine();
        if (!line.isValid())
        {
            // There is no text left to be inserted into the layout.
            break;
        }
        line.setLineWidth(lineWidth);
        textTotalWidth += line.naturalTextWidth();
    }
    descriptionLayout.endLayout();

    // Add space for last visible line.
    textTotalWidth += lineWidth;

    QFontMetrics descriptionFontMetrics(m_descriptionFont, qobject_cast<QWidget *>(parent()));
    m_descriptionText = descriptionFontMetrics.elidedText(description.simplified(),
                                                          Qt::ElideRight,
                                                          textTotalWidth);
    m_descriptionRect =
        descriptionFontMetrics.boundingRect(0,
                                            0,
                                            option.rect.width() - 2 * m_itemPadding,
                                            0,
                                            descriptionFlags(),
                                            m_descriptionText);
    return m_descriptionRect.height();
}

int JobItemDelegate::titleFlags()
{
    return Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
}

int JobItemDelegate::descriptionFlags()
{
    return Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
}
