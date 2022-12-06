#include "relieffundsdialog.h"
#include "ui_relieffundsdialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QDir>
#include <QMessageBox>

ReliefFundsDialog::ReliefFundsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReliefFundsDialog)
{
    ui->setupUi(this);

    //For Mac
    #ifdef Q_OS_MACX
        QDir test(QDir::currentPath());

        //Go up from .app
        test.cdUp();
        test.cdUp();
        test.cdUp();

        QString tempString;

        QFileInfo fi(test, tempString);
        QString _fileName = fi.canonicalFilePath();

    //For Windows
    #else
        QString _fileName = QDir::currentPath();
    #endif

    QMessageBox _m;
    _m.setText(_fileName);
    _m.exec();
    //Images we want in the relief funds page
    QPixmap redCrossImage(_fileName + "/images/RedCrossImage.png");
    QPixmap directReliefImage(_fileName + "/images/DirectReliefImage.png");
    QPixmap disasterFundImage(_fileName + "/images/DisasterFundImage.png");

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
