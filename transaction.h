#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QWidget>
#include <QDate>
#include <QButtonGroup>

#include "form.h"
#include "user.h"

namespace Ui {
class transaction;
}

class User;

class transaction : public QWidget
{
    Q_OBJECT

public:
    explicit transaction(User *user ,QWidget *parent = nullptr);

    double Validation();

    ~transaction();

private slots:
    void on_radioButton_expenses_clicked();
    void on_radioButton_income_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_ready_clicked();

public slots:
    void slotUpdate();

private:
    Ui::transaction *ui;
    User *user;

signals:
    void signalTransaction(QString type, QString check, QString category, double price, QString comment, QDate date);
};

#endif // TRANSACTION_H
