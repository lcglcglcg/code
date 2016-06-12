#include "task_table.h"


task_table::task_table()
{

    setColumnCount(2);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    //horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    QStringList header;
    header.append(tr("关键词"));
    header.append(tr("链接"));
    setHorizontalHeaderLabels(header);
}

int task_table::task_table_cmp(QString keyword)
{

    QList<QTableWidgetItem *> list;
    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QTableWidgetItem *item, list) {

        if (item->column() == 0) {

            if (keyword.compare(item->text()) == 0) {

                return true;
            }
        }
    }

    return false;
}

void task_table::task_table_add(QString keyword, QString url)
{

    if (task_table_cmp(keyword) == true)
        return;

    int row = rowCount();
    insertRow(row);

    setItem(row, 0, new QTableWidgetItem(keyword));
    setItem(row, 1, new QTableWidgetItem(url));
}

void task_table::task_table_del(QString keyword)
{

    QList<QTableWidgetItem *> list;
    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QTableWidgetItem *item, list) {

        if (item->column() == 0) {

            if (keyword.compare(item->text()) == 0) {

                removeRow(item->row());
                return;
            }
        }
    }
}

QStringList task_table::task_get_keyword()
{

    QStringList keyword_list;
    QList<QTableWidgetItem *> list;
    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QTableWidgetItem *item, list) {

        if (item->column() == 0) {

            keyword_list << item->text();
        }
    }

    return keyword_list;
}
