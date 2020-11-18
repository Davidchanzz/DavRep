#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowModality(Qt::ApplicationModal);

    enter = new QShortcut(this);
    enter->setKey(Qt::Key_Return);
    connect(enter, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
}

DialogSearch::~DialogSearch()
{
    delete ui;
}

void DialogSearch::on_pushButton_clicked()
{
    emit sendData(ui->lineEdit->text(), ui->comboBox->currentText());
    this->close();
}
