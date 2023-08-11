#include "ext_dir_iterator.hpp"

ext_dir_iterator::ext_dir_iterator()
    : default_or_value_initialized(true)
{}
ext_dir_iterator::ext_dir_iterator(ext_dir_iterator &&other)
    : default_or_value_initialized(std::exchange(other.default_or_value_initialized, true)),
      dir_iterator(std::move(other.dir_iterator)),
      value(std::move(other.value))
{
}
ext_dir_iterator &ext_dir_iterator::operator=(ext_dir_iterator &&other)
{
    default_or_value_initialized = std::exchange(other.default_or_value_initialized, true);
    dir_iterator = std::move(other.dir_iterator),
    value = std::move(other.value);
    return *this;
}
void ext_dir_iterator::swap(ext_dir_iterator &other)
{
    std::swap(default_or_value_initialized, other.default_or_value_initialized);
    dir_iterator.swap(other.dir_iterator);
    value.swap(other.value);
}

QFileInfo ext_dir_iterator::operator*() const
{
    assert(!default_or_value_initialized);
    return dir_iterator->fileInfo();
}

ext_dir_iterator &ext_dir_iterator::operator++()
{
    assert(!default_or_value_initialized);
    assert(*this != std::default_sentinel);
    value.reset();
    if(dir_iterator->hasNext())
    {
        dir_iterator->next();
        value.emplace(dir_iterator->fileInfo());
    }
    return *this;
}
void ext_dir_iterator::operator++(int) // std::input_or_output_iterator (only moveable, only i++), !LegacyIterator (copyable, *i++ referenceable) // https://stackoverflow.com/questions/11876128/c-outputiterator-post-increment-requirements // std::input_iterator, !LegacyInputIterator (equality_comparable)
{
    assert(!default_or_value_initialized);
    ++*this;
}

bool ext_dir_iterator::operator==(std::default_sentinel_t const &) const
{
    assert(!default_or_value_initialized);
    return !value.has_value() && !dir_iterator->hasNext();
}

namespace std
{
    namespace detail
    {
        BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(ext_dir_iterator, std::input_iterator);
        BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(ext_dir_iterator, std::input_iterator_tag, std::input_iterator_tag, QFileInfo, QFileInfo, boost::stl_interfaces::proxy_arrow_result<QFileInfo>, std::ptrdiff_t);
    } // namespace detail
} // namespace std
