#ifndef JOBITEMDELEGATE_H
#define JOBITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace FreelanceNavigator
{
class IElanceJob;

class JobItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit JobItemDelegate(QObject * parent = 0);
    ~JobItemDelegate();

    virtual void paint(QPainter * painter,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

private:
    static QFont nameFont(const QStyleOptionViewItem & option);
    static QFont budgetFont(const QStyleOptionViewItem & option);
    static int nameHeight(const QStyleOptionViewItem & option);
    static int budgetHeight(const QStyleOptionViewItem & option);
    void paintName(QPainter * painter,
                   const QStyleOptionViewItem & option,
                   const QSharedPointer<IElanceJob> & job) const;
    void paintBudget(QPainter * painter,
                     const QStyleOptionViewItem & option,
                     const QSharedPointer<IElanceJob> & job) const;
    void paintDescription(QPainter * painter,
                          const QStyleOptionViewItem & option,
                          const QSharedPointer<IElanceJob> & job) const;

    static const int m_minDescriptionCharactersCount = 100;
    static const int m_nameBottomMargin = 2;
    static const int m_budgetBottomMargin = 2;
    static const int m_itemBottomMargin = 10;
    static const int m_descriptionLinesCount = 2;
};
}

#endif // JOBITEMDELEGATE_H
