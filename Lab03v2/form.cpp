#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowModality(Qt::ApplicationModal);

    enter = new QShortcut(this);
    enter->setKey(Qt::Key_Return);
    connect(enter, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    emit sendData(ui->lineEdit->text());
    this->close();
}
