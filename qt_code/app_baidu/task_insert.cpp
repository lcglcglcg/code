#include "task_insert.h"


void task_insert_page1::page_init(QLineEdit *keyword_edit)
{

    setTitle(tr("请输入关键词"));
    registerField(tr("keyword_edit*"), keyword_edit);//触发next按钮

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(keyword_edit);
    setLayout(layout);
}

void task_insert_page2::page_init(QProgressBar *progress, QComboBox *url_list)
{

    setTitle(tr("请选择链接"));
    registerField(tr("url_list*"), url_list);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(progress);
    layout->addWidget(url_list);
    setLayout(layout);
}

void task_insert_page3::page_init(QLineEdit *keyword, QLineEdit *url, QLineEdit time_edit[])
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
    layout->addWidget(url);
    layout->addLayout(row_01);
    layout->addLayout(row_02);
    layout->addLayout(row_03);
    layout->addLayout(row_04);
    layout->addLayout(row_05);
    layout->addLayout(row_06);
    setLayout(layout);
}

task_insert::task_insert(network_tcp *network)
{

    this->network = network;

    QAbstractButton *bt_next = button(QWizard::NextButton);
    QAbstractButton *bt_finish = button(QWizard::FinishButton);
    connect(bt_next, SIGNAL(clicked()), this, SLOT(buttion_next()));
    connect(bt_finish, SIGNAL(clicked()), this, SLOT(buttion_finish()));


    task_insert_page1 *page1 = new task_insert_page1;
    page1->page_init(&page1_keyword_edit);
    connect(&page1_keyword_edit, SIGNAL(textChanged(QString)),
            this, SLOT(page1_edit_changed(QString)));
    addPage(page1);


    task_insert_page2 *page2 = new task_insert_page2;
    page2->page_init(&page2_progress, &page2_url_list);
    connect(&page2_keyword_web, SIGNAL(loadProgress(int)),
            this, SLOT(page2_web_progress(int)));
    connect(&page2_keyword_web, SIGNAL(keyword_signal(QStringList)),
            this,  SLOT(page2_url_combobox(QStringList)));
    addPage(page2);


    task_insert_page3 *page3 = new task_insert_page3;
    page3->page_init(&page3_keyword_edit, &page3_url_edit, page3_time_edit);
    page3_keyword_edit.setEnabled(false);
    page3_url_edit.setEnabled(false);
    addPage(page3);
}


void task_insert::page1_edit_changed(QString str)
{

    if (str.trimmed().isEmpty())
        page1_keyword_edit.clear();
}

void task_insert::page2_web_progress(int progress)
{

    page2_progress.setValue((page2_keyword_web.page_index * 100) + progress);
}

void task_insert::page2_url_combobox(QStringList list)
{

    page2_url_list.clear();
    page2_url_list.addItems(list);
}

void task_insert::buttion_next()
{

    if (nextId() > 0) {

        //setStartId(1);
        QString keyword = page1_keyword_edit.text();
        page2_progress.setRange(0, 100 * 3);
        page2_keyword_web.keyword_start(keyword, 3);

    } else if (nextId() == -1) {

        QString keyword = page1_keyword_edit.text();
        QString url = page2_url_list.currentText();
        page3_keyword_edit.setText(keyword);
        page3_url_edit.setText(url);
    }
}

void task_insert::buttion_finish()
{

    int time_click[24];
    for(int n = 0; n < 24; n++)
        time_click[n] = page3_time_edit[n].text().toInt();

    QString keyword = page1_keyword_edit.text();
    QString url = page2_url_list.currentText();
    network->packet_keyword_insert(keyword, url, time_click);
}
