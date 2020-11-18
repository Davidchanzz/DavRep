#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>
#include <QShortcut>

namespace Ui {
class DialogSearch;
}

class DialogSearch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSearch(QWidget *parent = nullptr);
    ~DialogSearch();

signals:
    void sendData(QString, QString);
private slots:

    void on_pushButton_clicked();

private:
    Ui::DialogSearch *ui;
    QShortcut *enter;
};

#endif // DIALOGSEARCH_H
