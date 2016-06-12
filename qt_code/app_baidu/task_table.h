#ifndef TASKTABLE_H
#define TASKTABLE_H

#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>

#include <QLayout>
#include <QPushButton>
#include <QWidget>

class task_table : public QTableWidget
{

    Q_OBJECT

public:
    task_table();

public slots:
    int task_table_cmp(QString keyword);
    void task_table_add(QString keyword, QString url);
    void task_table_del(QString keyword);
    QStringList task_get_keyword(void);

};

#endif // TASKTABLE_H

