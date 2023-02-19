#ifndef EXT_STYLE_H
#define EXT_STYLE_H

#include <QProxyStyle>

class MyProxyStyle: public QProxyStyle
{
  public:
    using QProxyStyle::QProxyStyle;

    virtual int styleHint(StyleHint hint, QStyleOption const *option = nullptr, QWidget const *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;
    virtual void drawComplexControl(ComplexControl cc, QStyleOptionComplex const *opt, QPainter *p, QWidget const *widget) const override;
    virtual QRect subControlRect(ComplexControl cc, QStyleOptionComplex const *opt, SubControl sc, QWidget const *widget) const override;
    virtual QSize sizeFromContents(QStyle::ContentsType type, QStyleOption const *option, QSize const &contentsSize, QWidget const *widget = nullptr) const override;
};

#endif // EXT_STYLE_H
