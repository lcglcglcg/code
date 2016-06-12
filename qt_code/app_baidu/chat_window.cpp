#include "chat_window.h"

chat_widget::chat_widget()
{

    edit_show.setReadOnly(true);
    edit_show.setContextMenuPolicy(Qt::NoContextMenu);
    edit_input.installEventFilter(this);//触发回车事件

    /**********************工具栏*****************************/

    //设置字体
    QFontComboBox *font_combobox = new QFontComboBox;
    connect(font_combobox, SIGNAL(currentFontChanged(QFont)), this, SLOT(set_font_type(QFont)));

    //字体大小
    QComboBox *font_size = new QComboBox;
    for (int n = 1; n < 100; n++)//设置字体大小范围
        font_size->addItem(QString::number(n));
    font_size->setCurrentIndex(6);
    connect(font_size, SIGNAL(currentIndexChanged(QString)), this, SLOT(set_font_size(QString)));

    //粗体
    QToolButton *buttion_bold = new QToolButton;
    buttion_bold->setCheckable(true);//设置按钮为按下/弹起独立状态
    buttion_bold->setIcon(QIcon(":/images/dialogmess/bold.png"));
    connect(buttion_bold, SIGNAL(clicked(bool)), this, SLOT(set_font_bold(bool)));

    //倾体
    QToolButton *buttion_italic = new QToolButton;
    buttion_italic->setCheckable(true);
    buttion_italic->setIcon(QIcon(":/images/dialogmess/italic.png"));
    connect(buttion_italic, SIGNAL(clicked(bool)), this, SLOT(set_font_italic(bool)));

    //下划线
    QToolButton *buttion_under = new QToolButton;
    buttion_under->setCheckable(true);
    buttion_under->setIcon(QIcon(":/images/dialogmess/under.png"));
    connect(buttion_under, SIGNAL(clicked(bool)), this, SLOT(set_font_under(bool)));

    //字体颜色
    QToolButton *buttion_color = new QToolButton;
    buttion_color->setIcon(QIcon(":/images/dialogmess/color.png"));
    connect(buttion_color, SIGNAL(clicked()), this, SLOT(set_font_color()));


    /**********************布局*****************************/

    //工具栏布局
    QHBoxLayout *layout_input_tool = new QHBoxLayout;
    layout_input_tool->addWidget(font_combobox);
    layout_input_tool->addWidget(font_size);
    layout_input_tool->addWidget(buttion_bold);
    layout_input_tool->addWidget(buttion_italic);
    layout_input_tool->addWidget(buttion_under);
    layout_input_tool->addWidget(buttion_color);
    layout_input_tool->addStretch();

    //输入框布局
    QVBoxLayout *layout_input = new QVBoxLayout;
    layout_input->addLayout(layout_input_tool);
    layout_input->addWidget(&edit_input);

    //整体布局
    QVBoxLayout *layout_show = new QVBoxLayout;
    layout_show->addWidget(&edit_show);
    layout_show->addLayout(layout_input);
    layout_show->setStretchFactor(&edit_show, 7);
    layout_show->setStretchFactor(layout_input, 3);
    setLayout(layout_show);

}

void chat_widget::set_font_type(QFont f)
{

    edit_input.setCurrentFont(f);
    edit_input.setFocus();
}

void chat_widget::set_font_size(QString size)
{

    edit_input.setFontPointSize(size.toInt());
    edit_input.setFocus();
}

void chat_widget::set_font_bold(bool check)
{

    if (check)
        edit_input.setFontWeight(QFont::Bold);
    else
        edit_input.setFontWeight(QFont::Normal);

    edit_input.setFocus();
}

void chat_widget::set_font_italic(bool check)
{

    edit_input.setFontItalic(check);
    edit_input.setFocus();
}

void chat_widget::set_font_under(bool check)
{

    edit_input.setFontUnderline(check);
    edit_input.setFocus();}

void chat_widget::set_font_color()
{

    font_color_value = QColorDialog::getColor(font_color_value, this);

    if (font_color_value.isValid()) {

        edit_input.setTextColor(font_color_value);
        edit_input.setFocus();
    }
}

bool chat_widget::eventFilter(QObject *obj, QEvent *event)
{

    //是否是edit_input的事件
    if (obj != &edit_input) return false;

    //是否是键盘事件
    if (event->type() != QEvent::KeyPress) return false;

    //是否是回车事件
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (!(keyEvent->key() == Qt::Key_Return ||
          keyEvent->key() == Qt::Key_Enter)) return false;


    edit_show.append(edit_input.toHtml());
    edit_input.clear();

    return true;
}
