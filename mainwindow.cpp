#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    user = new User;

    // СОЗДАНИЕ БАЗЫ ДАННЫХ
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/timofey/Desktop/CourseWork/build-Finance-Qt_6_6_0_for_macOS-Debug/data.db");
    if(db.open())
    {
        qDebug("База данных открыта \n");
    }
    else
    {
        qDebug("База данных НЕ открыта \n");
    }
    // СОЗДАНИЕ БАЗЫ ДАННЫХ

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Data(Id INT, Count TEXT, Type TEXT, Price DOUBLE, Category TEXT, Comment TEXT, Date DATE);");
    query->exec("CREATE TABLE Money(Id INT, Cash DOUBLE, Card double, Income DOUBLE, Expenses DOUBLE);");
    query->exec("CREATE TABLE Categorie_income(Id INT, Income_categories TEXT, Income_categories_money DOUBLE);");
    query->exec("CREATE TABLE Categorie_expenses(Id INT, Expenses_categories TEXT, Expenses_categories_money DOUBLE);");

    model_data = new QSqlTableModel(this, db);
    model_data->setTable("Data");
    model_data->select();


    // БАЗА ДАННЫХ, КОТОРАЯ ХРАНИТ ИНФОРМАЦИЮ О БЮДЖЕТЕ
    model_money = new QSqlTableModel(this, db);
    model_money->setTable("Money");
    model_money->select();

    query->exec("SELECT COUNT(*) FROM Money");
    if (query->next())
    {
        int count = query->value(0).toInt();
        if (count == 0)
        {
            qDebug() << "Создается таблица Money \n";
            model_money->insertRow(0);
            model_money->setData(model_money->index(0, 0), 1);
            model_money->setData(model_money->index(0, 1), 0);
            model_money->setData(model_money->index(0, 2), 0);
            model_money->setData(model_money->index(0, 3), 0);
            model_money->setData(model_money->index(0, 4), 0);
            if (!model_money->submitAll())
            {
                qDebug() << "Ошибка при добавлении строки в таблицу Money \n";
            }
        }
        else
        {
            qDebug() << "Таблица Money содержит " << count << " строк" << "\n";
        }
    }
    else
    {
        qDebug() << "Ошибка при выполнении запроса: \n";
    }

    query->exec("SELECT Cash, Card, Income, Expenses FROM Money");

    if (query->next())
    {
        this->user->setCash(round((query->value(0).toDouble())*100)/100);
        this->user->setCard(round((query->value(1).toDouble())*100)/100);
        this->user->setIncome(round((query->value(2).toDouble())*100)/100);
        this->user->setExpenses(round((query->value(3).toDouble())*100)/100);
        this->user->setMoney(query->value(0).toDouble() + query->value(1).toDouble());

        qDebug() << this->user->getCard() << this->user->getCash() << this->user->getIncome() << this->user->getExpenses() << this->user->getMoney();

        ui->label_money_number->setText(QString::number(user->getMoney()) + "$");
        ui->label_card_number->setText(QString::number(user->getCard()) + "$");
        ui->label_cash_number->setText(QString::number(user->getCash()) + "$");
        ui->label_income_number->setText(QString::number(user->getIncome()) + "$");
        ui->label_expenses_number->setText(QString::number(user->getExpenses()) + "$");
    }
    else
    {
        qDebug() << "Ошибка при выполнении запроса: SELECT Cash, Card FROM Money \n";
    }
    // БАЗА ДАННЫХ, КОТОРАЯ ХРАНИТ ИНФОРМАЦИЮ О БЮДЖЕТЕ


    model_categories_income = new QSqlTableModel(this, db);
    model_categories_income->setTable("Categorie_income");
    model_categories_income->select();

    query->prepare("SELECT Income_categories, Income_categories_money FROM Categorie_income");

    if (query->exec())
    {
        while(query->next())
        {
            this->user->categoriesIncome.push_back(query->value(0).toString());
            this->user->categoriesIncomeMoney.push_back(round((query->value(1).toDouble())*100)/100);

            ui->listWidget_income_name->addItem(query->value(0).toString());
            ui->listWidget_income_money->addItem(query->value(1).toString() + "$");
        }
    }
    else
    {
        qDebug() << "Ошибка во время выгрузки данных из Categorie_incom\n";
    }


    model_categories_expenses = new QSqlTableModel(this, db);
    model_categories_expenses->setTable("Categorie_expenses");
    model_categories_expenses->select();

    query->prepare("SELECT Expenses_categories, Expenses_categories_money FROM Categorie_expenses");

    if (query->exec())
    {
        while(query->next())
        {
            this->user->categoriesExpenses.push_back(query->value(0).toString());
            this->user->categoriesExpensesMoney.push_back(round((query->value(1).toDouble())*100)/100);

            ui->listWidget_expenses_name->addItem(query->value(0).toString());
            ui->listWidget_expenses_money->addItem(query->value(1).toString() + "$");
        }
    }
    else
    {
        qDebug() << "Ошибка во время выгрузки данных из Categorie_expenses\n";
    }

    for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
    {
        qDebug() << "Категория = " << this->user->categoriesExpenses[i] << "Расход = " << this->user->categoriesExpensesMoney[i];
    }


    ui->tableView->setModel(model_data);                                                                            // ставит бд в виджет
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setStretchLastSection(false);
    ui->tableView->horizontalHeader()->resizeSection(0, 40);
    ui->tableView->horizontalHeader()->resizeSection(1, 60);
    ui->tableView->horizontalHeader()->resizeSection(2, 100);

    form = new Form;
    connect(this, &MainWindow::signalCash, form, &Form::slotCash);                                                  // коннект для НАЛИЧНЫХ
    connect(this, &MainWindow::signalCard, form, &Form::slotCard);                                                  // коннект для КАРТЫ
    connect(this, &MainWindow::signalCategoryIncome, form, &Form::slotCategoryIncome);                              // коннект для КАТЕГОРИИ ДОХОДА
    connect(this, &MainWindow::signalCategoryExpenses, form, &Form::slotCategoryExpenses);                          // коннект для КАТЕГОРИИ РАСХОДА
    connect(form, &Form::signalChangeCash, this, &MainWindow::slotChangeCash);                                      // коннект ддя изменения НАЛИЧНЫХ
    connect(form, &Form::signalChangeCard, this, &MainWindow::slotChangeCard);                                      // коннект ддя изменения КАРТЫ
    connect(form, &Form::signalAddCategoryIncome, this, &MainWindow::slotAddCategoryIncome);                        // коннект для добавления КАТЕГОРИИ ДОХОДА
    connect(form, &Form::signalAddCategoryExpenses, this, &MainWindow::slotAddCategoryExpenses);                    // коннект для добавления КАТЕГОРИИ РАСХОДА

    trans = new transaction(this->user);
    connect(trans, &transaction::signalTransaction, this, &MainWindow::slotTransaction);
    connect(this, &MainWindow::signalUpdate, trans, &transaction::slotUpdate);

    ui->label_max_expenses->hide();
    ui->label_max_expenses_number->hide();

    ui->label_max_income->hide();
    ui->label_max_income_number->hide();
}

