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
    qreal nameHeight() const;
    qreal budgetHeight() const;
    qreal descriptionHeight() const;
    void paintName(QPainter * painter,
                   const QStyleOptionViewItem & option,
                   const QSharedPointer<IElanceJob> & job) const;
    void paintBudget(QPainter * painter,
                     const QStyleOptionViewItem & option,
                     const QSharedPointer<IElanceJob> & job) const;
    void paintDescription(QPainter * painter,
                          const QStyleOptionViewItem & option,
                          const QSharedPointer<IElanceJob> & job) const;

    QFont * m_nameFont;
    QFontMetricsF * m_nameFontMetrics;
    QFont * m_budgetFont;
    QFontMetricsF * m_budgetFontMetrics;
    QFont * m_descriptionFont;
    QFontMetricsF * m_descriptionFontMetrics;

    static const int m_minDescriptionCharactersCount = 100;
    static const int m_nameBottomMargin = 2;
    static const int m_budgetBottomMargin = 2;
    static const int m_itemBottomMargin = 10;
    static const int m_descriptionLinesCount = 2;
};
}

#endif // JOBITEMDELEGATE_H
