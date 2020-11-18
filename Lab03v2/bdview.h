#ifndef BDVIEW_H
#define BDVIEW_H

#include <QSqlTableModel>
#include <QTableView>
#include <QItemDelegate>
#include <QComboBox>
#include <QDate>
#include <QTime>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSpinBox>

class MyModel : public QSqlTableModel {
    Q_OBJECT
public:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,
                 const QVariant &value, int role);
signals:
    void resizecell();
};

class BDView : public QTableView {
    Q_OBJECT
public:
    BDView(QWidget *parent = nullptr);
private slots:

    void ini();
//    virtual void resizeEvent(QResizeEvent *event);
};

class MyDSBDelegate : public QItemDelegate {
    Q_OBJECT
public:
    MyDSBDelegate(QString tm1 = "test1",
                  QString tm2 = "test2",
                  QString tm3 = "test3",
                  QString tm4 = "test4",
                  QObject *parent = nullptr);
    QWidget *createEditor(
                QWidget *parent,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
private:
    QString tm1;
    QString tm2;
    QString tm3;
    QString tm4;
};

class MySpinDelegate : public QItemDelegate {
    Q_OBJECT
public:
    QWidget *createEditor(
                QWidget *parent,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
    void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // BDVIEW_H
