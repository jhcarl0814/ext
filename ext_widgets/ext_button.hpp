#ifndef EXT_BUTTON_H
#define EXT_BUTTON_H

#include <QPushButton>

class PushButton: public QPushButton
{
    Q_OBJECT
  public:
    using QPushButton::QPushButton;
    virtual QSize sizeHint() const override;
};

#endif // EXT_BUTTON_H
