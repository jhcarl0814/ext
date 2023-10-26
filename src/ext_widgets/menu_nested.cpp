#include <ext_widgets/menu_nested.hpp>
#include <ext_widgets/menu_nested_p.hpp>

#include <QMouseEvent>

bool QObject_make_menu_not_trigger_child_when_right_click_child::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    switch(event->type())
    {
    case QEvent::Type::MouseButtonRelease:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->button() == Qt::MouseButton::RightButton)
        {
            if(QAction *a = menu->actionAt(e->pos()); a != nullptr)
            {
                MouseRightButtonPress_handler(menu, a, e->pos(), menu);
                return true;
            }
            if(QMenu *parentMenu = qobject_cast<QMenu *>(menu->parentWidget()); parentMenu != nullptr)
            {
                QPoint pos_in_parent = parentMenu->mapFromGlobal(menu->mapToGlobal(e->pos()));
                if(QAction *a_in_parent = parentMenu->actionAt(pos_in_parent); a_in_parent != nullptr)
                {
                    MouseRightButtonPress_handler(parentMenu, a_in_parent, pos_in_parent, menu);
                    return true;
                }
            }
        }
    } break;
    case QEvent::Type::MouseButtonPress:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->button() == Qt::MouseButton::RightButton)
        {
            if(QAction *a = menu->actionAt(e->pos()); a != nullptr)
            {
                return true;
            }
            if(QMenu *parentMenu = qobject_cast<QMenu *>(menu->parentWidget()); parentMenu != nullptr)
            {
                QPoint pos_in_parent = parentMenu->mapFromGlobal(menu->mapToGlobal(e->pos()));
                if(QAction *a_in_parent = parentMenu->actionAt(pos_in_parent); a_in_parent != nullptr)
                {
                    return true;
                }
            }
        }
    } break;
    }
    return false;
}

void make_menu_not_trigger_child_when_right_click_child(QMenu *menu, std::function<void(QMenu *, QAction *, QPoint, QMenu *)> MouseRightButtonPress_handler)
{
    QObject_make_menu_not_trigger_child_when_right_click_child *o = new QObject_make_menu_not_trigger_child_when_right_click_child(menu);
    o->MouseRightButtonPress_handler = MouseRightButtonPress_handler;
    menu->installEventFilter(o);
}
