#ifndef MENU_PERMANENT_P_H
#define MENU_PERMANENT_P_H

#include <QMouseEvent>

#include <QMenu>

// not used: https://stackoverflow.com/questions/2050462/prevent-a-qmenu-from-closing-when-one-of-its-qaction-is-triggered
class QObject_make_top_level_menu_not_hide_when_toggling_checkable_child_and_not_hide_and_close_some_window_menu_when_clicking_child: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

class QObject_make_non_top_level_menu_not_hide_when_toggling_checkable_child: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

class QObject_make_menu_not_hide_when_not_clicking_on_action: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MENU_PERMANENT_P_H
