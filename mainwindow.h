#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>
#include <QVector>
#include <iostream>

#include <form.h>
#include <transaction.h>
#include <user.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_change_cash_clicked();
    void on_pushButton_change_card_clicked();
    void on_pushButton_add_trans_clicked();
    void on_pushButton_delete_trans_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_add_income_category_clicked();
    void on_pushButton_add_expenses_category_clicked();
    void on_pushButton_delete_income_category_clicked();
    void on_pushButton_delete_expenses_category_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model_data;                                         // модель для транзакций
    QSqlTableModel *model_money;                                        // модель для бюджета
    QSqlTableModel *model_categories_income;                            // модель для категорий ДОХОДА
    QSqlTableModel *model_categories_expenses;                          // модель для категорий РАСХОДА
    Form *form;                                                         // класс формы для изменения бюджета
    transaction *trans;                                                 // класс формы для добавления транзакции
    int row;                                                            // номер выделенной строки
    User *user;

public slots:
    void slotChangeCash(double number);
    void slotChangeCard(double number);
    void slotAddCategoryIncome(QString category);
    void slotAddCategoryExpenses(QString category);
    void slotTransaction(QString type, QString check, QString category, double price, QString comment, QDate date);

signals:
    void signalCash();                                                  // сигнал изменнения НАЛИЧНЫХ
    void signalCard();                                                  // сигнал изменнения КАРТЫ
    void signalCategoryIncome();                                        // сигнал добавления КАТЕГОРИИ ДОХОДОВ
    void signalCategoryExpenses();                                      // сигнал добавления КАТЕГОРИИ РАСХОДА
    void signalUpdate();
};
#endif // MAINWINDOW_H
