#ifndef MENU_POSITIONED_H
#define MENU_POSITIONED_H

#include <QMenu>
#include <QPushButton>

void make_menu_button_menu(QMenu *menu, QPushButton *btn);

void make_menu_top_right_menu(QMenu *menu, QPoint pos);

void make_menu_in_screen(QMenu *menu);

#endif // MENU_POSITIONED_H
