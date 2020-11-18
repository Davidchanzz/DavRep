#include "mainwindow.h"
#include "form.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nForm = new Form();
    dSearch = new DialogSearch();
    formReNameTable = new DialogReNameTable();

    DataBaseConnect();
    DataViewerCreate();

    addTable = new QShortcut(this);
    addRecord = new QShortcut(this);
    delRecord = new QShortcut(this);
    delTable = new QShortcut(this);
    search = new QShortcut(this);
    nextSearchItem = new QShortcut(this);
    reNameTable = new QShortcut(this);

    addTable->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
    addRecord->setKey(Qt::CTRL + Qt::Key_N);
    delRecord->setKey(Qt::CTRL + Qt::Key_Delete);
    delTable->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_Delete);
    search->setKey((Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    nextSearchItem->setKey(Qt::CTRL + Qt::Key_F);
    reNameTable->setKey(Qt::CTRL + Qt::SHIFT + Qt::Key_T);

    connect(nForm, SIGNAL(sendData(QString)), this, SLOT(recieveTableName(QString)));
    connect(dSearch, SIGNAL(sendData(QString, QString)), this, SLOT(searchReasult(QString, QString)));
    connect(addTable, SIGNAL(activated()), nForm, SLOT(show()));
    connect(addRecord, SIGNAL(activated()), this, SLOT(slotAddRecord()));
    connect(delRecord, SIGNAL(activated()), this, SLOT(slotDelRecord()));
    connect(delTable, SIGNAL(activated()), this, SLOT(slotDelTable()));
    connect(search, SIGNAL(activated()), dSearch, SLOT(show()));
    connect(nextSearchItem, SIGNAL(activated()), this, SLOT(ShwoSearchResult()));
    connect(reNameTable, SIGNAL(activated()), formReNameTable, SLOT(show()));
    connect(formReNameTable, SIGNAL(sendData(QString)), this, SLOT(slotReNameTable(QString)));
}

void MainWindow::slotReNameTable(QString newNameTable){

    if (newNameTable == "") return;
    QSqlQuery query;
    QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    query.exec("ALTER TABLE " + nameTable + " RENAME TO " + newNameTable + ";");

    DataViewerCreate();
}

void MainWindow::DataBaseConnect(){

    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("BD.sqlite");
    db.setUserName("");
    db.setPassword("");
    db.open();
}

void MainWindow::DataViewerCreate(){

    ui->tabWidget->clear();
    Views.clear();
    MyModels.clear();

    QStringList ListTable = db.tables();

    for(auto it = ListTable.begin(); it != ListTable.end(); it++){
        if (*it == "sqlite_sequence") continue;

        BDView *view = new BDView();
        MyModel *Model = new MyModel();

        connect(Model, SIGNAL(resizecell()), view, SLOT(ini()));

        Model->setTable(*it);
        Model->setEditStrategy(QSqlTableModel::OnFieldChange);
        Model->select();

        MyModels.push_back(Model);

        view->setModel(Model);
        view->setColumnHidden(0, true);
        view->setAlternatingRowColors(true);
        view->setSortingEnabled(true);
        view->resizeRowsToContents();
        view->resizeColumnsToContents();
        view->sortByColumn(0, Qt::AscendingOrder);

        Views.push_back(view);

        ui->tabWidget->addTab(view, *it);
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::recieveTableName(QString nameTable){
    if (nameTable != "") {
        QSqlQuery query;

        query.exec( "CREATE TABLE " + nameTable + "("
                    "id INTEGER PRIMARY KEY, "
                    "Code INTEGER NOT NULL, "
                    "Product TEXT, "
                    "Amount INTEGER, "
                    "Price REAL NOT NULL, "
                    "Status TEXT NOT NULL, "
                    "Buyer TEXT, "
                    "Time TEXT NOT NULL UNIQUE, "
                    "Date TEXT NOT NULL"
                    ");");

        DataViewerCreate();
    };
}

void MainWindow::searchReasult(QString searchQuery, QString ColName){
  if (searchQuery == "") return;

  Matches.clear();

  QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

  QSqlQuery query;

  query.exec("SELECT * FROM " + nameTable + " WHERE " + ColName + " like '%" + searchQuery + "%';");

  while(query.next()){
     Matches.push_back(query.value(7).toString());
  }

  ShwoSearchResult();
}

void MainWindow::ShwoSearchResult(){

    static int currRecordIndex;
    QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    int currTable = ui->tabWidget->currentIndex();

    for(int i = 0; i <= MyModels[currTable]->rowCount(); i++){
        QModelIndex index = MyModels[currTable]->index(i, 7);
        if(index.data().toString() == Matches[currRecordIndex]){
            Views[currTable]->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            Views[currTable]->scrollTo(index, QAbstractItemView::EnsureVisible);
        }
    }
      currRecordIndex == Matches.size() - 1? currRecordIndex = 0 : currRecordIndex++;
}

void MainWindow::slotDelTable()
{
    QSqlQuery query;
    QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    query.exec("DROP TABLE " + nameTable + "");

    DataViewerCreate();
}

void MainWindow::slotAddRecord()
{
    QSqlQuery query;
    int currTable = ui->tabWidget->currentIndex();
    QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    query.prepare("INSERT INTO " + nameTable + "(Code, Product, Amount, Price, Status, Buyer, Time, Date) "
                  "VALUES (:Code, '', 0, 0, 'During', '', :Time, :Date)");

    QTime currTime = QTime::currentTime();
    QDate currDate = QDate::currentDate();
    int code = (qrand() + 1000 + currTime.msec()) % 1000;
    query.bindValue(":Code", code);
    query.bindValue(":Time", currTime.toString());
    query.bindValue(":Date", currDate.toString("dd.MM.yyyy"));
    query.exec();

    MyModels[currTable]->select();
}

void MainWindow::slotDelRecord()
{
    QSqlQuery query;
    int currTable = ui->tabWidget->currentIndex();
    int Row = Views[currTable]->currentIndex().row();

    QString nameTable = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QModelIndex indexTime = MyModels[currTable]->index(Row, 7);

    query.exec("DELETE FROM " + nameTable + " WHERE time = '" + indexTime.data(Qt::DisplayRole).toString() + "'");

    MyModels[currTable]->select();
}

void MainWindow::on_actionAboute_triggered()
{
    QMessageBox::information(this, "Order book","This program displays orders stored in the database and allows you to work with them.");
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(this, "Developer","Nodiya Giorgi M80-213.");
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::warning(this, "Help me!","The program is controlled by hotkeys.\n"
                                          "CTRL + SHIFT + N : Create new table\n"
                                          "CTRL + SHIFT + DELETE : Delete current table\n"
                                          "CTRL + N : Add new record\n"
                                          "CTRL + DELETE : Delete current record\n"
                                          "CTRL + SHIFT + F : Search\n"
                                          "CTRL + F : Next Search Matche"
                                          "CTRL + SHIFT + T : Rename current table\n");
}
