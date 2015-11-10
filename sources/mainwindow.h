#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class QStandardItemModel;

namespace FreelanceNavigator
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:

private:
    Ui::MainWindow * ui;
    QStandardItemModel * m_jobsModel;
};
}

#endif // MAINWINDOW_H
