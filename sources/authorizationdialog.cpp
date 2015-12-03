#include <QDebug>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QLayout>
#include <QProgressBar>
#include "authorizationdialog.h"

using namespace FreelanceNavigator;

AuthorizationDialog::AuthorizationDialog(const QString & serviceName,
                                         const QUrl & authorizationUrl,
                                         QWidget * parent) :
    QDialog(parent),
    m_webView(new QWebEngineView(this)),
    m_progressBar(new QProgressBar(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowState(windowState() | Qt::WindowMaximized);
    setWindowTitle(tr("Freelance Navigator - %1 authorization").arg(serviceName));

    m_webView->setPage(new QWebEnginePage(new QWebEngineProfile(this), m_webView));

    m_progressBar->setTextVisible(false);

    setupLayout();
    setupConnections();

    m_webView->load(authorizationUrl);
}

AuthorizationDialog::~AuthorizationDialog()
{
}

void AuthorizationDialog::setupLayout()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(m_webView);
    layout->addWidget(m_progressBar);
    setLayout(layout);
}

void AuthorizationDialog::setupConnections()
{
    connect(m_webView, &QWebEngineView::loadStarted, m_progressBar, &QProgressBar::show);
    connect(m_webView, &QWebEngineView::loadStarted, m_progressBar, &QProgressBar::reset);
    connect(m_webView, &QWebEngineView::loadProgress, m_progressBar, &QProgressBar::setValue);
    connect(m_webView, &QWebEngineView::loadFinished, m_progressBar, &QProgressBar::hide);
    connect(m_webView, &QWebEngineView::urlChanged, this, &AuthorizationDialog::urlChanged);
}
