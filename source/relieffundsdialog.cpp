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

void ReliefFundsDialog::on_RedCrossButton_clicked()
{
    QString link = "https://www.redcross.org/about-us/our-work/disaster-relief.html";
    QDesktopServices::openUrl(QUrl(link));
}

void ReliefFundsDialog::on_DirectReliefButton_clicked()
{
    QString link = "https://donate.directrelief.org/give/406660/#!/donation/checkout";
    QDesktopServices::openUrl(QUrl(link));
}

void ReliefFundsDialog::on_DisasterFundButton_clicked()
{
    QString link = "https://www.volunteerflorida.org/donatefdf/#:~:text=The%20Florida%20Disaster%20Fund%20is,times%20of%20emergency%20or%20disaster.";
    QDesktopServices::openUrl(QUrl(link));
}
