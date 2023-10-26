#ifndef EXT_WEBENGINE_H
#define EXT_WEBENGINE_H

#include <QWebEngineView>

#include <ext_infrastructure/ext_type_traits.hpp>

class WebEngineView: public QWebEngineView
{
    Q_OBJECT
  public:
    template<typename Tuple, std::size_t... Is>
    WebEngineView(Tuple &&base_args, std::index_sequence<Is...>)
        : QWebEngineView(std::get<Is>(std::forward<Tuple>(base_args))...)
    {
        ctor_impl();
    }
    template<typename Tuple = std::tuple<>>
    WebEngineView(Tuple &&base_args = {})
        : WebEngineView(std::forward<Tuple>(base_args), std::make_index_sequence<std::tuple_size_v<remove_lvalue_reference_t<Tuple>>>())
    {}

  protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;

  private:
    void ctor_impl();
};

#endif // EXT_WEBENGINE_H
