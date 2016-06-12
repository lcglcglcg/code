#ifndef LG_EDIT_H
#define LG_EDIT_H

#include <QWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QFontComboBox>
#include <QToolButton>
#include <QLayout>
#include <QtWebKit>
#include <QColor>
#include <QMessageBox>

class lg_edit : public QWidget
{
    Q_OBJECT

public:
    lg_edit();
    ~lg_edit();

    /**********************显示*****************************/

    QTabWidget      show_tab;//显示框标签
    QTextEdit       show_edit;//聊天显示框
    QWebView        show_web;

    /**********************输入*****************************/

    QFontComboBox   font_combobox;//(输入工具栏)字体设置

    QComboBox       font_size;//(输入工具栏)字体大小
    QStringList     font_size_list;//(输入工具栏)字体大小范围

    QToolButton     font_bold;//(输入工具栏)粗体(按钮)
    QIcon           font_bold_ico;

    QToolButton     font_italic;//(输入工具栏)斜体(按钮)
    QIcon           font_italic_ico;

    QToolButton     font_under;//(输入工具栏)字体下划线(按钮)
    QIcon           font_under_ico;

    QToolButton     font_color;//(输入工具栏)字体颜色(按钮)
    QIcon           font_color_ico;
    QColor          font_color_value;//字体颜色值

    QToolButton     open_file;//(输入工具栏)发送文件(按钮)
    QIcon           open_file_ico;

    QTextEdit       input_edit;//输入框

    /**********************布局*****************************/

    QVBoxLayout     layout;//整体布局
    QVBoxLayout     layout_input;//输入框布局
    QHBoxLayout     layout_input_tool;//输入工具栏布局

public slots:
    void set_font(QFont f);//设置字体
    void set_font_size(QString size);//设置字体大小
    void set_font_bold(bool check);//设置粗体
    void set_font_italic(bool check);//设置斜体
    void set_font_under(bool check);//设置字体下划线
    void set_font_color();//设置字体颜色

protected:
    bool eventFilter(QObject *obj, QEvent *event);//捕获键盘事件
    QMessageBox     mess_box;
};

#endif // LG_EDIT_H
