#include "bdview.h"

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const {

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() > 1 && index.column() < 7)
        flags |= Qt::ItemIsEditable;
    return flags;
}

void BDView::ini(){
    resizeRowsToContents();
    resizeColumnsToContents();
}

QVariant MyModel::data(const QModelIndex &index, int role) const {

    QVariant value = QSqlQueryModel::data(index, role);

    switch (role) {

    case Qt::DisplayRole:
    case Qt::EditRole:
        if (index.column() == 0)
            return value.toString().prepend(tr("№"));
        else if (index.column() == 3)
            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
        else if (index.column() == 4 && role == Qt::DisplayRole)
            return tr("%1").arg(value.toDouble(), 0, 'f', 2);
        else if (index.column() == 7 && role == Qt::DisplayRole)
            return value.toTime().toString("hh:mm:ss");
        else return value;

    case Qt::TextColorRole:
        if(index.column() == 4)
            return qVariantFromValue(QColor(Qt::red));
        else
            return value;

    case Qt::TextAlignmentRole:
        if(index.column() == 4)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        else if(index.column() == 7 || index.column() == 8)
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        else
            return int(Qt::AlignLeft | Qt::AlignVCenter);

    case Qt::FontRole:
        if(index.column() == 2) {
            QFont font = QFont("Helvetica", 10, QFont::Bold);
            return qVariantFromValue(font);
        }else
            return value;

    case Qt::BackgroundColorRole: {
        if(index.model()->data(index.model()->index(index.row(), 5)) == "Сompleted")
            return qVariantFromValue(QColor(Qt::green));
        if(index.model()->data(index.model()->index(index.row(), 5)) == "Canceled")
            return qVariantFromValue(QColor(Qt::red));
        return value;
    }
    }
    return value;
}

bool MyModel::setData(
            const QModelIndex &index,
            const QVariant &value,
            int) {
    if (index.column() < 2 || index.column() > 6)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(
                index.row(), 0);
    int id = QSqlQueryModel::data(primaryKeyIndex).toInt();



    bool ok;
    QSqlQuery query;
    QString str = tableName();
    if (index.column() == 2) {
        query.prepare("UPDATE " + str + " SET Product = ? WHERE id = ?");
        query.addBindValue(value.toString());
        query.addBindValue(id);
    }else if(index.column() == 3) {
        query.prepare("UPDATE " + str + " SET Amount = ? WHERE id = ?");
        query.addBindValue(value.toInt());
        query.addBindValue(id);
    }else if(index.column() == 4) {
        query.prepare("UPDATE " + str + " SET Price = ? WHERE id = ?");
        query.addBindValue(value.toDouble());
        query.addBindValue(id);
    }else if(index.column() == 5) {
        query.prepare("UPDATE " + str + " SET Status = ? WHERE id = ?");
        query.addBindValue(value.toString());
        query.addBindValue(id);
    }else if(index.column() == 6) {
        query.prepare("UPDATE " + str + " SET Buyer = ? WHERE id = ?");
        query.addBindValue(value.toString());
        query.addBindValue(id);
    }
    ok = query.exec();
    select();
    emit resizecell();
    return ok;
}

MyDSBDelegate::MyDSBDelegate(QString tm1, QString tm2, QString tm3, QString tm4, QObject *parent): QItemDelegate(parent), tm1(tm1), tm2(tm2), tm3(tm3), tm4(tm4){

}

QWidget *MyDSBDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const {
    QComboBox *editor = new QComboBox(parent);
    editor->addItem(tm1);
    editor->addItem(tm2);
    editor->addItem(tm3);
    editor->addItem(tm4);
    editor->installEventFilter(const_cast<MyDSBDelegate*>(this));
    return editor;
}

void MyDSBDelegate::updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex&) const {
    editor->setGeometry(option.rect);
}

QWidget *MySpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const {
    QSpinBox *editor = new QSpinBox(parent);
    editor->installEventFilter(const_cast<MySpinDelegate*>(this));
    return editor;
}

void MySpinDelegate::updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex&) const {
    editor->setGeometry(option.rect);
}


BDView::BDView(QWidget *parent)
      : QTableView(parent) {

    MyDSBDelegate *dsbd = new MyDSBDelegate("Accepted", "During", "Сompleted", "Canceled", this);
    MySpinDelegate *dbsp = new MySpinDelegate();
    setItemDelegateForColumn(5, dsbd);
    setItemDelegateForColumn(3, dbsp);
}
