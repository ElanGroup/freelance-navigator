#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "elanceapiclient.h"

namespace Ui
{
class MainWindow;
}

class QStandardItemModel;

namespace FreelanceNavigator
{
class IElanceCategory;
class IElanceJobsPage;
class JobsLoader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void fillCategories(const QList<QSharedPointer<IElanceCategory> > & categories);
    void fillSubcategories(int categoryIndex, bool loadSettings);
    void updateSubcategories(int categoryIndex);
    void editElanceSettings();
    void showAbout();
    void logout();
    void setFiltersAndLoadJobs();
    void loadFirstPageOfJobs();
    void loadPreviousPageOfJobs();
    void loadNextPageOfJobs();
    void processLoadedJobs(bool isOk);
    void processError(ElanceApiClient::ElanceApiError error);

private:
    void loadJobs(int page);
    void setupConnections();
    void loadSettings();
    void saveSettings();

    Ui::MainWindow * ui;
    ElanceApiClient * m_elanceApiClient;
    QHash<int, QList<QSharedPointer<IElanceCategory> > > m_subcategories;
    QStandardItemModel * m_jobsModel;
    JobsLoader * m_jobsLoader;
};
}

#endif // MAINWINDOW_H
