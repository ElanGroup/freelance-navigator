#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include "jobitemdelegate.h"
#include "ielancejob.h"

using namespace FreelanceNavigator;

JobItemDelegate::JobItemDelegate(QWidget * jobsWidget, QObject * parent)
    : QStyledItemDelegate(parent),
      m_jobsWidget(jobsWidget),
      m_nameFont(new QFont("Helvetica", 12, QFont::Bold)),
      m_nameFontMetrics(new QFontMetricsF(*m_nameFont)),
      m_budgetFont(new QFont("Helvetica", 8, QFont::DemiBold)),
      m_budgetFontMetrics(new QFontMetricsF(*m_budgetFont)),
      m_descriptionFont(new QFont("Helvetica", 9)),
      m_descriptionFontMetrics(new QFontMetricsF(*m_descriptionFont)),
      m_mouseOverNameJobId(-1)
{
    m_nameFont->setUnderline(true);
}

JobItemDelegate::~JobItemDelegate()
{
    delete m_descriptionFontMetrics;
    delete m_descriptionFont;
    delete m_budgetFontMetrics;
    delete m_budgetFont;
    delete m_nameFontMetrics;
    delete m_nameFont;
}

void JobItemDelegate::paint(QPainter * painter,
                            const QStyleOptionViewItem & option,
                            const QModelIndex & index) const
{
    QSharedPointer<IElanceJob> job = qvariant_cast<QSharedPointer<IElanceJob> >(index.data());
    Q_ASSERT(job);

    paintName(painter, option, job);
    paintBudget(painter, option, job);
    paintDescription(painter, option, job);
}

QSize JobItemDelegate::sizeHint(const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    int minWidth = m_minDescriptionCharactersCount * m_descriptionFontMetrics->averageCharWidth();
    QSize defaultSize = QStyledItemDelegate::sizeHint(option, index);
    int width = qMax(minWidth, defaultSize.width());
    int height = nameHeight() + budgetHeight() + descriptionHeight();
    height += m_itemBottomMargin;
    return QSize(width, height);
}

bool JobItemDelegate::editorEvent(QEvent * event,
                                  QAbstractItemModel * model,
                                  const QStyleOptionViewItem & option,
                                  const QModelIndex & index)
{
    if (event->type() == QEvent::MouseMove)
    {
        QSharedPointer<IElanceJob> job = qvariant_cast<QSharedPointer<IElanceJob> >(index.data());
        Q_ASSERT(job);

        QRectF itemRect = option.rect;
        QRectF nameRect(itemRect.x(),
                        itemRect.y(),
                        m_nameFontMetrics->width(job->name()),
                        m_nameFontMetrics->height());
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        bool isMouseOverName = nameRect.contains(mouseEvent->localPos());
        m_jobsWidget->setCursor(QCursor(isMouseOverName ? Qt::PointingHandCursor
                                                        : Qt::ArrowCursor));
        m_mouseOverNameJobId = isMouseOverName ? job->jobId() : -1;
        return true;
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            QSharedPointer<IElanceJob> job =
                qvariant_cast<QSharedPointer<IElanceJob> >(index.data());
            Q_ASSERT(job);

            QDesktopServices::openUrl(job->url());
            m_visitedJobs.insert(job->jobId());
            m_mouseOverNameJobId = -1;
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

qreal JobItemDelegate::nameHeight() const
{
    return m_nameFontMetrics->height() + m_nameBottomMargin;
}

qreal JobItemDelegate::budgetHeight() const
{
    return m_budgetFontMetrics->height() + m_budgetBottomMargin;
}

qreal JobItemDelegate::descriptionHeight() const
{
    return m_descriptionFontMetrics->lineSpacing() * m_descriptionLinesCount;
}

void JobItemDelegate::paintName(QPainter * painter,
                                const QStyleOptionViewItem & option,
                                const QSharedPointer<IElanceJob> & job) const
{
    QRectF rect = option.rect;
    rect.setHeight(m_nameFontMetrics->height());

    QColor color = m_visitedJobs.contains(job->jobId()) ? option.palette.linkVisited().color()
                                                        : option.palette.link().color();
    if (m_mouseOverNameJobId == job->jobId())
    {
        color = color.lighter();
    }

    painter->save();
    painter->setPen(color);
    painter->setFont(*m_nameFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, job->name());
    painter->restore();
}

void JobItemDelegate::paintBudget(QPainter * painter,
                                  const QStyleOptionViewItem & option,
                                  const QSharedPointer<IElanceJob> & job) const
{
    QRectF rect = option.rect;
    rect.translate(0, nameHeight());
    rect.setHeight(m_budgetFontMetrics->height());

    QString text = QString("%1: %2").arg(job->isHourly() ? tr("Hourly Rate") : tr("Fixed Price"),
                                         job->budget());

    painter->save();
    painter->setPen(option.palette.windowText().color());
    painter->setFont(*m_budgetFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, text);
    painter->restore();
}

void JobItemDelegate::paintDescription(QPainter * painter,
                                       const QStyleOptionViewItem & option,
                                       const QSharedPointer<IElanceJob> & job) const
{
    QRectF rect = option.rect;
    rect.translate(0, nameHeight() + budgetHeight());
    rect.setHeight(descriptionHeight());

    painter->save();
    painter->setPen(option.palette.windowText().color());
    painter->setFont(*m_descriptionFont);
    painter->drawText(rect,
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                      job->description().simplified());
    painter->restore();
}
