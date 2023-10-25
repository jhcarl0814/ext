#ifndef COLORED_WIDGET_H
#define COLORED_WIDGET_H

#include <QWidget>

void set_widget_background_color(QWidget *widget, QColor const &color = Qt::GlobalColor::red);

void set_widget_opacity(QWidget *widget, qreal opacity);

#endif // COLORED_WIDGET_H
