#include <ext_widgets/menu_positioned.hpp>
#include <ext_widgets/menu_positioned_p.hpp>

#include <QEvent>
#include <QScreen>

bool QObject_make_menu_button_menu::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    QWidget *btn = menu->parentWidget();
    switch(event->type())
    {
    case QEvent::Type::Show:
        menu->move(btn->parentWidget()->mapToGlobal(btn->pos()) - QPoint(0, menu->size().height()));
        return true;
        break;
    default:
        break;
    }
    return false;
}

void make_menu_button_menu(QMenu *menu, QPushButton *btn)
{
    btn->setMenu(menu);
    menu->installEventFilter(new QObject_make_menu_button_menu(menu));
}

bool QObject_make_menu_top_right_menu::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    switch(event->type())
    {
    case QEvent::Type::Show:
        menu->move(pos - QPoint(0, menu->size().height()));
        return true;
        break;
    default:
        break;
    }
    return false;
}

void make_menu_top_right_menu(QMenu *menu, QPoint pos)
{
    QObject_make_menu_top_right_menu *o = new QObject_make_menu_top_right_menu(menu);
    o->pos = pos;
    menu->installEventFilter(o);
}

bool QObject_make_menu_in_screen::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    switch(event->type())
    {
    case QEvent::Type::Show:
    {
        QRect screenGeometry = menu->screen()->geometry();
        if(menu->geometry().right() > screenGeometry.right())
        {
            menu->move(menu->pos() - QPoint(menu->geometry().width(), 0));
        }
        if(menu->geometry().left() < screenGeometry.left())
        {
            menu->move(menu->pos() + QPoint(screenGeometry.left() - menu->geometry().left(), 0));
        }
        if(menu->geometry().bottom() > screenGeometry.bottom())
        {
            if(QMenu *parentMenu = qobject_cast<QMenu *>(menu->parentWidget()); parentMenu != nullptr)
            {
                menu->move(menu->pos() - QPoint(0, menu->geometry().height() - parentMenu->actionGeometry(menu->menuAction()).height()));
            }
            else
            {
                menu->move(menu->pos() - QPoint(0, menu->geometry().height()));
            }
        }
        if(menu->geometry().top() < screenGeometry.top())
        {
            menu->move(menu->pos() + QPoint(0, screenGeometry.top() - menu->geometry().top()));
        }
        return true;
    } break;
    default:
        break;
    }
    return false;
}

void make_menu_in_screen(QMenu *menu)
{
    menu->installEventFilter(new QObject_make_menu_in_screen(menu));
}
