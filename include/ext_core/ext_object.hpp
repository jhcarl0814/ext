#ifndef EXT_OBJECT_H
#define EXT_OBJECT_H

#include <QObject>

template<typename Deleter = decltype(std::mem_fn(&QObject::deleteLater))>
std::unique_ptr<QObject, Deleter> make_QObject(Deleter deleter = std::mem_fn(&QObject::deleteLater))
{
    return std::unique_ptr<QObject, Deleter>(new QObject(), std::forward<Deleter>(deleter));
}

#endif // EXT_OBJECT_H
