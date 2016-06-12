#include "task_delete.h"


void task_delete_page1::page_init(QComboBox *keyword_list)
{

    setTitle(tr("选择需要删除的关键词"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(keyword_list);
    setLayout(layout);
}

void task_delete_page2::page_init(QLineEdit *keyword)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(keyword);
    setLayout(layout);
}

task_delete::task_delete(network_tcp *network, QStringList keyword_list)
{

    this->network = network;
    page1_keyword_list.addItems(keyword_list);

    QAbstractButton *bt_next = button(QWizard::NextButton);
    QAbstractButton *bt_finish = button(QWizard::FinishButton);
    connect(bt_next, SIGNAL(clicked()), this, SLOT(buttion_next()));
    connect(bt_finish, SIGNAL(clicked()), this, SLOT(buttion_finish()));


    task_delete_page1 *page1 = new task_delete_page1;
    page1->page_init(&page1_keyword_list);
    addPage(page1);

    task_delete_page2 *page2 = new task_delete_page2;
    page2_keyword_edit.setEnabled(false);
    page2->page_init(&page2_keyword_edit);
    addPage(page2);
}

void task_delete::buttion_next()
{

    page2_keyword_edit.setText(page1_keyword_list.currentText());
}

void task_delete::buttion_finish()
{

    QString keyword = page2_keyword_edit.text();
    network->packet_keyword_delete(keyword);
}
