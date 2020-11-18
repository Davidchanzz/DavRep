#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bdview.h"

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QSqlTableModel>
#include <QTabWidget>
#include <QTableView>
#include <QVector>
#include <QShortcut>
#include <QTime>
#include <QDate>
#include <QWidget>
#include <QMessageBox>
#include "form.h"
#include "dialogsearch.h"
#include "dialogrenametable.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void recieveTableName(QString);

    void slotAddRecord();

    void slotDelRecord();

    void slotDelTable();

    void searchReasult(QString, QString);

    void ShwoSearchResult();

    void DataBaseConnect();

    void DataViewerCreate();

    void on_actionAboute_triggered();

    void on_actionAuthor_triggered();

    void on_actionHelp_triggered();

    void slotReNameTable(QString);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QVector<BDView*> Views;
    QVector<MyModel*> MyModels;
    QVector<QString> Matches;
    QShortcut *addTable;
    QShortcut *addRecord;
    QShortcut *delRecord;
    QShortcut *delTable;
    QShortcut *search;
    QShortcut *nextSearchItem;
    QShortcut *reNameTable;
    Form *nForm;
    DialogSearch *dSearch;
    DialogReNameTable *formReNameTable;
};

#endif // MAINWINDOW_H
