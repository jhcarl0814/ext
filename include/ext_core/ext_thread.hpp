#ifndef EXT_CORE__EXT_THREAD_H
#define EXT_CORE__EXT_THREAD_H

#include <coroutine>

#include <QThread>
#include <QAbstractEventDispatcher>
#include <QTimer>
#include <QCoreApplication>

//template<typename Fn>
//decltype(auto) post_to_thread(QThread *thread, Fn &&fn, Qt::ConnectionType type = Qt::AutoConnection) // https://stackoverflow.com/questions/21646467/how-to-execute-a-functor-or-a-lambda-in-a-given-thread-in-qt-gcd-style
//{
//    if constexpr(std::is_same_v<decltype(std::move(fn)()), void>)
//    {
//        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(thread), std::forward<Fn>(fn), type);
//        return;
//    }
//    else
//    {
//        decltype(fn()) retVal;
//        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(thread), std::forward<Fn>(fn), type, Q_RETURN_ARG(QString, retVal));
//        return retVal;
//    }
//}

//template<typename Fn>
//decltype(auto) post_to_object(QObject *object, Fn &&fn, Qt::ConnectionType type = Qt::AutoConnection)
//{
//    if constexpr(std::is_same_v<decltype(std::move(fn)()), void>)
//    {
//        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(object->thread()), std::forward<Fn>(fn), type);
//        return;
//    }
//    else
//    {
//        decltype(fn()) retVal;
//        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(object->thread()), std::forward<Fn>(fn), type, Q_RETURN_ARG(QString, retVal));
//        return retVal;
//    }
//}

struct coroutine_dispatch_to_main_thread_t
{
    Qt::ConnectionType type = Qt::AutoConnection;
    bool await_ready() const { return QThread::currentThread() == QCoreApplication::instance()->thread(); }
    void await_suspend(std::coroutine_handle<> fn) const
    {
        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(QCoreApplication::instance()->thread()), fn, type);
    }
    void await_resume() const {}
};
struct coroutine_dispatch_to_thread_t
{
    QThread *thread;
    Qt::ConnectionType type = Qt::AutoConnection;
    bool await_ready() const { return QThread::currentThread() == thread; }
    void await_suspend(std::coroutine_handle<> fn) const
    {
        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(thread), fn, type);
    }
    void await_resume() const {}
};
//struct coroutine_dispatch_to_object_t
//{
//    QObject *object;
//    Qt::ConnectionType type = Qt::AutoConnection;
//    bool await_ready() const { return QThread::currentThread() == object->thread(); }
//    void await_suspend(std::coroutine_handle<> fn) const
//    {
//        QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(object->thread()), fn, type);
//    }
//    void await_resume() const {}
//};
struct coroutine_post_to_same_thread_delay_t
{
    int msec;
    Qt::TimerType timerType = Qt::TimerType::PreciseTimer;
    bool await_ready() const { return false; }
    void await_suspend(std::coroutine_handle<> fn) const
    {
        QTimer::singleShot(msec, QAbstractEventDispatcher::instance(QThread::currentThread()), fn);
    }
    void await_resume() const {}
};

//#include <ext_core/ext_object.hpp>
//QThread *thread = new QThread(qApp);
//auto obj = make_QObject();
//obj->moveToThread(thread);
//QObject::connect(obj.get(), &QObject::destroyed, thread, &QThread::quit);
//QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
//QMetaObject::invokeMethod(
//    obj.get(), [obj = std::move(obj)]()
//    {
//        QMetaObject::invokeMethod(qApp,[](){
//            },Qt::ConnectionType::QueuedConnection);
//        obj->deleteLater(); });
//thread->start();

#endif // EXT_CORE__EXT_THREAD_H
