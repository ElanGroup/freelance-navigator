#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

namespace FreelanceNavigator
{
class ElanceApiClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const ElanceApiClient & elanceApiClient, QWidget * parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow * ui;
    const ElanceApiClient & m_elanceApiClient;
};
}

#endif // MAINWINDOW_H
