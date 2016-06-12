#ifndef __TASK_DELETE_H__
#define __TASK_DELETE_H__

#include <QtGui>
#include "network.h"

class task_delete_page1 : public QWizardPage
{

public:
    void page_init(QComboBox *keyword_list);
};

class task_delete_page2 : public QWizardPage
{

public:
    void page_init(QLineEdit *keyword);
};


class task_delete : public QWizard
{
    Q_OBJECT

public:
    task_delete(network_tcp *network, QStringList keyword_list);

private slots:
    void buttion_next();
    void buttion_finish();

public:
    network_tcp *network;

    QComboBox page1_keyword_list;
    QLineEdit page2_keyword_edit;
};

#endif
