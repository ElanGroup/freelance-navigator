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
class UpworkCategory;
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
    void processUpworkError(Upwork::UpworkApiError error);
    void processUpworkWarning(Upwork::UpworkApiWarning warning);
    void loadUpworkCategories();
    void fillUpworkCategories(const QList<QSharedPointer<Upwork::UpworkCategory>> & categories);
    void updateUpworkSubcategories(int categoryIndex);

private:
    void setupConnections();
    void showAbout();
    void fillUpworkSubcategories(int categoryIndex, bool loadSettings);

    Ui::MainWindow * ui;
    Settings * m_settings;
    Upwork::UpworkApiClient * m_upworkApiClient;
    QHash<QString, QList<QSharedPointer<Upwork::UpworkCategory>>> m_upworkSubcategories;
};
}

#endif // MAINWINDOW_H
