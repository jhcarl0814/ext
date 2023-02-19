#include "widget_colored.hpp"

#include <QGraphicsOpacityEffect>

void set_widget_background_color(QWidget *widget, QColor const &color)
{
    {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::Window, color);
        widget->setPalette(pal);
    }
    widget->setAutoFillBackground(true);
}

void set_widget_opacity(QWidget *widget, qreal opacity)
{
    {
        QGraphicsOpacityEffect *op = new QGraphicsOpacityEffect(widget);
        op->setOpacity(opacity);
        widget->setGraphicsEffect(op);
    }
    //    widget->setAutoFillBackground(true);
}
