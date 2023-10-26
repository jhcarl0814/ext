#include <ext_widgets/ext_style.hpp>

#include <QStyleOptionGroupBox>
#include <QPainter>

// C:\Qt\6.4.2\Src\qtbase\src\widgets\styles\qcommonstyle.cpp

QSize MyProxyStyle::sizeFromContents(QStyle::ContentsType ct, QStyleOption const *opt, QSize const &csz, QWidget const *widget) const
{
    QSize sz(csz);
    switch(ct) {
    case CT_PushButton:
        if(QStyleOptionButton const *btn = qstyleoption_cast<QStyleOptionButton const *>(opt)) {
            sz = QCommonStyle::sizeFromContents(ct, opt, csz, widget);
            int w = sz.width(),
                h = sz.height();
            int defwidth = 0;
            if(btn->features & QStyleOptionButton::AutoDefaultButton)
                defwidth = 2 * proxy()->pixelMetric(PM_ButtonDefaultIndicator, btn, widget);
            //            const qreal dpi = QStyleHelper::dpi(opt);
            //            int minwidth = int(QStyleHelper::dpiScaled(75, dpi));
            //            int minheight = int(QStyleHelper::dpiScaled(23, dpi));
            int minwidth = 0;
            int minheight = 0;
            if(w < minwidth + defwidth && !btn->text.isEmpty())
                w = minwidth + defwidth;
            if(h < minheight + defwidth)
                h = minheight + defwidth;
            sz = QSize(w, h);
        }
        break;
    default:
        sz = QProxyStyle::sizeFromContents(ct, opt, csz, widget);
        break;
    }
    return sz;
}

int MyProxyStyle::styleHint(StyleHint hint, QStyleOption const *option, QWidget const *widget, QStyleHintReturn *returnData) const
{
    if(hint == QStyle::StyleHint::SH_ToolTip_WakeUpDelay) // https://forum.qt.io/topic/90403/show-tooltip-immediatly
        return 0;
    //        else if(hint == QStyle::StyleHint::SH_Menu_SubMenuPopupDelay)
    //            return 0;
    //        else if(hint == QStyle::StyleHint::SH_Menu_SubMenuSloppyCloseTimeout)
    //            return 0;
    else
        return QProxyStyle::styleHint(hint, option, widget, returnData);
}

// restoring: https://stackoverflow.com/questions/11696143/how-can-i-make-a-qgroupbox-with-a-title-that-is-two-lines

void MyProxyStyle::drawComplexControl(ComplexControl cc, QStyleOptionComplex const *opt, QPainter *p, QWidget const *widget) const
{
    switch(cc) {
#if QT_CONFIG(groupbox)
    case CC_GroupBox:
        if(QStyleOptionGroupBox const *groupBox = qstyleoption_cast<QStyleOptionGroupBox const *>(opt)) {
            // Draw frame
            QRect textRect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxLabel, widget);
            QRect checkBoxRect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxCheckBox, widget);
            if(groupBox->subControls & QStyle::SC_GroupBoxFrame) {
                QStyleOptionFrame frame;
                frame.QStyleOption::operator=(*groupBox);
                frame.features = groupBox->features;
                frame.lineWidth = groupBox->lineWidth;
                frame.midLineWidth = groupBox->midLineWidth;
                frame.rect = proxy()->subControlRect(CC_GroupBox, opt, SC_GroupBoxFrame, widget);
                p->save();
                QRegion region(groupBox->rect);
                if(!groupBox->text.isEmpty()) {
                    bool ltr = groupBox->direction == Qt::LeftToRight;
                    QRect finalRect;
                    if(groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
                        finalRect = checkBoxRect.united(textRect);
                        finalRect.adjust(ltr ? -4 : 0, 0, ltr ? 0 : 4, 0);
                    }
                    else {
                        finalRect = textRect;
                    }
                    region -= finalRect;
                }
                p->setClipRegion(region);
                proxy()->drawPrimitive(PE_FrameGroupBox, &frame, p, widget);
                p->restore();
            }

            // Draw title
            if((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
                QColor textColor = groupBox->textColor;
                if(textColor.isValid())
                    p->setPen(textColor);
                int alignment = int(groupBox->textAlignment);
                if(!proxy()->styleHint(QStyle::SH_UnderlineShortcut, opt, widget))
                    alignment |= Qt::TextHideMnemonic;

                using namespace Qt::Literals::StringLiterals;
                QRect textRect2 = textRect.adjusted(opt->fontMetrics.size(Qt::TextShowMnemonic, u" "_s).width() / 2, 0, -opt->fontMetrics.size(Qt::TextShowMnemonic, u" "_s).width() / 2, 0);

                proxy()->drawItemText(p, textRect2, Qt::TextShowMnemonic /*| Qt::AlignHCenter*/ | alignment,
                    groupBox->palette, groupBox->state & State_Enabled, groupBox->text,
                    textColor.isValid() ? QPalette::NoRole : QPalette::WindowText);

                if(groupBox->state & State_HasFocus) {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=(*groupBox);
                    fropt.rect = textRect;
                    proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
                }
            }

            // Draw checkbox
            if(groupBox->subControls & SC_GroupBoxCheckBox) {
                QStyleOptionButton box;
                box.QStyleOption::operator=(*groupBox);
                box.rect = checkBoxRect;
                proxy()->drawPrimitive(PE_IndicatorCheckBox, &box, p, widget);
            }
        }
        break;
#endif // QT_CONFIG(groupbox)
    default:
        QProxyStyle::drawComplexControl(cc, opt, p, widget);
    }
}

