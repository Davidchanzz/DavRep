#include "dialogrenametable.h"
#include "ui_dialogrenametable.h"

DialogReNameTable::DialogReNameTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReNameTable)
{
    ui->setupUi(this);
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowModality(Qt::ApplicationModal);

    enter = new QShortcut(this);
    enter->setKey(Qt::Key_Return);
    connect(enter, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
}

DialogReNameTable::~DialogReNameTable()
{
    delete ui;
}

void DialogReNameTable::on_pushButton_clicked()
{
    emit sendData(ui->lineEdit->text());
    this->close();
}
