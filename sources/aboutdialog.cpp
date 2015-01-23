#include "aboutdialog.h"
#include "ui_aboutdialog.h"

using namespace FreelanceNavigator;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->qtLabel->setText(tr("Using Qt ") + QT_VERSION_STR);

    QString applicationVersion = QApplication::applicationVersion();
    ui->freelanceNavigatorLabel->setText("Freelance Navigator " + applicationVersion);

    connect(ui->closeButton, &QPushButton::clicked, this, &AboutDialog::reject);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
