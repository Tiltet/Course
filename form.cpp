#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    // ПРОВЕРКА НА ВВОД ВЕЩЕСТВЕННОГО ЧИСЛА
    this->locale = QLocale::system();
    locale.setNumberOptions(QLocale::RejectGroupSeparator | QLocale::OmitGroupSeparator);
    this->validator = new QDoubleValidator(this);
    this->validator->setLocale(locale);
    // ПРОВЕРКА НА ВВОД ВЕЩЕСТВЕННОГО ЧИСЛА
}

Form::~Form()
{
    delete ui;
}


// ФУНКЦИЯ ПРОВЕРКИ ВВОДА ПОЛЬЗОВАТЕЛЯ
double Form::Validation()
{
    QString text = ui->lineEdit->text();
    qDebug() << "Изначальная строка: " << text;
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
        else if (number > 10000)
        {
            qDebug() << "Вот такое число не прошло: " << number;
            throw std::invalid_argument("Число слишком большое");
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



// ФУНКЦИЯ КНОПКИ ГОТОВО
void Form::on_pushButton_ready_clicked()
{
    try
    {
        if (this->change == "наличные")
        {
            double number = Validation();
            emit signalChangeCash(number);
        }
        else if (this->change == "карта")
        {
            double number = Validation();
            emit signalChangeCard(number);
        }
        else if (this->change == "категория дохода")
        {
            emit signalAddCategoryIncome(ui->lineEdit->text());
        }
        else if (this->change == "категория расхода")
        {
            emit signalAddCategoryExpenses(ui->lineEdit->text());
        }

        ui->lineEdit->clear();
        ui->lineEdit->setValidator(0);
        this->close();
    }
    catch(std::invalid_argument(&e))
    {
        QMessageBox::critical(this, "Ошибка", e.what());
        ui->lineEdit->clear();
    }
}
// ФУНКЦИЯ КНОПКИ ГОТОВО



// СЛОТЫ ПЕРЕДАЧИ ВЫБОРА ПОЛЬЗОВАТЕЛЯ
void Form::slotCash()
{
    ui->label->setText("Бюджет наличных: ");
    this->change = "наличные";
    ui->lineEdit->setValidator(this->validator);
}

void Form::slotCard()
{
    ui->label->setText("Бюджет карты: ");
    this->change = "карта";
    ui->lineEdit->setValidator(this->validator);
}

void Form::slotCategoryIncome()
{
    ui->label->setText("Категория дохода:");
    this->change = "категория дохода";
}

void Form::slotCategoryExpenses()
{
    ui->label->setText("Категория расхода:");
    this->change = "категория расхода";
}
// СЛОТЫ ПЕРЕДАЧИ ВЫБОРА ПОЛЬЗОВАТЕЛЯ



// ФУНКЦИЯ КНОПКИ ОТМЕНА
void Form::on_pushButton_cancellation_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit->setValidator(0);
    this->close();
}
// ФУНКЦИЯ КНОПКИ ОТМЕНА
