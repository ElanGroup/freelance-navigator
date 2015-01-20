#include "aboutdialog.h"
#include "ui_aboutdialog.h"

using namespace FreelanceNavigator;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
