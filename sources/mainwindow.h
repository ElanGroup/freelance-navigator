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

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void fillCategories(const QList<QSharedPointer<IElanceCategory> > & categories);
    void showJobs(const QSharedPointer<IElanceJobsPage> & jobs);
    void fillSubcategories(int categoryIndex, bool loadSettings);
    void updateSubcategories(int categoryIndex);

private:
    void setupConnections();
    void saveSettings();

    Ui::MainWindow * ui;
    ElanceApiClient * m_elanceApiClient;
    QHash<int, QList<QSharedPointer<IElanceCategory> > > m_subcategories;
};
}

#endif // MAINWINDOW_H
