#ifndef __TASK_UPDATE_H__
#define __TASK_UPDATE_H__

#include <QtGui>
#include "network.h"

class task_update_page1 : public QWizardPage
{

public:
    void page_init(QComboBox *keyword_list);
};


class task_update_page2 : public QWizardPage
{

public:
    void page_init(QLineEdit *keyword, QLineEdit time_edit[]);
    QLabel time_name[24];
};

class task_update : public QWizard
{

    Q_OBJECT

public:
    task_update(network_tcp *network, QStringList keyword_list);

public slots:
    void buttion_next();
    void buttion_finish();

public:
    network_tcp *network;

    QComboBox page1_keyword_list;

    QLineEdit page2_keyword;
    QLineEdit page2_time_edit[24];
};

#endif
