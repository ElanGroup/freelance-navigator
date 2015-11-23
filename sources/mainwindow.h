#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

namespace FreelanceNavigator
{
namespace Upwork
{
class UpworkApiClient;
enum class UpworkApiError;
enum class UpworkApiWarning;
}

class Settings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void processUpworkError(FreelanceNavigator::Upwork::UpworkApiError error);
    void processUpworkWarning(FreelanceNavigator::Upwork::UpworkApiWarning warning);
    void loadUpworkCategories();

private:
    void setupConnections();
    void showAbout();

    Ui::MainWindow * ui;
    Settings * m_settings;
    Upwork::UpworkApiClient * m_upworkApiClient;
};
}

#endif // MAINWINDOW_H
