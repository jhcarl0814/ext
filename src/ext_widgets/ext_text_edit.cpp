#include <ext_widgets/ext_text_edit.hpp>

void make_plain_text_edit_readonly(QPlainTextEdit *plain_text_edit)
{
    plain_text_edit->setReadOnly(true);
    plain_text_edit->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse | Qt::TextInteractionFlag::TextSelectableByKeyboard | Qt::TextInteractionFlag::LinksAccessibleByMouse | Qt::TextInteractionFlag::LinksAccessibleByKeyboard);
    {
        QPalette palette = plain_text_edit->palette();
        palette.setColor(QPalette::Base, palette.color(QPalette::Window));
        plain_text_edit->setPalette(palette);
    }
}

void make_line_edit_readonly(QLineEdit *line_edit)
{
    line_edit->setReadOnly(true);
    //    line_edit->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse | Qt::TextInteractionFlag::TextSelectableByKeyboard | Qt::TextInteractionFlag::LinksAccessibleByMouse | Qt::TextInteractionFlag::LinksAccessibleByKeyboard);
    {
        QPalette palette = line_edit->palette();
        palette.setColor(QPalette::Base, palette.color(QPalette::Window));
        line_edit->setPalette(palette);
    }
}
