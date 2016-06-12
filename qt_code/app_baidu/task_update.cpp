#include "task_update.h"


void task_update_page1::page_init(QComboBox *keyword_list)
{

    setTitle(tr("选择需要修改的关键词"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(keyword_list);
    setLayout(layout);
}

void task_update_page2::page_init(QLineEdit *keyword, QLineEdit time_edit[])
{
    setTitle(tr("请修改时间段的点击次数/次"));

    QHBoxLayout *row_01 = new QHBoxLayout;
    QHBoxLayout *row_02 = new QHBoxLayout;
    QHBoxLayout *row_03 = new QHBoxLayout;
    QHBoxLayout *row_04 = new QHBoxLayout;
    QHBoxLayout *row_05 = new QHBoxLayout;
    QHBoxLayout *row_06 = new QHBoxLayout;

    for (int n = 0; n < 24; n++) {

        QString hour_str;
        QString hour_start;
        QString hour_end;

        if(n < 10) {

            hour_start = tr("0") + QString::number(n);
            if(n == 9)
                hour_end = QString::number(n + 1);
            else
                hour_end = tr("0") + QString::number(n + 1);

        } else {

            hour_start = QString::number(n);
            if (n == 23)
                hour_end = tr("00");
            else
                hour_end = QString::number(n + 1);
        }

        hour_start += tr("点");
        hour_end += tr("点");
        hour_str = hour_start + tr("-") + hour_end;
        time_name[n].setText(hour_str);
    }

    for (int n = 0; n < 4; n++) {

        row_01->addWidget(&time_name[n]);
        row_01->addWidget(&time_edit[n]);
    }

    for (int n = 4; n < 8; n++) {

        row_02->addWidget(&time_name[n]);
        row_02->addWidget(&time_edit[n]);
    }

    for (int n = 8; n < 12; n++) {

        row_03->addWidget(&time_name[n]);
        row_03->addWidget(&time_edit[n]);
    }

    for (int n = 12; n < 16; n++) {

        row_04->addWidget(&time_name[n]);
        row_04->addWidget(&time_edit[n]);
    }

    for (int n = 16; n < 20; n++) {

        row_05->addWidget(&time_name[n]);
        row_05->addWidget(&time_edit[n]);
    }

    for (int n = 20; n < 24; n++) {

        row_06->addWidget(&time_name[n]);
        row_06->addWidget(&time_edit[n]);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(keyword);
    layout->addLayout(row_01);
    layout->addLayout(row_02);
    layout->addLayout(row_03);
    layout->addLayout(row_04);
    layout->addLayout(row_05);
    layout->addLayout(row_06);
    setLayout(layout);
}


task_update::task_update(network_tcp *network, QStringList keyword_list)
{

    this->network = network;
    page1_keyword_list.addItems(keyword_list);

    QAbstractButton *bt_next = button(QWizard::NextButton);
    QAbstractButton *bt_finish = button(QWizard::FinishButton);
    connect(bt_next, SIGNAL(clicked()), this, SLOT(buttion_next()));
    connect(bt_finish, SIGNAL(clicked()), this, SLOT(buttion_finish()));

    task_update_page1 *page1 = new task_update_page1;
    page1->page_init(&page1_keyword_list);
    addPage(page1);

    task_update_page2 *page2 = new task_update_page2;
    page2_keyword.setEnabled(false);
    page2->page_init(&page2_keyword, page2_time_edit);
    addPage(page2);
}


void task_update::buttion_next()
{

    page2_keyword.setText(page1_keyword_list.currentText());
}

void task_update::buttion_finish()
{
    int time_click[24];
    for (int n = 0; n < 24; n++)
        time_click[n] = page2_time_edit[n].text().toInt();

    QString keyword = page2_keyword.text();

    network->packet_keyword_update(keyword, time_click);
}
