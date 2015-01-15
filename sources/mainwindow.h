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
class IElanceCategory;
class IElanceJobsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent = 0);
    ~MainWindow();

private slots:
    void showCategories(const QList<QSharedPointer<IElanceCategory> > & categories);
    void showJobs(const QSharedPointer<IElanceJobsPage> & jobs);
    void updateSubcategories(int categoryIndex);

private:
    void setupConnections();

    Ui::MainWindow * ui;
    ElanceApiClient * m_elanceApiClient;
    QHash<int, QList<QSharedPointer<IElanceCategory> > > m_subcategories;
};
}

#endif // MAINWINDOW_H
