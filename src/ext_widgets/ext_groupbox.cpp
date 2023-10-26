#include <ext_widgets/ext_groupbox.hpp>

#include <QStyleOptionGroupBox>

QSize GroupBox::minimumSizeHint() const // restoring: https://stackoverflow.com/questions/11696143/how-can-i-make-a-qgroupbox-with-a-title-that-is-two-lines
{
    QStyleOptionGroupBox option;
    initStyleOption(&option);

    QFontMetrics metrics(fontMetrics());

    //    int baseWidth = metrics.horizontalAdvance(this->title()) + metrics.horizontalAdvance(u' ');
    //    int baseHeight = metrics.height();
    int baseWidth = metrics.size(Qt::TextShowMnemonic, this->title() + u' ').width();
    int baseHeight = metrics.size(Qt::TextShowMnemonic, this->title()).height();

    if(this->isCheckable())
    {
        baseWidth += style()->pixelMetric(QStyle::PM_IndicatorWidth, &option);
        baseWidth += style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing, &option);
        baseHeight = qMax(baseHeight, style()->pixelMetric(QStyle::PM_IndicatorHeight, &option));
    }

    QSize size = style()->sizeFromContents(QStyle::CT_GroupBox, &option, QSize(baseWidth, baseHeight), this);
    return size.expandedTo(QWidget::minimumSizeHint());
}