MainWindow::~MainWindow()
{
    query->prepare("UPDATE Money SET Cash = :cash, Card = :card, Income = :income, Expenses = :expenses WHERE id = 1");
    query->bindValue(":cash", this->user->getCash());
    query->bindValue(":card", this->user->getCard());
    query->bindValue(":income", this->user->getIncome());
    query->bindValue(":expenses", this->user->getExpenses());

    // qDebug() << this->user->getCash() << this->user->getCard() << this->user->getIncome() << this->user->getExpenses();

    if (!query->exec())
    {
        qDebug() << "Ошибка во время обновления данных таблицы Money\n";
    }
    else
    {
        qDebug() << "Обновление данных Money прошла успешно \n";
    }

    model_money->select();

    query->exec("DELETE FROM Categorie_income");

    for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
    {
        query->prepare("INSERT INTO Categorie_income (Id, Income_categories, Income_categories_money) VALUES (:id, :category, :money)");
        query->bindValue(":category", this->user->categoriesIncome[i]);
        query->bindValue(":money", this->user->categoriesIncomeMoney[i]);
        query->bindValue(":id", i+1);

        if (!query->exec())
        {
            qDebug() << "Ошибка во время обновления таблицы Categorie_income";
        }
    }

    model_categories_income->select();

    query->exec("DELETE FROM Categorie_expenses");

    for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
    {
        query->prepare("INSERT INTO Categorie_expenses (Id, Expenses_categories, Expenses_categories_money) VALUES (:id, :category, :money)");
        query->bindValue(":category", this->user->categoriesExpenses[i]);
        query->bindValue(":money", this->user->categoriesExpensesMoney[i]);
        query->bindValue(":id", i+1);

        if (!query->exec())
        {
            qDebug() << "Ошибка во время обновления таблицы Categorie_expenses";
        }
    }

    model_categories_expenses->select();

    delete ui;
}

