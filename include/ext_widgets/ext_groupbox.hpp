#ifndef EXT_GROUPBOX_H
#define EXT_GROUPBOX_H

#include <QGroupBox>

class GroupBox: public QGroupBox
{
    Q_OBJECT
  public:
    using QGroupBox::QGroupBox;
    virtual QSize minimumSizeHint() const override;
};

#endif // EXT_GROUPBOX_H
