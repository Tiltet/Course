#include "user.h"

User::User()
{

}

double User::getMoney()
{
    return this->money;
}

void User::setMoney(double money)
{
    this->money = money;
    return;
}

double User::getCard()
{
    return this->card;
}

void User::setCard(double card)
{
    this->card = card;
    return;
}

double User::getCash()
{
    return this->cash;
}

void User::setCash(double cash)
{
    this->cash = cash;
    return;
}

double User::getIncome()
{
    return this->income;
}

void User::setIncome(double income)
{
    this->income = income;
    return;
}

double User::getMaxIncome()
{
    return this->maxIncome;
}

void User::setMaxIncome(double maxIncome)
{
    this->maxIncome = maxIncome;
    return;
}

double User::getMinIncome()
{
    return this->minIncome;
}

void User::setMinIncome(double minIncome)
{
    this->minIncome = minIncome;
    return;
}

double User::getExpenses()
{
    return this->expenses;
}

void User::setExpenses(double expenses)
{
    this->expenses = expenses;
    return;
}

double User::getMaxExpenses()
{
    return this->maxExpenses;
}

void User::setMaxExpenses(double maxExpenses)
{
    this->maxExpenses = maxExpenses;
}

double User::getMinExpenses()
{
    return this->minExpenses;
}

void User::setMinExpenses(double minExpenses)
{
    this->minExpenses = minExpenses;
}
