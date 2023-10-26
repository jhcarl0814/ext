#include <ext_widgets/widget_consuming_click.hpp>

#include <QMouseEvent>

void Widget_consuming_click::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}
