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
    void loadJobs();
    void showJobs(const QSharedPointer<IElanceJobsPage> & jobs);
    void processError(ElanceApiClient::ElanceApiError error);

private:
    void setupConnections();
    void saveSettings();

    Ui::MainWindow * ui;
    ElanceApiClient * m_elanceApiClient;
    QHash<int, QList<QSharedPointer<IElanceCategory> > > m_subcategories;
    QStandardItemModel * m_jobsModel;
};
}

#endif // MAINWINDOW_H
