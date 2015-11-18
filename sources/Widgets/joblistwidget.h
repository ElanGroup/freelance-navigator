#ifndef JOBLISTWIDGET_H
#define JOBLISTWIDGET_H

#include <QWidget>

namespace Ui
{
class JobListWidget;
}

namespace FreelanceNavigator
{
namespace Widgets
{

class JobListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JobListWidget(QWidget * parent = 0);
    ~JobListWidget();

signals:

private:
    Ui::JobListWidget * ui;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBLISTWIDGET_H