QRect MyProxyStyle::subControlRect(ComplexControl cc, QStyleOptionComplex const *opt, SubControl sc, QWidget const *widget) const
{
    QRect ret;
    switch(cc)
    {
#if QT_CONFIG(groupbox)
    case CC_GroupBox:
    {
        if(QStyleOptionGroupBox const *groupBox = qstyleoption_cast<QStyleOptionGroupBox const *>(opt))
        {
            switch(sc)
            {
            case SC_GroupBoxFrame:
            case SC_GroupBoxContents:
            {
                int topMargin = 0;
                int topHeight = 0;
                int verticalAlignment = proxy()->styleHint(SH_GroupBox_TextLabelVerticalAlignment, groupBox, widget);
                bool hasCheckBox = groupBox->subControls & QStyle::SC_GroupBoxCheckBox;
                if(groupBox->text.size() || hasCheckBox)
                {
                    int checkBoxHeight = hasCheckBox ? proxy()->pixelMetric(PM_IndicatorHeight, groupBox, widget) : 0;
                    topHeight = qMax(groupBox->fontMetrics.size(Qt::TextFlag::TextShowMnemonic, groupBox->text).height(), checkBoxHeight);
                    if(verticalAlignment & Qt::AlignVCenter)
                        topMargin = topHeight / 2;
                    else if(verticalAlignment & Qt::AlignTop)
                        topMargin = topHeight;
                }

                QRect frameRect = groupBox->rect;
                frameRect.setTop(topMargin);

                if(sc == SC_GroupBoxFrame)
                {
                    ret = frameRect;
                    break;
                }

                int frameWidth = 0;
                if((groupBox->features & QStyleOptionFrame::Flat) == 0)
                    frameWidth = proxy()->pixelMetric(PM_DefaultFrameWidth, groupBox, widget);
                ret = frameRect.adjusted(frameWidth, frameWidth + topHeight - topMargin,
                    -frameWidth, -frameWidth);
                break;
            }
            case SC_GroupBoxCheckBox:
            case SC_GroupBoxLabel:
            {
                QFontMetrics fontMetrics = groupBox->fontMetrics;
                int th = fontMetrics.size(Qt::TextShowMnemonic, groupBox->text).height();
                using namespace Qt::Literals::StringLiterals;
                int tw = fontMetrics.size(Qt::TextShowMnemonic, groupBox->text).width() + fontMetrics.size(Qt::TextShowMnemonic, u" "_s).width();
                int marg = (groupBox->features & QStyleOptionFrame::Flat) ? 0 : 8;
                ret = groupBox->rect.adjusted(marg, 0, -marg, 0);

                int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, opt, widget);
                int indicatorHeight = proxy()->pixelMetric(PM_IndicatorHeight, opt, widget);
                int indicatorSpace = proxy()->pixelMetric(PM_CheckBoxLabelSpacing, opt, widget) - 1;
                bool hasCheckBox = groupBox->subControls & QStyle::SC_GroupBoxCheckBox;
                int checkBoxWidth = hasCheckBox ? (indicatorWidth + indicatorSpace) : 0;
                int checkBoxHeight = hasCheckBox ? indicatorHeight : 0;

                int h = qMax(th, checkBoxHeight);
                ret.setHeight(h);

                // Adjusted rect for label + indicatorWidth + indicatorSpace
                QRect totalRect = alignedRect(groupBox->direction, groupBox->textAlignment,
                    QSize(tw + checkBoxWidth, h), ret);

                // Adjust totalRect if checkbox is set
                if(hasCheckBox)
                {
                    bool ltr = groupBox->direction == Qt::LeftToRight;
                    int left = 0;
                    // Adjust for check box
                    if(sc == SC_GroupBoxCheckBox)
                    {
                        left = ltr ? totalRect.left() : (totalRect.right() - indicatorWidth);
                        int top = totalRect.top() + (h - checkBoxHeight) / 2;
                        totalRect.setRect(left, top, indicatorWidth, indicatorHeight);
                        // Adjust for label
                    }
                    else
                    {
                        left = ltr ? (totalRect.left() + checkBoxWidth - 2) : totalRect.left();
                        int top = totalRect.top() + (h - th) / 2;
                        totalRect.setRect(left, top, totalRect.width() - checkBoxWidth, th);
                    }
                }
                ret = totalRect;
                break;
            }
            default:
                break;
            }
        }
        break;
    }
#endif // QT_CONFIG(groupbox)
    default:
        ret = QProxyStyle::subControlRect(cc, opt, sc, widget);
        break;
    }
    return ret;
}
