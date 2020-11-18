#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QShortcut>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

signals:
    void sendData(QString str);
private slots:

    void on_pushButton_clicked();

private:
    Ui::Form *ui;
    QShortcut *enter;
};

#endif // FORM_H
