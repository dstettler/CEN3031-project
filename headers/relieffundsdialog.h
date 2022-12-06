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

private:
    Ui::ReliefFundsDialog *ui;
};

#endif // RELIEFFUNDSDIALOG_H
