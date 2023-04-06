#include "mymodeltable.h"

MyModelTable::MyModelTable(QObject *parent)
    : QAbstractTableModel(parent)
{

}

MyModelTable::MyModelTable(const QVector<Task> &tasks, QObject *parent)
    : QAbstractTableModel(parent)
    , m_tasks(tasks)
{

}

int MyModelTable::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_tasks.size();
}

int MyModelTable::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 4;
}

QVariant MyModelTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if ((!index.row()) >= m_tasks.size() || index.row() < 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const auto &task = m_tasks.at(index.row());

        switch (index.column()) {
        case 0:
            return task.m_family;
            break;
        case 1:
            return task.m_firstName;
            break;
        case 2:
            return task.m_secondName;
            break;
        case 3:
            return task.m_phoneNumber;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant MyModelTable::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Фамилия");
            break;
        case 1:
            return QString("Имя");
            break;
        case 2:
            return QString("Отчество");
            break;
        case 3:
            return QString("Номер телефона");
            break;
        default:
            break;
        }
    }

    return QVariant();
}

bool MyModelTable::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        m_tasks.insert(position, {QString(), QString(),QString(), QString()});
    }

    endInsertRows();
    return true;
}

bool MyModelTable::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        m_tasks.removeAt(position);
    }

    endRemoveRows();
    return true;
}

bool MyModelTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        const int row = index.row();
        auto task = m_tasks.value(row);
        if (role == Qt::EditRole) {

            switch (index.column()) {
            case 0:
                task.m_family = value.toString();
//                if (!task.m_family .isEmpty())
                break;
            case 1:
                task.m_firstName = value.toString();
//                if (!task.m_firstName .isEmpty())
                break;
            case 2:
                task.m_secondName = value.toString();
//                if (!task.m_secondName .isEmpty())
                break;
            case 3:
                task.m_phoneNumber = value.toString();
//                if (!task.m_phoneNumber .isEmpty())
                break;
            default:
                return false;
            }
        }

//        if (role == Qt::EditRole) {
//            switch (index.column()) {
//            default:
//                return false;
//            }
//        }

        m_tasks.replace(row, task);

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags MyModelTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        flags |= Qt::ItemIsEditable;
    return flags;
}

