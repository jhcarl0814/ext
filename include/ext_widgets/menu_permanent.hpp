#ifndef MENU_PERMANENT_H
#define MENU_PERMANENT_H

#include <QMenu>

void make_top_level_menu_not_shrink(QMenu *menu);

void make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_impl(QMenu *menu);

constexpr inline auto make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_default_fn = [] {};

template<typename Fn_Close_Window_Menu = decltype(make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_default_fn)>
void make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child(
    QMenu *menu, Fn_Close_Window_Menu fn_close_window_menu = make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_default_fn)
{
    make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child_impl(menu);
    QObject::connect(menu, &QMenu::triggered, menu, [/*menu, */ fn_close_window_menu](QAction *action)
        {
            //            ext_debug_log((menu), qDebug_compact());
            if(!action->isCheckable())
                fn_close_window_menu(); //
        });
}

void make_non_top_level_menu_not_hide_when_toggling_checkable_child(QMenu *menu);

void make_menu_not_hide_when_not_clicking_on_action(QMenu *menu);

#endif // MENU_PERMANENT_H
