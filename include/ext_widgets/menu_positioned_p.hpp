#ifndef MENU_POSITIONED_P_H
#define MENU_POSITIONED_P_H

#include <QObject>
#include <QPoint>

class QObject_make_menu_button_menu: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

class QObject_make_menu_top_right_menu: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;
    QPoint pos;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

class QObject_make_menu_in_screen: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MENU_POSITIONED_P_H
