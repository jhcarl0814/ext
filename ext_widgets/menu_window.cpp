#include "menu_window.hpp"

#include <QHBoxLayout>
#include "../ext_widgets/widget_consuming_click.hpp"

void attach_widget_consuming_click(QMenu *menu)
{
    Widget_consuming_click *widget_consuming_click = new Widget_consuming_click();
    menu->setProperty("widget_consuming_click", QVariant::fromValue<QObject *>(widget_consuming_click));
    QHBoxLayout *menu_hlayout = new QHBoxLayout(menu);
    menu_hlayout->setContentsMargins(QMargins());
    menu_hlayout->addWidget(widget_consuming_click);
}
