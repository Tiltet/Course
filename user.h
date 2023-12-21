#include <iostream>
#include <QVector>
#include <QString>

#ifndef USER_H
#define USER_H

using namespace std;

class User
{
public:
    User();

    double getMoney();
    void setMoney(double money);

    double getCard();
    void setCard(double card);

    double getCash();
    void setCash(double cash);

    double getIncome();
    void setIncome(double income);

    double getMaxIncome();
    void setMaxIncome(double maxIncome);

    double getMinIncome();
    void setMinIncome(double minIncome);

    double getExpenses();
    void setExpenses(double expenses);

    double getMaxExpenses();
    void setMaxExpenses(double maxExpenses);

    double getMinExpenses();
    void setMinExpenses(double minExpenses);

    QVector<QString> categoriesIncome;
    QVector<double> categoriesIncomeMoney;
    QVector<QString> categoriesExpenses;
    QVector<double> categoriesExpensesMoney;

private:
    double money;
    double cash;
    double card;
    double income;
    double maxIncome;
    double minIncome;
    double expenses;
    double maxExpenses;
    double minExpenses;
};

#endif // USER_H
