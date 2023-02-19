#ifndef CONSUMING_CLICK_WIDGET_H
#define CONSUMING_CLICK_WIDGET_H

#include <QWidget>

class Widget_consuming_click: public QWidget
{
    Q_OBJECT
  public:
    using QWidget::QWidget;

  protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // CONSUMING_CLICK_WIDGET_H
