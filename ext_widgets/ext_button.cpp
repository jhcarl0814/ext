#include "ext_button.hpp"

#include <QStyleOptionButton>
#include <QDialogButtonBox>

QSize PushButton::sizeHint() const
{
    //    Q_D(const QPushButton);
    //    if (d->sizeHint.isValid() && d->lastAutoDefault == autoDefault())
    //        return d->sizeHint;
    //    d->lastAutoDefault = autoDefault();
    ensurePolished();
    int w = 0, h = 0;
    QStyleOptionButton opt;
    initStyleOption(&opt);
    // calculate contents size...
#if !defined(QT_NO_ICON) && QT_CONFIG(dialogbuttonbox)
    bool showButtonBoxIcons = qobject_cast<QDialogButtonBox *>(parentWidget()) && style()->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons);
    if(!icon().isNull() || showButtonBoxIcons) {
        int ih = opt.iconSize.height();
        int iw = opt.iconSize.width() + 4;
        w += iw;
        h = qMax(h, ih);
    }
#endif
    QString s(text());
    bool empty = s.isEmpty();
    //    if(empty)
    //        s = QStringLiteral("XXXX");
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);
    if(!empty || !w)
        w += sz.width();
    if(!empty || !h)
        h = qMax(h, sz.height());
    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height
#if QT_CONFIG(menu)
    if(menu())
        w += style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);
#endif
    //    d->sizeHint = style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this);
    return style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(w, h), this);
}
