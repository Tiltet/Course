#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QDoubleValidator>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    double Validation();
    ~Form();

private slots:
    void on_pushButton_ready_clicked();

    void on_pushButton_cancellation_clicked();

private:
    Ui::Form *ui;
    QString change;
    QValidator *validator;
    QLocale locale;

signals:
    void signalChangeCash(double number);                   // сигнал для изменения НАЛИЧНЫХ
    void signalChangeCard(double number);                   // сигнал для изменения КАРТЫ
    void signalAddCategoryIncome(QString category);         // сигнал для добавления КАТЕГОРИИ ДОХОДА
    void signalAddCategoryExpenses(QString category);       // сигнал для добавления КАТЕГОРИИ РАСХОДА

public slots:
    void slotCash();                                        // слот для изменения НАЛИЧНЫХ
    void slotCard();                                        // слот для изменения КАРТЫ
    void slotCategoryIncome();                              // слот для изменения КАТЕГОРИИ ДОХОДА
    void slotCategoryExpenses();                            // слот для изменения КАТЕГОРИИ РАСХОДА

};

#endif // FORM_H
