#ifndef JOBITEMDELEGATE_H
#define JOBITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace FreelanceNavigator
{
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
    int titleHeight(const QStyleOptionViewItem & option, const QString & title) const;
    static int titleFlags();

    static const QFont m_titleFont;

    static const int m_itemPadding = 5;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBITEMDELEGATE_H
