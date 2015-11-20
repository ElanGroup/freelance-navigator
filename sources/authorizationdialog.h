#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

class QWebEngineView;
class QProgressBar;

namespace FreelanceNavigator
{

class AuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationDialog(const QString & serviceName,
                                 const QUrl & authorizationUrl,
                                 QWidget * parent = 0);
    ~AuthorizationDialog();

signals:
    void urlChanged(const QUrl & url) const;

private:
    void setupLayout();
    void setupConnections();

    QWebEngineView * m_webView;
    QProgressBar * m_progressBar;
};

} // namespace FreelanceNavigator

#endif // AUTHORIZATIONDIALOG_H
