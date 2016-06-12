#include "keyword.h"

keyword_web::keyword_web()
{
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(keyword_complete(bool)));
}

void keyword_web::keyword_start(QString keyword, int page_max)
{

    this->page_index = 0;
    this->page_max = page_max;
    this->keyword = keyword;

    url_list.clear();

    QUrl url = QString(tr("http://www.baidu.com/s?wd=") + keyword);
    this->mainFrame()->load(url);
}

void keyword_web::keyword_complete(bool complete)
{
    if (!complete) return;

    QWebFrame  *frame = this->currentFrame();

    QWebElement document = frame->documentElement();
    QList <QWebElement> element_list = document.findAll("a[href]").toList();

    foreach (QWebElement element, element_list) {


        if (element.isNull()) return;
        element.setFocus();

        QString str = element.toPlainText();

        if (!str.compare("下一页>")) {

            if (++page_index < page_max) {

                QKeyEvent event(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
                this->event(static_cast<QEvent *>(&event));
                return;
            }

            emit keyword_signal(url_list);
            return;
        }

        if (str.compare("")
                && str.compare("更多»")
                && str.compare("搜索设置")
                && str.compare("百度首页")
                && str.compare("登录")
                && str.compare("注册")
                && str.compare("新闻")
                && str.compare("贴吧")
                && str.compare("知道")
                && str.compare("MP3")
                && str.compare("图片")
                && str.compare("视频")
                && str.compare("地图")
                && str.compare("百科")
                && str.compare("文库")
                && str.compare("词典")
                && str.compare("更多>>")
                && str.compare("手写")
                && str.compare("拼音")
                && str.compare("关闭")
                && str.compare("把百度设为主页")
                && str.compare("把百度设为主页")
                && str.compare("推广链接")
                && str.compare("▶来百度推广您的产品")
                && str.compare("百度快照")
                && str.compare("把百度添加到桌面")
                && str.compare("举报图片")
                && str.compare("精品区")
                && str.compare("图片区")
                && str.compare("视频区")
                && str.compare("更多文库相关文档>>")
                && str.compare("更多贴吧相关讨论>>")
                && str.compare("更多知道相关问题>>")
                ){

            QString url = element.attribute("href");
            QString temp = url.mid(0, sizeof("http://www.baidu.com/link?url") - 1);

            if (temp.compare("http://www.baidu.com/link?url") == 0) {

                url_list << str;
            }
        }
    }
}

