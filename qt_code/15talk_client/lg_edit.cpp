#include "lg_edit.h"
#include "lg_tree.h"
#include <QColorDialog>

extern lg_tree *global_tree;
extern lg_edit *global_edit;

lg_edit::lg_edit()
{

    /**********************显示框*****************************/

    QIcon icon(":/images/paste.png");

    show_edit.setContextMenuPolicy(Qt::NoContextMenu);
    show_edit.setReadOnly(true);
    show_tab.addTab(&show_edit, icon, "edit");

    show_web.setUrl(QUrl("http://www.baidu.com"));
    show_tab.addTab(&show_web, icon, "web");

    /**********************输入*****************************/

    for (int n = 6; n < 100; n++)//设置字体大小范围
        font_size_list << QString::number(n);

    font_size.addItems(font_size_list);
    font_size.setCurrentIndex(6);

    font_bold.setCheckable(true);//设置按钮为按下/弹起独立状态
    font_bold.setIcon(QIcon(":/images/textbold.png"));

    font_italic.setCheckable(true);//设置按钮为按下/弹起独立状态
    font_italic.setIcon(QIcon(":/images/textitalic.png"));

    font_under.setCheckable(true);//设置按钮为按下/弹起独立状态
    font_under.setIcon(QIcon(":/images/textunder.png"));

    font_color.setIcon(QIcon(":/images/textcolor.png"));
    open_file.setIcon(QIcon(":/images/textopenfile.png"));

    input_edit.installEventFilter(this);//回车触发事件(发送信息)

    /**********************工具栏链接槽*****************************/

    connect(&font_combobox, SIGNAL(currentFontChanged(QFont)), this, SLOT(set_font(QFont)));
    connect(&font_size, SIGNAL(currentIndexChanged(QString)), this ,SLOT(set_font_size(QString)));
    connect(&font_bold, SIGNAL(clicked(bool)), this, SLOT(set_font_bold(bool)));
    connect(&font_italic, SIGNAL(clicked(bool)), this, SLOT(set_font_italic(bool)));
    connect(&font_under, SIGNAL(clicked(bool)), this, SLOT(set_font_under(bool)));
    connect(&font_color, SIGNAL(clicked()), this, SLOT(set_font_color()));

    /**********************布局*****************************/

    //工具栏布局
    layout_input_tool.addWidget(&font_combobox);
    layout_input_tool.addWidget(&font_size);
    layout_input_tool.addWidget(&font_bold);
    layout_input_tool.addWidget(&font_italic);
    layout_input_tool.addWidget(&font_under);
    layout_input_tool.addWidget(&font_color);
    layout_input_tool.addWidget(&open_file);
    layout_input_tool.addStretch();//不随界面大小自动拉伸

    //输入框布局
    layout_input.addLayout(&layout_input_tool);
    layout_input.addWidget(&input_edit);

    //整体布局
    layout.addWidget(&show_tab);
    layout.addLayout(&layout_input);
    layout.setStretchFactor(&show_tab, 7);
    layout.setStretchFactor(&layout_input, 3);
    setLayout(&layout);
}

lg_edit::~lg_edit()
{

}

void lg_edit::set_font(QFont f)
{
    input_edit.setCurrentFont(f);
    input_edit.setFocus();
}

void lg_edit::set_font_size(QString size)
{
    input_edit.setFontPointSize(size.toInt());
    input_edit.setFocus();
}

void lg_edit::set_font_bold(bool check)
{
    if (check)
        input_edit.setFontWeight(QFont::Bold);
    else
        input_edit.setFontWeight(QFont::Normal);

    input_edit.setFocus();
}

void lg_edit::set_font_italic(bool check)
{
    input_edit.setFontItalic(check);
    input_edit.setFocus();
}

void lg_edit::set_font_under(bool check)
{
    input_edit.setFontUnderline(check);
    input_edit.setFocus();
}

void lg_edit::set_font_color()
{
    font_color_value = QColorDialog::getColor(font_color_value, this);

    if (font_color_value.isValid()) {

        input_edit.setTextColor(font_color_value);
        input_edit.setFocus();
    }
}

bool lg_edit::eventFilter(QObject *obj, QEvent *event)
{

    //是否是lg_edit的事件
    if (obj != &input_edit) return false;

    //是否是键盘事件
    if (event->type() != QEvent::KeyPress) return false;

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

    //是否是回车事件
    if (!(keyEvent->key() == Qt::Key_Return ||
          keyEvent->key() == Qt::Key_Enter)) return false;

    //lg_tree当前焦点是否为空
    if (!global_tree->currentItem()) {

        mess_box.setText(tr("没有选择患者"));
        mess_box.exec();
        return false;
    }

    //lg_tree当前焦点是否是访客(web)
    if (global_tree->currentItem()->text(1) != _TREE_WEB_USER_STRING) {

        mess_box.setText(tr("没有选择患者"));
        mess_box.exec();
        return false;
    }

    //当前访客(web)不在对话中
    if (global_tree->currentItem()->text(2) != _TREE_DIALOG_STRING) {

        mess_box.setText(tr("患者不在对话中"));
        mess_box.exec();
        return false;
    }

    //将新消息添加到此访客buffer并将此buffer显示到showedit
    lg_tree_web_user *web_user = static_cast<lg_tree_web_user *>(global_tree->currentItem());
    web_user->mess_buffer += input_edit.toHtml() + tr("\n");

    show_edit.setText(web_user->mess_buffer);
    input_edit.clear();

    return true;
}
