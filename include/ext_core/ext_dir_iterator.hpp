#ifndef EXT_DIR_ITERATOR_H
#define EXT_DIR_ITERATOR_H

#include <QDirIterator>
#include <boost/stl_interfaces/iterator_interface.hpp>

struct ext_dir_iterator: boost::stl_interfaces::proxy_iterator_interface<ext_dir_iterator, std::input_iterator_tag, QFileInfo>
{
    bool default_or_value_initialized; //https://stackoverflow.com/questions/28832492/why-do-iterators-need-to-be-default-constructible //std::views::join
    std::unique_ptr<QDirIterator> dir_iterator;
    std::optional<QFileInfo> value;

  public:
    using iterator_category = std::input_iterator_tag;

    ext_dir_iterator();
    template<typename... Ts>
    ext_dir_iterator(Ts &&...vs)
        : default_or_value_initialized(false),
          dir_iterator(std::make_unique<QDirIterator>(std::forward<Ts>(vs)...))
    {
        if(dir_iterator->hasNext())
        {
            dir_iterator->next();
            value.emplace(dir_iterator->fileInfo());
        }
    }
    ext_dir_iterator(ext_dir_iterator &&other);
    ext_dir_iterator &operator=(ext_dir_iterator &&other);
    void swap(ext_dir_iterator &other);

    QFileInfo operator*() const;

    ext_dir_iterator &operator++();
    void operator++(int);

    bool operator==(std::default_sentinel_t const &) const;
};

#endif // EXT_DIR_ITERATOR_H
