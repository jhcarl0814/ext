#include "keep_window_stays_on_top.hpp"
#include "keep_window_stays_on_top_p.hpp"

#include <QTimer>

bool QObject_keep_window_stays_on_top::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *widget = static_cast<QWidget *>(watched);
    switch(event->type())
    {
    case QEvent::Type::WindowDeactivate:
        widget->raise();
        return true;
        break;
    }
    return false;
}

void keep_window_stays_on_top(QWidget *this_)
{
    this_->installEventFilter(new QObject_keep_window_stays_on_top(this_));
    QTimer *timer = new QTimer(this_);
    timer->setInterval(1000);
    timer->setTimerType(Qt::TimerType::PreciseTimer);
    QObject::connect(timer, &QTimer::timeout, this_, [this_]()
        { this_->raise(); });
    timer->start();
}

//void keep_window_stays_on_top(QWidget *this_)
//{
//    declare_ext_debug_guard((this_), qDebug_compact());
//    QTimer *timer = new QTimer(this_);
//    timer->setInterval(5000);
//    timer->setTimerType(Qt::TimerType::PreciseTimer);
//    QObject::connect(timer, &QTimer::timeout, this_, [this_]()
//      {
//            this_->setWindowState(Qt::WindowState::WindowMinimized);
//            this_->setWindowState(Qt::WindowState::WindowNoState);
//            this_->activateWindow(); });
//    timer->start();
//}
