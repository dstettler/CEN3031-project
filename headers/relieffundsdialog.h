#ifndef RELIEFFUNDSDIALOG_H
#define RELIEFFUNDSDIALOG_H

#include <QDialog>

namespace Ui {
class ReliefFundsDialog;
}

class ReliefFundsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReliefFundsDialog(QWidget *parent = nullptr);

    ~ReliefFundsDialog();

private Q_SLOTS:
    void on_RedCrossButton_clicked();
    void on_DirectReliefButton_clicked();
    void on_DisasterFundButton_clicked();

private:
    Ui::ReliefFundsDialog *ui;

};

#endif // RELIEFFUNDSDIALOG_H
