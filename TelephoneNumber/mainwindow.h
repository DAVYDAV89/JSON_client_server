#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "mymodeltable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MyModelTable *m_table;

    QTcpSocket *m_Socket;


    QJsonArray readJSfile();
    void clearTable();
    void loadData(const QJsonValue &val);

private slots:
    void slotAddContact();
    void slotRemoveContact();
    void slot_on_filtrContact(const QString &);
    void slotSaveContactInJS();

    void slot_ready();
};
#endif // MAINWINDOW_H
