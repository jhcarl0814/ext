#ifndef KEEP_WINDOW_STAYS_ON_TOP_P_H
#define KEEP_WINDOW_STAYS_ON_TOP_P_H

#include <QEvent>

#include <QWidget>

class QObject_keep_window_stays_on_top: public QObject
{
    Q_OBJECT
  public:
    using QObject::QObject;

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // KEEP_WINDOW_STAYS_ON_TOP_P_H
