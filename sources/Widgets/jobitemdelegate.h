#ifndef JOBITEMDELEGATE_H
#define JOBITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit JobItemDelegate(QWidget * parent = 0);
    JobItemDelegate(const JobItemDelegate &) = delete;
    JobItemDelegate& operator=(const JobItemDelegate &) = delete;

    virtual void paint(QPainter * painter,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem & option,
                           const QModelIndex & index) const override;

private:
    void paintTitle(QPainter * painter,
                    const QStyleOptionViewItem & option,
                    const QString & title) const;
    void paintInfoRow(QPainter * painter,
                      const QStyleOptionViewItem & option,
                      const QSharedPointer<Job> & job) const;
    void paintDescription(QPainter * painter, const QStyleOptionViewItem & option) const;
    int titleHeight(const QStyleOptionViewItem & option, const QString & title) const;
    int infoRowHeight(const QStyleOptionViewItem & option) const;
    int descriptionHeight(const QStyleOptionViewItem & option, const QString & description) const;
    static int titleFlags();
    static int descriptionFlags();

    static const QFont m_titleFont;
    static const QFont m_infoRowFont;
    static const QFont m_descriptionFont;

    static const QPen m_infoRowPen;

    static const int m_itemPadding = 10;
    static const int m_infoRowPadding = 5;
    static const int m_descriptionLineCount = 3;

    mutable QRect m_titleRect;
    mutable QRect m_infoRowRect;
    mutable QRect m_descriptionRect;

    mutable QString m_descriptionText;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBITEMDELEGATE_H
