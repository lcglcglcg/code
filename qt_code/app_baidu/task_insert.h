#ifndef __TASK_INSERT_H__
#define __TASK_INSERT_H__

#include <QtGui>
#include "keyword.h"
#include "network.h"

class task_insert_page1 : public QWizardPage
{

public:
    void page_init(QLineEdit *keyword_edit);
};


class task_insert_page2 : public QWizardPage
{

public:
    void page_init(QProgressBar *progress, QComboBox *url_list);
};


class task_insert_page3 : public QWizardPage
{

public:
    void page_init(QLineEdit *keyword, QLineEdit *url, QLineEdit time_edit[]);
    QLabel time_name[24];
};



class task_insert : public QWizard
{
    Q_OBJECT

public:
    task_insert(network_tcp *network);

private slots:
    void buttion_next();
    void buttion_finish();

    void page1_edit_changed(QString str);
    void page2_web_progress(int progress);
    void page2_url_combobox(QStringList list);

public:
    network_tcp *network;

    QLineEdit page1_keyword_edit;

    keyword_web page2_keyword_web;
    QProgressBar page2_progress;
    QComboBox page2_url_list;

    QLineEdit page3_keyword_edit;
    QLineEdit page3_url_edit;
    QLineEdit page3_time_edit[24];

};

#endif
