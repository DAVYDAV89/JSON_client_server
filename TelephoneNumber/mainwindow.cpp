#include "ui_mainwindow.h"

#include "mainwindow.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_table(new MyModelTable(this))
{
    ui->setupUi(this);
    ui->tableView -> setModel(m_table);
    ui->tableView -> setColumnWidth(0, 100);
    ui->tableView -> setColumnWidth(1, 150);
    ui->tableView -> setColumnWidth(2, 200);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, [&](QPoint _pos) {
        QMenu * menu = new QMenu(this);

        QAction * addContact = new QAction(("Добавить"), this);
        QAction * deleteContact = new QAction(("Удалить"), this);

        connect(addContact, &QAction::triggered, this, &MainWindow::slotAddContact );
        connect(deleteContact, &QAction::triggered, this, &MainWindow::slotRemoveContact );

        menu->addAction(addContact);
        menu->addAction(deleteContact);

        menu->popup(ui->tableView->viewport()->mapToGlobal(_pos));
    });

    m_table -> insertRows(0, 1, QModelIndex());
    ui->tableView -> setRowHidden(0, true);
    ui->tableView -> setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->le_filtr, &QLineEdit::textChanged, this, &MainWindow::slot_on_filtrContact);
    connect(ui->btn_addContact, &QPushButton::clicked, this, &MainWindow::slotAddContact);
    connect(ui->btn_removeContact, &QPushButton::clicked, this, &MainWindow::slotRemoveContact);
    connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::slotSaveContactInJS);

    connect(ui->btn_exit, &QPushButton::clicked, [&]() {
        close();
    });

    m_Socket = new QTcpSocket(this);
    m_Socket->connectToHost("127.0.0.1", 9999);

    connect(m_Socket, &QTcpSocket::disconnected, m_Socket, &QTcpSocket::deleteLater);
    connect(m_Socket, &QTcpSocket::readyRead, this, &MainWindow::slot_ready );


    foreach(const QJsonValue &val, readJSfile()) {
        loadData(val);
    }

    ui->tableView->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAddContact()
{
    int countRow = m_table -> rowCount(QModelIndex());
    m_table -> insertRows(countRow, 1, QModelIndex());

    QModelIndex index;
    index = m_table -> index( countRow, 0, QModelIndex());
    m_table -> setData(index, "", Qt::EditRole);

    index = m_table -> index(countRow, 1, QModelIndex());
    m_table -> setData(index, "", Qt::EditRole);

    index = m_table -> index(countRow, 2, QModelIndex());
    m_table -> setData(index, "", Qt::EditRole);

    index = m_table -> index(countRow, 3, QModelIndex());
    m_table -> setData(index, "", Qt::EditRole);
}

void MainWindow::slotRemoveContact()
{
    int selectedRows = ui->tableView -> selectionModel()->selectedRows().count();

    if (selectedRows == 0)
        return;

    m_table -> removeRows(ui->tableView -> selectionModel()->selectedRows().at(0).row(), selectedRows);
}

QJsonArray MainWindow::readJSfile()
{
    QString strJS;
    QFile file("phoneBook.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        strJS = file.readAll();
        file.close();
    }

    QJsonDocument doc = QJsonDocument::fromJson(strJS.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray jarr = json["book"].toArray();

    return jarr;
}

void MainWindow::clearTable()
{
    m_table -> removeRows(0, m_table -> rowCount(QModelIndex()));
    m_table -> insertRow(0, QModelIndex());
    ui->tableView -> setRowHidden(0, true);
}

void MainWindow::loadData(const QJsonValue &val)
{   
    int countRow = m_table -> rowCount(QModelIndex());
    m_table -> insertRows(countRow, 1, QModelIndex());

    QModelIndex index;
    index = m_table -> index( countRow, 0, QModelIndex());
    m_table -> setData(index, val["family"].toString(), Qt::EditRole);

    index = m_table -> index(countRow, 1, QModelIndex());
    m_table -> setData(index, val["firstName"].toString(), Qt::EditRole);

    index = m_table -> index(countRow, 2, QModelIndex());
    m_table -> setData(index, val["secondName"].toString(), Qt::EditRole);

    index = m_table -> index(countRow, 3, QModelIndex());

    QString _lstNumber;
    foreach(const auto &number, val["phoneNumber"].toArray()) {
        _lstNumber.push_front(number.toString() + ",\n");
    }
    _lstNumber = _lstNumber.left(_lstNumber.lastIndexOf(QChar(',')));

    m_table -> setData(index, _lstNumber, Qt::EditRole);

    ui->tableView -> resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::slot_on_filtrContact(const QString & _filtr)
{
    clearTable();

    foreach(const QJsonValue &val, readJSfile()) {
        if (_filtr.isEmpty()) {
            loadData(val);
        }
        else {
            QString _lstNumber;
            foreach(const auto &number, val["phoneNumber"].toArray()) {
                _lstNumber += number.toString() + ",\n";
            }

            if ( (val["family"].toString().contains(_filtr))
                 || (val["firstName"].toString().contains(_filtr))
                 || (val["secondName"].toString().contains(_filtr))
                 || (_lstNumber.contains(_filtr))
                 )   {
                loadData(val);
            }
        }
    }
}

void MainWindow::slotSaveContactInJS()
{
    QModelIndex _index;
    QVariantMap _map;
    QJsonDocument doc;
    QString jsonString;

//    stream << quint64(0);
    jsonString += "{\n";
    jsonString += "\"book\":[";

    for (int row = 0; row < m_table -> rowCount(QModelIndex()); ++row ) {
        if (ui->tableView -> isRowHidden(row))
            continue;

        for (int col = 0; col < m_table -> columnCount(QModelIndex()); ++col ) {
            _index = m_table -> index(row, col, QModelIndex());
            switch (col) {
            case 0:
                _map.insert("family", _index.model()->data(_index.model()->index(_index.row(),_index.column())).toString() );
                break;
            case 1:
                _map.insert("firstName", _index.model()->data(_index.model()->index(_index.row(),_index.column())).toString() );
                break;
            case 2:
                _map.insert("secondName", _index.model()->data(_index.model()->index(_index.row(),_index.column())).toString() );
                break;
            case 3:
                _map.insert("phoneNumber", QJsonArray::fromStringList(QStringList(_index.model()->data(_index.model()->index(_index.row(),_index.column())).toString().split(",\n"))));
                break;
            }
        }
        QJsonDocument doc(QJsonObject::fromVariantMap(_map));
        jsonString += doc.toJson(QJsonDocument::Indented);

        if (row != m_table -> rowCount(QModelIndex()) - 1 )
            jsonString += ",";
    }
    jsonString += "]\n";
    jsonString += "}";

//    qDebug() << jsonString;


    qDebug() << m_Socket->write(jsonString.toLocal8Bit());
}

void MainWindow::slot_ready()
{

}
