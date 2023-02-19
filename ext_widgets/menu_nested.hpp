#ifndef MENU_NESTED_H
#define MENU_NESTED_H

#include <QMenu>

void make_menu_not_trigger_child_when_right_click_child(QMenu *menu, std::function<void(QMenu *, QAction *, QPoint, QMenu *)> MouseRightButtonPress_handler);

#endif // MENU_NESTED_H
