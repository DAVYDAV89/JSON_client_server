#ifndef MYMODELTABLE_H
#define MYMODELTABLE_H

#include <QAbstractTableModel>
#include <QDateTime>

struct Task
{
    QString     m_family;
    QString     m_firstName;
    QString     m_secondName;
    QString     m_phoneNumber;
};

class MyModelTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModelTable(QObject *parent = nullptr);
    MyModelTable(const QVector<Task> &tasks, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

//    const QVector<Task> &getTasks() const;

private:
    QVector<Task> m_tasks;
};

#endif // MYMODELTABLE_H
