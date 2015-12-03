#include <QStyleOptionViewItem>
#include <QTextLayout>
#include <QPainter>
#include <QPaintDevice>
#include <QDateTime>
#include "jobitem.h"
#include "job.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

const QFont JobItem::m_titleFont(QStringLiteral("Helvetica"), 12, QFont::Bold);
const QFont JobItem::m_infoRowFont(QStringLiteral("Helvetica"), 9, QFont::DemiBold);
const QFont JobItem::m_descriptionFont(QStringLiteral("Helvetica"), 10);

const QPen JobItem::m_infoRowPen(Qt::gray);

JobItem::JobItem(const QSharedPointer<Job> job, QPaintDevice * paintDevice) :
    m_job(job),
    m_paintDevice(paintDevice),
    m_titleFontMetrics(QFontMetrics(m_titleFont, paintDevice)),
    m_infoRowFontMetrics(QFontMetrics(m_infoRowFont, paintDevice)),
    m_descriptionFontMetrics(QFontMetrics(m_descriptionFont, paintDevice))
{
}

QSharedPointer<Job> JobItem::job() const
{
    return m_job;
}

QSize JobItem::sizeHint(const QStyleOptionViewItem & option)
{
    if (m_size.width() == option.rect.width())
    {
        return m_size;
    }

    int height = 2 * m_itemPadding + titleHeight(option);
    height += m_infoRowPadding + infoRowHeight(option);
    height += m_infoRowPadding + descriptionHeight(option);
    m_size = QSize(option.rect.width(), height);
    return m_size;
}

void JobItem::paint(QPainter * painter, const QStyleOptionViewItem & option)
{
    paintTitle(painter, option);
    paintInfoRow(painter, option);
    paintDescription(painter, option);
}

int JobItem::titleHeight(const QStyleOptionViewItem & option)
{
    /*
     * Note that the given height is 0. That is because boundingRect() will return
     * the suitable height if the given geometry does not fit. And this is exactly
     * what we want.
     */
    m_titleRect = m_titleFontMetrics.boundingRect(0,
                                                  0,
                                                  option.rect.width() - 2 * m_itemPadding,
                                                  0,
                                                  titleFlags(),
                                                  m_job->title());
    return m_titleRect.height();
}

int JobItem::infoRowHeight(const QStyleOptionViewItem & option)
{
    m_infoRowRect = QRect(0,
                          0,
                          option.rect.width() - 2 * m_itemPadding,
                          qMax(m_infoRowFontMetrics.height(), m_infoRowFontMetrics.lineSpacing()));
    return m_infoRowRect.height();
}

int JobItem::descriptionHeight(const QStyleOptionViewItem & option)
{
    int textTotalWidth = 0;
    int lineWidth = option.rect.width() - 2 * m_itemPadding;
    QString description = m_job->description().simplified();
    QTextLayout descriptionLayout(description, m_descriptionFont, m_paintDevice);
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

    m_descriptionText = m_descriptionFontMetrics.elidedText(description,
                                                            Qt::ElideRight,
                                                            textTotalWidth);
    m_descriptionRect =
        m_descriptionFontMetrics.boundingRect(0,
                                              0,
                                              option.rect.width() - 2 * m_itemPadding,
                                              0,
                                              descriptionFlags(),
                                              m_descriptionText);
    return m_descriptionRect.height();
}

void JobItem::paintTitle(QPainter * painter, const QStyleOptionViewItem & option)
{
    m_titleRect.moveTo(option.rect.left() + m_itemPadding, option.rect.top() + m_itemPadding);

    painter->save();
    painter->setFont(m_titleFont);
    painter->drawText(m_titleRect, titleFlags(), m_job->title());
    painter->restore();
}

void JobItem::paintInfoRow(QPainter * painter, const QStyleOptionViewItem & option)
{
    m_infoRowRect.moveTo(option.rect.left() + m_itemPadding,
                         m_titleRect.bottom() + m_infoRowPadding);

    painter->save();
    painter->setFont(m_infoRowFont);
    painter->setPen(m_infoRowPen);
    painter->drawText(m_infoRowRect, Qt::AlignLeft | Qt::AlignTop, infoRowText());
    painter->restore();
}

void JobItem::paintDescription(QPainter * painter, const QStyleOptionViewItem & option)
{
    m_descriptionRect.moveTo(option.rect.left() + m_itemPadding,
                             m_infoRowRect.bottom() + m_infoRowPadding);

    painter->save();
    painter->setFont(m_descriptionFont);
    painter->drawText(m_descriptionRect, descriptionFlags(), m_descriptionText);
    painter->restore();
}

int JobItem::titleFlags()
{
    return Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
}

int JobItem::descriptionFlags()
{
    return Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
}

QString JobItem::infoRowText()
{
    if (m_infoRowText.isEmpty())
    {
        QByteArray text;
        text.append(m_job->type() == JobType::Fixed ? QObject::tr("Fixed Price")
                                                    : QObject::tr("Hourly Rated"));
        if (m_job->budget() > 0)
        {
            text.append("    ");
            text.append(QObject::tr("Budget: $%1").arg(m_job->budget()));
        }
        text.append("    ");
        QDateTime localPostedDate(m_job->postedDate().toLocalTime());
        QString localPostedDateString = localPostedDate.toString(Qt::DefaultLocaleShortDate);
        text.append(QObject::tr("Posted at: %1").arg(localPostedDateString));
        m_infoRowText = text;
    }
    return m_infoRowText;
}
