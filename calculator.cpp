#include "calculator.h"
#include "ui_calculator.h"

double calcVal = 0.0;
double ans = 0.0;
QString mem = "";
bool divTrigger = false;
bool multTrigger = false;
bool subTrigger = false;
bool addTrigger = false;
bool percentTrigger = false;
bool sqrtTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    QPushButton* numberButtons[10];
    for(int i = 0; i < 10; i++)
    {
        QString buttonName = "Button" + QString::number(i); //creates a variable representing the name of the button (according to the format i used in ui PushButton naming)
        numberButtons[i] = Calculator::findChild<QPushButton*>(buttonName); //stores the state of a number button
        connect(numberButtons[i], SIGNAL(pressed()), this, SLOT(NumPressed())); //whenever there is a signal that the button has been pressed, it triggers the function NumPressed

    }
    connect(ui->Add, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Sqrt, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Percent, SIGNAL(pressed()), this, SLOT(MathButtonPressed()));
    connect(ui->Enter, SIGNAL(pressed()), this, SLOT(Enter()));
    connect(ui->Clear, SIGNAL(pressed()), this, SLOT(Functional()));
    connect(ui->Delete, SIGNAL(pressed()), this, SLOT(Functional()));
    connect(ui->Answer, SIGNAL(pressed()), this, SLOT(Functional()));
    connect(ui->Decimal, SIGNAL(pressed()), this, SLOT(Functional()));
    connect(ui->ChangeSign, SIGNAL(pressed()), this, SLOT(ChangeSign()));

}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed()
{
    QPushButton* button = (QPushButton*)sender(); //sender returns a pointer to the object that sent the signal which in this case is numberButtons[i]
    QString buttonValue = button->text(); //numberButtons[i] represents the ui pushbutton and therefore we can get the text on the button which is its value
    QString displayValue = ui->Display->text();
    if(displayValue.toDouble() == 0 || displayValue.toDouble() == 0.0)
    {
        ui->Display->setText(buttonValue);
    }
    else
    {
        ui->Display->setText(displayValue + buttonValue);
    }

    mem = mem + buttonValue;
    ui->Memory->setText(mem);
}

void Calculator::MathButtonPressed()
{
    divTrigger = false;
    multTrigger = false;
    subTrigger = false;
    addTrigger = false;
    percentTrigger = false;
    sqrtTrigger = false;

    calcVal = ui->Display->text().toDouble();

    QPushButton* button = (QPushButton*)sender();
    QString operation = button->objectName();

    if(operation.compare((QString)"Add", Qt::CaseInsensitive) == 0)
    {
        addTrigger = true;
        mem = mem + " " + "+ ";
    }
    else if(operation.compare((QString)"Subtract", Qt::CaseInsensitive) == 0)
    {
        subTrigger = true;
        mem = mem + " " + "- ";
    }
    else if(operation.compare((QString)"Multiply", Qt::CaseInsensitive) == 0)
    {
        multTrigger = true;
        mem = mem + " " + "x ";
    }
    else if(operation.compare((QString)"Divide", Qt::CaseInsensitive) == 0)
    {
        divTrigger = true;
        mem = mem + " " + "÷ ";
    }
    else if(operation.compare((QString)"Sqrt", Qt::CaseInsensitive) == 0)
    {
        sqrtTrigger = true;
        mem = mem.sliced(0, mem.length() - QString::number(calcVal).length() - 1) + "√" + QString::number(calcVal);
    }
    else if(operation.compare((QString)"Percent", Qt::CaseInsensitive) == 0)
    {
        percentTrigger = true;
        mem = mem + " " + "% ";
    }

    ui->Display->setText("");

    ui->Memory->setText(mem);


}

void Calculator::Enter()
{
    double displayValue = ui->Display->text().toDouble();
    double solution = displayValue;

    if(addTrigger)
        solution = displayValue + calcVal;
    else if(subTrigger)
        solution = calcVal - displayValue;
    else if(multTrigger)
        solution = calcVal * displayValue;
    else if(divTrigger)
        solution = calcVal / displayValue;
    else if(sqrtTrigger)
        solution = sqrt(displayValue);
    else if(percentTrigger)
        solution = displayValue / 100;

    ui->Display->setText(QString::number(solution));
    ans = solution;
    mem = mem + " = " + QString::number(ans);
    ui->Memory->setText(mem);
}

void Calculator::ChangeSign()
{
    QString displayVal = ui->Display->text();
    static QRegularExpression reg("[-]?[0-9.]*");
    if (reg.match(displayVal).hasMatch())
    {
        ui->Display->setText(QString::number(-1 * displayVal.toDouble()));
    }
    mem = mem.sliced(0, mem.length() - displayVal.length() - 1) + "-" + displayVal;
    ui->Memory->setText(mem);

}

void Calculator::Functional()
{
    QPushButton* button = (QPushButton*)sender();
    QString funct = button->objectName();
    if (funct.compare((QString)"Clear", Qt::CaseInsensitive) == 0)
    {
        ui->Display->setText("");
    }
    else if (funct.compare((QString)"Delete", Qt::CaseInsensitive) == 0)
    {
        calcVal = 0.0;
        ans = 0.0;
        mem = "";
        ui->Memory->setText(mem);
        ui->Display->setText("");
    }
    else if (funct.compare((QString)"Answer", Qt::CaseInsensitive) == 0)
    {
        ui->Display->setText(QString::number(ans));
    }
    else if (funct.compare((QString)"Decimal", Qt::CaseInsensitive) == 0)
    {
        ui->Display->setText(ui->Display->text() + ".");
        mem = mem + ".";
        ui->Memory->setText(mem);
    }
}
