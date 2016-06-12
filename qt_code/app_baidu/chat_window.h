#ifndef LG_EDIT_H
#define LG_EDIT_H

#include <QEvent>
#include <QKeyEvent>

#include <QColor>
#include <QColorDialog>

#include <QTextEdit>
#include <QToolButton>
#include <QFontComboBox>

#include <QLayout>
#include <QWidget>

#include <QDebug>

class chat_widget : public QWidget
{
    Q_OBJECT

public:
    chat_widget();

    QTextEdit       edit_show;//显示框
    QTextEdit       edit_input;//输入框

public slots:
    void set_font_type(QFont f);//设置字体
    void set_font_size(QString size);//字体设置大小
    void set_font_bold(bool check);//字体设置粗体
    void set_font_italic(bool check);//字体设置斜体
    void set_font_under(bool check);//字体设置下划线
    void set_font_color();//字体设置颜色

protected:
    bool eventFilter(QObject *obj, QEvent *event);//捕获键盘事件

private:
    QColor font_color_value;
};

#endif // LG_EDIT_H
