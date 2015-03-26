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
class IElanceJob;
class JobsLoader;
class JobsManager;

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
    void jobTypeChanged(int index);
    void loadJobs();
    void updateJobControls();
    void finishJobsLoad();
    void showFirstPageOfJobs();
    void showPreviousPageOfJobs();
    void showNextPageOfJobs();
    void showLastPageOfJobs();
    void showRequestedPageOfJobs();
    void processError(ElanceApiClient::ElanceApiError error);

private:
    void setSubcategoriesFilter();
    void setJobTypeFilter();
    void setPostedDateFilter();
    void setBudgetFilter();
    void showJobs(int page);
    void updatePageButtons();
    void setupConnections();
    void loadSettings();
    void saveSettings();

    Ui::MainWindow * ui;
    ElanceApiClient * m_elanceApiClient;
    QHash<int, QList<QSharedPointer<IElanceCategory> > > m_subcategories;
    QStandardItemModel * m_jobsModel;
    JobsLoader * m_jobsLoader;
    JobsManager * m_jobsManager;
    int m_currentJobsPage;
    int m_minBudget;
    int m_maxBudget;
    int m_minHourlyRate;
    int m_maxHourlyRate;
    bool m_isErrorMessageShown;
};
}

#endif // MAINWINDOW_H
