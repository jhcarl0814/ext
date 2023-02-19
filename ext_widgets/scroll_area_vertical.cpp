#include "scroll_area_vertical.hpp"

#include <QScrollBar>
#include <QEvent>

void ScrollAreaVertical::ctor_impl(QWidget *widget)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setWidget(widget);
    this->widget()->installEventFilter(this);
}

bool ScrollAreaVertical::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Type::LayoutRequest)
        updateGeometry();
    return QScrollArea::eventFilter(watched, event);
}

QSize ScrollAreaVertical::minimumSizeHint() const
{
    return {widget()->minimumSizeHint().width() + verticalScrollBar()->width() + frameWidth() * 2, -1};
}

QSize ScrollAreaVertical::sizeHint() const
{
    return {widget()->sizeHint().width() + verticalScrollBar()->width() + frameWidth() * 2, -1};
}
