#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

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
    void setupConnections();
    void showAbout();

    Ui::MainWindow * ui;
};
}

#endif // MAINWINDOW_H
