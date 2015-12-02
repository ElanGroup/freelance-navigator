#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Upwork/upworksearchjobparameters.h"

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

private slots:
    void upworkJobTypeChanged(int index);
    void processUpworkError(Upwork::UpworkApiError error);
    void processUpworkWarning(Upwork::UpworkApiWarning warning);
    void loadUpworkCategories();
    void fillUpworkCategories(const QList<QSharedPointer<Upwork::UpworkCategory>> & categories);
    void updateUpworkSubcategories(int categoryIndex);
    void searchUpworkJobs();
    void finishUpworkJobSearch();
    void processMaxJobCount(int count);

private:
    void setupConnections();
    void setupUpworkFilters();
    void showAbout();
    void updateUpworkSearchButtonState();
    void fillUpworkSubcategories(int categoryIndex, bool loadSettings);
    Upwork::UpworkSearchJobParameters upworkSearchJobParameters() const;

    Ui::MainWindow * ui;
    Settings * m_settings;
    Upwork::UpworkApiClient * m_upworkApiClient;
    QHash<QString, QList<QSharedPointer<Upwork::UpworkCategory>>> m_upworkSubcategories;
};
}

#endif // MAINWINDOW_H
