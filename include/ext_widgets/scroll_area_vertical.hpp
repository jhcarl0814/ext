#ifndef SCROLL_AREA_VERTICAL_H
#define SCROLL_AREA_VERTICAL_H

#include <QScrollArea>

#include <ext_infrastructure/ext_type_traits.hpp>

class ScrollAreaVertical: public QScrollArea //https://forum.qt.io/topic/13374/solved-qscrollarea-vertical-scroll-only
{
    Q_OBJECT
  public:
    template<typename Tuple, std::size_t... Is>
    ScrollAreaVertical(QWidget *widget, Tuple &&base_args, std::index_sequence<Is...>)
        : QScrollArea(std::get<Is>(std::forward<Tuple>(base_args))...)
    {
        ctor_impl(widget);
    }
    template<typename Tuple = std::tuple<>>
    ScrollAreaVertical(QWidget *widget, Tuple &&base_args = {})
        : ScrollAreaVertical(widget, std::forward<Tuple>(base_args), std::make_index_sequence<std::tuple_size_v<remove_lvalue_reference_t<Tuple>>>())
    {}

  private:
    void ctor_impl(QWidget *widget);

  protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;
};

#endif // SCROLL_AREA_VERTICAL_H
