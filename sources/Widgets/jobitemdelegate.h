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
    explicit JobItemDelegate(QObject * parent = 0);
    JobItemDelegate(const JobItemDelegate &) = delete;
    JobItemDelegate& operator=(const JobItemDelegate &) = delete;

};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBITEMDELEGATE_H
