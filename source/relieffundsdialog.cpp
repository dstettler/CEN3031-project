#include "relieffundsdialog.h"
#include "ui_relieffundsdialog.h"

ReliefFundsDialog::ReliefFundsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReliefFundsDialog)
{
    ui->setupUi(this);
}

ReliefFundsDialog::~ReliefFundsDialog()
{
    delete ui;
}