// КНОПКА ДОБАВЛЕНИЯ ТРАНЗАКЦИИ
void MainWindow::on_pushButton_add_trans_clicked()
{
    emit signalUpdate();
    trans->show();
}
// КНОПКА ДОБАВЛЕНИЯ ТРАНЗАКЦИИ



// СЛОТ ДОБАВЛЕНИЯ ТРАНЗАКЦИИ
void MainWindow::slotTransaction(QString type, QString check, QString category, double price, QString comment, QDate date)
{
    model_data->insertRow(model_data->rowCount());

    int id = 0;
    query->exec("SELECT id FROM Data ORDER BY id DESC LIMIT 1");
    if (query->next())
    {
        id = query->value(0).toInt();
        qDebug() << "ID = " << id;
    }
    else
    {
        qDebug() << "Ошибка во время получения последнего ID базы транзакций \n";
    }

    id = id + 1;

    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Type")), type);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Count")), check);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Category")), category);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Price")), price);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Comment")), comment);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("Date")), date);
    model_data->setData(model_data->index(model_data->rowCount()-1, model_data->fieldIndex("id")), id);
    model_data->submitAll();

    if (check == "+")
    {
        if (type == "наличные")
        {
            this->user->setCash(this->user->getCash() + price);
            ui->label_cash_number->setText(QString::number(this->user->getCash()) + "$");

            this->user->setIncome(this->user->getIncome() + price);
            ui->label_income_number->setText(QString::number(this->user->getIncome()) + "$");

            qDebug() << "Пополнение НАЛИЧНЫХ = " << this->user->getCash()
                     << "Пополнение ДОХОДОВ = " << this->user->getIncome()
                     << "\n";
        }
        else
        {
            this->user->setCard(this->user->getCard() + price);
            ui->label_card_number->setText(QString::number(this->user->getCard()) + "$");

            this->user->setIncome(this->user->getIncome() + price);
            ui->label_income_number->setText(QString::number(this->user->getIncome()) + "$");

            qDebug() << "Пополнение КАРТЫ" << this->user->getCard()
                     << "Пополнение ДОХОДОВ = " << this->user->getIncome()
                     << "\n";
        }

        for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
        {
            if (this->user->categoriesIncome[i] == category)
            {
                this->user->categoriesIncomeMoney[i] = this->user->categoriesIncomeMoney[i] + price;

                for(int j = 0; j < ui->listWidget_income_name->count(); ++j)
                {
                    QListWidgetItem* item = ui->listWidget_income_name->item(j);
                    if (item->text() == category)
                    {
                        QListWidgetItem* item2 = ui->listWidget_income_money->item(j);
                        if (item2)
                        {
                            item2->setText(QString::number(this->user->categoriesIncomeMoney[i]) + "$");
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (type == "наличные")
        {
            this->user->setCash(this->user->getCash() - price);
            ui->label_cash_number->setText(QString::number(this->user->getCash()) + "$");

            this->user->setExpenses(this->user->getExpenses() + price);
            ui->label_expenses_number->setText(QString::number(this->user->getExpenses()) + "$");

            qDebug() << "Трата НАЛИЧНЫХ" << this->user->getCash()
                     << "Пополнение РАСХОДОВ" << this->user->getExpenses()
                     << "\n";
        }
        else
        {
            this->user->setCard(this->user->getCard() - price);
            ui->label_card_number->setText(QString::number(this->user->getCard()) + "$");

            this->user->setExpenses(this->user->getExpenses() + price);
            ui->label_expenses_number->setText(QString::number(this->user->getExpenses()) + "$");

            qDebug() << "Трата КАРТЫ" << this->user->getCard()
                     << "Пополнение РАСХОДОВ" << this->user->getExpenses()
                     << "\n";
        }

        for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
        {
            if (this->user->categoriesExpenses[i] == category)
            {
                this->user->categoriesExpensesMoney[i] = this->user->categoriesExpensesMoney[i] + price;

                for(int j = 0; j < ui->listWidget_expenses_name->count(); ++j)
                {
                    QListWidgetItem* item = ui->listWidget_expenses_name->item(j);
                    if (item->text() == category)
                    {
                        QListWidgetItem* item2 = ui->listWidget_expenses_money->item(j);
                        if (item2)
                        {
                            item2->setText(QString::number(this->user->categoriesExpensesMoney[i]) + "$");
                        }
                    }
                }
            }
        }
    }

    this->user->setMoney(this->user->getCard() + this->user->getCash());
    ui->label_money_number->setText(QString::number(this->user->getMoney()) + "$");
}
// СЛОТ ДОБАВЛЕНИЯ ТРАНЗАКЦИИ



// КНОПКА УДАЛЕНИЯ ТРАНЗАКЦИИ
void MainWindow::on_pushButton_delete_trans_clicked()
{
    query->exec("SELECT * FROM Data ORDER BY Id DESC LIMIT 1");

    if (query->next())
    {
        int id = ui->tableView->model()->index(row, 0).data().toInt();
        QString check = ui->tableView->model()->index(row, 1).data().toString();
        QString type = ui->tableView->model()->index(row, 2).data().toString();
        double price = ui->tableView->model()->index(row, 3).data().toDouble();
        QString category = ui->tableView->model()->index(row, 4).data().toString();
        QString comment = ui->tableView->model()->index(row, 5).data().toString();
        QDate date = ui->tableView->model()->index(row, 6).data().toDate();

        if (check == "-")
        {
            if (type == "наличные")
            {
                this->user->setCash(this->user->getCash() + price);
                ui->label_cash_number->setText(QString::number(this->user->getCash()) + "$");

                this->user->setExpenses(this->user->getExpenses() - price);
                ui->label_expenses_number->setText(QString::number(this->user->getExpenses()));

                qDebug() << "Пополнение НАЛИЧНИХ = " << this->user->getCash()
                         << "Уменьшение РАСХОДОВ = " << this->user->getExpenses()
                         << "\n";
            }
            else
            {
                this->user->setCard(this->user->getCard() + price);
                ui->label_card_number->setText(QString::number(this->user->getCard()) + "$");

                this->user->setExpenses(this->user->getExpenses() - price);
                ui->label_expenses_number->setText(QString::number(this->user->getExpenses()));

                qDebug() << "Пополнение КАРТЫ" << this->user->getCard()
                         << "Изменение РАСХОДОВ = " << this->user->getExpenses()
                         << "\n";
            }

            this->user->setMoney(this->user->getMoney() + price);
            ui->label_money_number->setText(QString::number(this->user->getMoney()) + "$");

            for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
            {
                if (this->user->categoriesExpenses[i] == category)
                {
                    this->user->categoriesExpensesMoney[i] = this->user->categoriesExpensesMoney[i] - price;

                    for(int j = 0; j < ui->listWidget_expenses_name->count(); ++j)
                    {
                        QListWidgetItem* item = ui->listWidget_expenses_name->item(j);
                        if (item->text() == category)
                        {
                            QListWidgetItem* item2 = ui->listWidget_expenses_money->item(j);
                            if (item2)
                            {
                                item2->setText(QString::number(this->user->categoriesExpensesMoney[i]) + "$");
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (type == "наличные")
            {
                // this->user->setCash(this->user->getCash() - price);
                this->user->setCash(round((this->user->getCash() - price)*100)/100);

                ui->label_cash_number->setText(QString::number(this->user->getCash()) + "$");

                // this->user->setIncome(this->user->getIncome() - price);
                this->user->setIncome(round((this->user->getIncome() - price)*100)/100);

                ui->label_income_number->setText(QString::number(this->user->getIncome()) + "$");

                qDebug() << "Трата НАЛИЧНЫХ" << this->user->getCash()
                         << "Изменение ДОХОДОВ" << this->user->getIncome()
                         << "\n";

            }
            else
            {
                // this->user->setCard(this->user->getCard() - price);
                this->user->setCard(round((this->user->getCard() - price)*100)/100);
                ui->label_card_number->setText(QString::number(this->user->getCard()) + "$");

                //this->user->setIncome(this->user->getIncome() - price);
                this->user->setIncome(round((this->user->getIncome() - price)*100)/100);

                ui->label_income_number->setText(QString::number(this->user->getIncome()) + "$");

                qDebug() << "Трата КАРТЫ" << this->user->getCard()
                         << "Изменение ДОХОДОВ" << this->user->getIncome()
                         << "\n";
            }

            // this->user->setMoney(this->user->getMoney() - price);
            this->user->setMoney(round((this->user->getMoney() - price)*100)/100);

            ui->label_money_number->setText(QString::number(this->user->getMoney()) + "$");
        }

        qDebug() << "Type = " << type
                 << "Check = " << check
                 << "Category" << category
                 << "Price = " << price
                 << "Comment = " << comment
                 << "Date = " << date
                 << "Id = " << id
                 << "\n";

        for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
        {
            if (this->user->categoriesIncome[i] == category)
            {
                this->user->categoriesIncomeMoney[i] = this->user->categoriesIncomeMoney[i] - price;

                for(int j = 0; j < ui->listWidget_income_name->count(); ++j)
                {
                    QListWidgetItem* item = ui->listWidget_income_name->item(j);
                    if (item->text() == category)
                    {
                        QListWidgetItem* item2 = ui->listWidget_income_money->item(j);
                        if (item2)
                        {
                            item2->setText(QString::number(this->user->categoriesIncomeMoney[i]) + "$");
                        }
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "Последняя строка не найдена \n";
    }

    model_data->removeRow(row);
}
// КНОПКА УДАЛЕНИЯ ТРАНЗАКЦИИ



// ФУНКЦИЯ ПЕРЕДАЧИ ВЫБРАННОЙ СТРОКИ В БД
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}
// ФУНКЦИЯ ПЕРЕДАЧИ ВЫБРАННОЙ СТРОКИ В БД



// КНОПКА ИЗМЕНЕНИЯ НАЛИЧНЫХ
void MainWindow::on_pushButton_change_cash_clicked()
{
    emit signalCash();
    form->show();
}
// КНОПКА ИЗМЕНЕНИЯ НАЛИЧНЫХ



// КНОПКА ИЗМЕНЕНИЯ КАРТЫ
void MainWindow::on_pushButton_change_card_clicked()
{
    emit signalCard();
    form->show();
}
// КНОПКА ИЗМЕНЕНИЯ КАРТЫ



// СЛОТ ИЗМЕНЕНИЯ НАЛИЧНЫХ
void MainWindow::slotChangeCash(double number)
{
    QString str = QString::number(number);
    this->user->setCash(number);
    ui->label_cash_number->setText(str + "$");

    this->user->setMoney(this->user->getCard() + this->user->getCash());
    ui->label_money_number->setText(QString::number(this->user->getMoney()) + "$");
}
// СЛОТ ИЗМЕНЕНИЯ НАЛИЧНЫХ



// CЛОТ ИЗМЕНЕНИЯ КАРТЫ
void MainWindow::slotChangeCard(double number)
{
    QString str = QString::number(number);
    this->user->setCard(number);
    ui->label_card_number->setText(str + "$");

    this->user->setMoney(this->user->getCard() + this->user->getCash());
    ui->label_money_number->setText(QString::number(this->user->getMoney()) + "$");
}
// CЛОТ ИЗМЕНЕНИЯ КАРТЫ



// СЛОТ ДОБАВЛЕНИЯ КАТЕГОРИИ ДОХОДА
void MainWindow::slotAddCategoryIncome(QString category)
{
    ui->listWidget_income_name->addItem(category);
    ui->listWidget_income_money->addItem("0$");

    this->user->categoriesIncome.push_back(category);
    this->user->categoriesIncomeMoney.push_back(0);

    int id = 0;
    query->exec("SELECT id FROM Categorie_income ORDER BY id DESC LIMIT 1");
    if (query->next())
    {
        id = query->value(0).toInt();
        qDebug() << "ID = " << id;
    }
    else
    {
        qDebug() << "Ошибка во время получения последнего ID базы транзакций \n";
    }

    this->model_categories_income->insertRow(id);
    model_categories_income->setData(model_categories_income->index(id,0), id + 1);
    model_categories_income->setData(model_categories_income->index(id,1), category);
    model_categories_income->setData(model_categories_income->index(id,2), 0);
    if (!model_categories_income->submitAll())
    {
        qDebug() << "Ошибка во время добавления новой категории дохода\n";
    }
}
// СЛОТ ДОБАВЛЕНИЯ КАТЕГОРИИ ДОХОДА



// СЛОТ ДОБАВЛЕНИЯ КАТЕГОРИИ РАСХОДА
void MainWindow::slotAddCategoryExpenses(QString category)
{
    ui->listWidget_expenses_name->addItem(category);
    ui->listWidget_expenses_money->addItem("0$");

    this->user->categoriesExpenses.push_back(category);
    this->user->categoriesExpensesMoney.push_back(0);
}
// СЛОТ ДОБАВЛЕНИЯ КАТЕГОРИИ РАСХОДА



// КНОПКА ДОБАВЛЕНИЯ КАТЕГОРИИ ДОХОДА
void MainWindow::on_pushButton_add_income_category_clicked()
{
    this->form->show();
    emit signalCategoryIncome();
}
// КНОПКА ДОБАВЛЕНИЯ КАТЕГОРИИ ДОХОДА



// КНОПКА ДОБАВЛЕНИЯ КАТЕГОРИИ РАСХОДА
void MainWindow::on_pushButton_add_expenses_category_clicked()
{
    this->form->show();
    emit signalCategoryExpenses();
}
// КНОПКА ДОБАВЛЕНИЯ КАТЕГОРИИ РАСХОДА



// КНОПКА УДАЛЕНИЯ КАТЕГОРИИ ДОХОДА
void MainWindow::on_pushButton_delete_income_category_clicked()
{
    QListWidgetItem *item_name = ui->listWidget_income_name->currentItem();
    QListWidgetItem *item_money = ui->listWidget_income_money->currentItem();

    if (item_name && item_money && (ui->listWidget_income_name->row(item_name) == ui->listWidget_income_money->row(item_money)))
    {
        this->row = ui->listWidget_income_name->row(item_name);

        ui->listWidget_income_name->takeItem(this->row);
        ui->listWidget_income_money->takeItem(this->row);
    }

    QString category_text = item_name->text();

    for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
    {
        if (this->user->categoriesIncome[i] == category_text)
        {
            // qDebug() << "Номер = " << i << "Категория = " << this->user->categoriesIncome[i];
            this->user->categoriesIncome.removeAt(i);
            this->user->categoriesIncomeMoney.removeAt(i);
        }
    }

    for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
    {
        qDebug() << "Категория дохода = " << this->user->categoriesIncome[i] << "Доход = " << this->user->categoriesIncomeMoney[i];
    }
}
// КНОПКА УДАЛЕНИЯ КАТЕГОРИИ ДОХОДА



// КНОПКА УДАЛЕНИЯ КАТЕГОРИИ РАСХОДА
void MainWindow::on_pushButton_delete_expenses_category_clicked()
{
    QListWidgetItem *item_name = ui->listWidget_expenses_name->currentItem();
    QListWidgetItem *item_money = ui->listWidget_expenses_money->currentItem();

    if (item_name && item_money && (ui->listWidget_expenses_name->row(item_name) == ui->listWidget_expenses_money->row(item_money)))
    {
        this->row = ui->listWidget_expenses_name->row(item_name);

        ui->listWidget_expenses_name->takeItem(this->row);
        ui->listWidget_expenses_money->takeItem(this->row);
    }

    QString category_text = item_name->text();

    for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
    {
        if (this->user->categoriesExpenses[i] == category_text)
        {
            this->user->categoriesExpenses.removeAt(i);
            this->user->categoriesExpensesMoney.removeAt(i);
        }
    }
}
// КНОПКА УДАЛЕНИЯ КАТЕГОРИИ РАСХОДА



// КНОПКА ВЫХОДА
void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}
// КНОПКА ВЫХОДА














