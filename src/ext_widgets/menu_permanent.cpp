#include <ext_widgets/menu_permanent.hpp>
#include <ext_widgets/menu_permanent_p.hpp>

void make_top_level_menu_not_shrink(QMenu *menu)
{
    QSizePolicy p = menu->sizePolicy();
    p.setHorizontalPolicy(QSizePolicy::Policy::Minimum);
    p.setVerticalPolicy(QSizePolicy::Policy::Minimum);
    menu->setSizePolicy(p);
}

bool QObject_make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *widget = static_cast<QWidget *>(watched);
    switch(event->type())
    {
    case QEvent::Type::Hide:
        widget->setVisible(true);
        return true;
        break;
    default:
        break;
    }
    return false;
}

void make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_impl(QMenu *menu)
{
    menu->installEventFilter(new QObject_make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child(menu));
}

bool QObject_make_non_top_level_menu_not_hide_when_toggling_checkable_child::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    switch(event->type())
    {
    case QEvent::Type::MouseButtonPress:
        if(QAction *a = menu->activeAction(); a != nullptr && a->isCheckable()) // https://forum.qt.io/topic/13615/solved-how-to-keep-a-context-menu-open
        {
            a->trigger();
            return true;
        }
        break;
    case QEvent::Type::MouseButtonRelease:
        if(QAction *a = menu->activeAction(); a != nullptr && a->isCheckable())
        {
            return true;
        }
        break;
    case QEvent::Type::MouseButtonDblClick:
        if(QAction *a = menu->activeAction(); a != nullptr && a->isCheckable())
        {
            a->trigger();
            return true;
        }
        return true;
        break;
    default:
        break;
    }
    return false;
}

void make_non_top_level_menu_not_hide_when_toggling_checkable_child(QMenu *menu)
{
    menu->installEventFilter(new QObject_make_non_top_level_menu_not_hide_when_toggling_checkable_child(menu));
}

bool QObject_make_menu_not_hide_when_not_clicking_on_action::eventFilter(QObject *watched, QEvent *event)
{
    QMenu *menu = static_cast<QMenu *>(watched);
    switch(event->type())
    {
    case QEvent::Type::MouseButtonPress:
    case QEvent::Type::MouseButtonRelease:
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        //            if(menu->geometry().contains(e->pos()) && std::ranges::none_of(menu->actions() | std::views::transform([&](QAction *a)
        //                                                                                                 { return menu->actionGeometry(a); }),
        //                                                          [&](QRect actionGeometry)
        //                                                          { return actionGeometry.contains(e->pos()); }))
        //            {
        //                return true;
        //            }
        if(menu->geometry().contains(e->pos()) && menu->actionAt(e->pos()) == nullptr)
        {
            return true;
        }
    } break;
    default:
        break;
    }
    return false;
}

void make_menu_not_hide_when_not_clicking_on_action(QMenu *menu)
{
    menu->installEventFilter(new QObject_make_menu_not_hide_when_not_clicking_on_action(menu));
}
