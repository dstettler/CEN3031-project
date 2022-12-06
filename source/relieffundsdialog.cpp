#include "relieffundsdialog.h"
#include "ui_relieffundsdialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>

ReliefFundsDialog::ReliefFundsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReliefFundsDialog)
{
    ui->setupUi(this);

    //Images we want in the relief funds page
    QPixmap redCrossImage("/Users/johnnyhuynh/Downloads/RedCrossImage.png");
    QPixmap directReliefImage("/Users/johnnyhuynh/Downloads/DirectReliefImage.png");
    QPixmap disasterFundImage("/Users/johnnyhuynh/Downloads/DisasterFundImage.png");

    //Setting the QLabels to the image
    ui->RedCrossImage->setPixmap(redCrossImage);
    ui->DirectReliefImage->setPixmap(directReliefImage);
    ui->DisasterFundImage->setPixmap(disasterFundImage);
}

ReliefFundsDialog::~ReliefFundsDialog()
{
    delete ui;
}

