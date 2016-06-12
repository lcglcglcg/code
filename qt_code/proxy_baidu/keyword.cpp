#include "keyword.h"

KeyWord::KeyWord(QTextEdit *edit)
{

    this->edit = edit;
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(keyword_progress(int)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(keyword_complete(bool)));
}

void KeyWord::keyword_start(QString word, int page_max)
{

    page_index = 1;
    this->page_max = page_max;
    list_word.clear();
    list_url.clear();
    QString url = BAIDU_FIND_STRING + word;
    load(QUrl(url));
}

void KeyWord::keyword_progress(int progress)
{

    QString buffer = "Analysis KeyWord Page:";
    buffer += QString::number(page_index);
    buffer += " progress:";
    buffer += QString::number(progress);
    edit->append(buffer);
}

void KeyWord::keyword_complete(bool complete)
{

    if (!complete) return;

    QWebFrame  *frame = this->page()->currentFrame();

    QWebElement document = frame->documentElement();

    QList <QWebElement> element_list = document.findAll("a").toList();

    foreach (QWebElement element, element_list) {

        if (element.isNull()) return;

        element.setFocus();

        QString str = element.toPlainText();

        if (str == "1") break;
        if (str == "2") break;
        if (str == "3") break;
        if (str == "4") break;
        if (str == "5") break;
        if (str == "6") break;
        if (str == "7") break;
        if (str == "8") break;
        if (str == "9") break;
        if (str == "10") break;
        if (str == "11") break;
        if (str == "<上一页") break;
        if (str != ""
                && str != "搜索设置"
                && str != "百度首页"
                && str != "登录"
                && str != "注册"
                && str != "新闻"
                && str != "贴吧"
                && str != "知道"
                && str != "MP3"
                && str != "图片"
                && str != "视频"
                && str != "地图"
                && str != "百科"
                && str != "文库"
                && str != "词典"
                && str != "更多>>"
                && str != "手写"
                && str != "拼音"
                && str != "关闭"
                && str != "把百度设为主页"
                && str != "把百度设为主页"
                && str != "推广链接"
                && str != "▶来百度推广您的产品"
                && str != "百度快照"
                && str != "把百度添加到桌面"
                && str != "举报图片"
                && str != "下一页>"

                ){

            list_word.append(element.toPlainText());
            list_url.append(element.attribute("href").remove(0, 7));

            //edit->appendPlainText(element.toPlainText());
            //edit->appendPlainText(element.attribute("href").remove(0, 7));
        }
    }

    if (++page_index > page_max) {

        emit signal_keyword_end();
        return;
    }

    foreach (QWebElement element, element_list) {

        if (element.toPlainText() == "下一页>") {

            QKeyEvent event(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
            this->keyPressEvent(&event);
            return;
        }
    }
}

