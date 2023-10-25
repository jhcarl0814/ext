#ifndef MENU_NESTED_P_H
#define MENU_NESTED_P_H

#include <QMenu>
#include <functional>

class QObject_make_menu_not_trigger_child_when_right_click_child: public QObject // https://stackoverflow.com/questions/3771654/clickable-menu-item-with-submenu-in-qt
{
    Q_OBJECT
  public:
    using QObject::QObject;
    std::function<void(QMenu *, QAction *, QPoint, QMenu *)> MouseRightButtonPress_handler /*, MouseButtonDblClick_handler*/;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MENU_NESTED_P_H
