#ifndef DIALOGRENAMETABLE_H
#define DIALOGRENAMETABLE_H

#include <QDialog>
#include <QShortcut>

namespace Ui {
class DialogReNameTable;
}

class DialogReNameTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReNameTable(QWidget *parent = nullptr);
    ~DialogReNameTable();

signals:
    void sendData(QString str);
private slots:

    void on_pushButton_clicked();

private:
    Ui::DialogReNameTable *ui;
    QShortcut *enter;
};

#endif // DIALOGRENAMETABLE_H
