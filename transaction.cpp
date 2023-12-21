#include "transaction.h"
#include "ui_transaction.h"

transaction::transaction(User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transaction),
    user(user)
{
    ui->setupUi(this);
    this->user = user;

    ui->radioButton_income->setChecked(true);
    ui->radioButton_cash->setChecked(true);

    // ГРУППИРОВКА radioButton
    QButtonGroup *group_money = new QButtonGroup(this);
    group_money->addButton(ui->radioButton_expenses);
    group_money->addButton(ui->radioButton_income);

    QButtonGroup *group_check = new QButtonGroup(this);
    group_check->addButton(ui->radioButton_card);
    group_check->addButton(ui->radioButton_cash);
    // ГРУППИРОВКА radioButton

    ui->dateEdit_date->setDate(QDate::currentDate()); // ставим актуальную дату в поле с датой

    // ПРОВЕРКА НА ВВОД ВЕЩЕСТВЕННОГО ЧИСЛА
    QLocale locale = QLocale::system();
    locale.setNumberOptions(QLocale::RejectGroupSeparator | QLocale::OmitGroupSeparator);
    QDoubleValidator* validator = new QDoubleValidator(this);
    validator->setLocale(locale);
    ui->lineEdit_money->setValidator(validator);
    // ПРОВЕРКА НА ВВОД ВЕЩЕСТВЕННОГО ЧИСЛА
}

transaction::~transaction()
{
    delete ui;
}



// СЛОТ ОБНОВЛЕНИЯ
void transaction::slotUpdate()
{
    ui->radioButton_income->setChecked(true);
    ui->radioButton_cash->setChecked(true);

    ui->comboBox_category->clear();
    qDebug() << "Обновление";
    for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
    {
        ui->comboBox_category->addItem(this->user->categoriesIncome[i]);
    }
    ui->comboBox_category->setCurrentIndex(0);
}
// СЛОТ ОБНОВЛЕНИЯ


// ФУНКЦИЯ ПРОВЕРКИ ВВОДА ПОЛЬЗОВАТЕЛЯ
double transaction::Validation()
{
    QString text = ui->lineEdit_money->text();
    text.replace(',','.');
    double number = text.toDouble();
    qDebug() << "Число после преобразования: " << number;

    std::stringstream ss;
    ss << number;
    std::string str = ss.str();
    size_t pos = str.find('.');
    if (pos != string::npos)
    {
        if ((str.length() - pos - 1 == 2) || (str.length() - pos - 1 == 1))
        {
            return number;
        }
        else
        {
            qDebug() << "Вот такое число не прошло: " << number;
            throw std::invalid_argument("Допустимо только 2 цифры после запятой");
        }
    }
    else
    {
        return number;
    }
}
// ФУНКЦИЯ ПРОВЕРКИ ВВОДА ПОЛЬЗОВАТЕЛЯ

// ИЗМЕНЕНИЕ ЛЕЙБЛА ДОХОД/РАСХОД
void transaction::on_radioButton_expenses_clicked()
{
    ui->label_money->setText("Введите расход:");
    ui->comboBox_category->clear();

    for (int i = 0; i < this->user->categoriesExpenses.size(); ++i)
    {
        ui->comboBox_category->addItem(this->user->categoriesExpenses[i]);
    }
    ui->comboBox_category->setCurrentIndex(0);
}
void transaction::on_radioButton_income_clicked()
{
    ui->label_money->setText("Введите доход:");
    ui->comboBox_category->clear();

    for (int i = 0; i < this->user->categoriesIncome.size(); ++i)
    {
        ui->comboBox_category->addItem(this->user->categoriesIncome[i]);
    }
    ui->comboBox_category->setCurrentIndex(0);
}
// ИЗМЕНЕНИЕ ЛЕЙБЛА ДОХОД/РАСХОД



// ФУНКЦИЯ КНОПКИ ОТМЕНА
void transaction::on_pushButton_exit_clicked()
{
    ui->lineEdit_comment->clear();
    ui->lineEdit_money->clear();
    this->close();
}
// ФУНКЦИЯ КНОПКИ ОТМЕНА



// ФУНКЦИЯ КНОПКИ ГОТОВО
void transaction::on_pushButton_ready_clicked()
{
    // bool type = false, check = false;
    QString type = "карта", check = "+";
    try
    {
        if (ui->radioButton_cash->isChecked())
        {
            type = "наличные";
        }
        else
        {
            type = "карта";
        }

        if (ui->radioButton_income->isChecked())
        {
            check = "+";
        }
        else
        {
            check = "-";
        }

        emit signalTransaction(type, check, ui->comboBox_category->currentText(), Validation(), ui->lineEdit_comment->text(), ui->dateEdit_date->date());

        ui->lineEdit_comment->clear();
        ui->lineEdit_money->clear();


    }
    catch(std::invalid_argument(&e))
    {
        QMessageBox::critical(this, "Ошибка",e.what());
        ui->lineEdit_money->clear();
    }

    this->close();
}
// ФУНКЦИЯ КНОПКИ ГОТОВО
