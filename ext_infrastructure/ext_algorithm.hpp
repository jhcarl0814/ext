#ifndef EXT_ALGORITHM_H
#define EXT_ALGORITHM_H

#include <algorithm>
#include <vector>
#include <ranges>
#include <functional>


template<typename Container, typename Value_Find>
auto contains_and_find(Container &container, Value_Find &&value_find)
{
    auto it = container.find(std::forward<Value_Find>(value_find));
    return std::make_pair(it != std::ranges::cend(container), it);
}

template<typename Container, typename Value_Find>
auto push_back_if_did_not_find(Container &container, Value_Find &&value_find)
{
    if(auto it = std::ranges::find(container, value_find); it == std::ranges::cend(container))
    {
        container.push_back(std::forward<Value_Find>(value_find));
        return --(it = std::ranges::end(container));
    }
    else
        return it;
}

template<typename Container, typename Func_Find, typename Func_Generate>
auto push_back_if_did_not_find_if(Container &container, Func_Find &&func_find, Func_Generate &&func_generate)
{
    if(auto it = std::ranges::find_if(container, std::forward<Func_Find>(func_find)); it == std::ranges::cend(container))
    {
        container.push_back(std::forward<Func_Generate>(func_generate)());
        return --(it = std::ranges::end(container));
    }
    else
        return it;
}

template<typename Container, typename Func_Get_Nested_Container_From_Nested_Container_Owner, typename Func_Find, typename Func_Find_Nested_Container_Owner, typename Func_Generate_Nested_Container_Ower, typename Func_Generate>
auto push_back_if_did_not_find_if_nested(Container &container, Func_Get_Nested_Container_From_Nested_Container_Owner &&func_get_nested_container_from_nested_container_owner, Func_Find &&func_find, Func_Find_Nested_Container_Owner &&func_find_nested_container_owner, Func_Generate_Nested_Container_Ower &&func_generate_nested_container_owner, Func_Generate &&func_generate)
{
    for(auto it_nested_container_owner = std::ranges::begin(container); it_nested_container_owner != std::ranges::cend(container); ++it_nested_container_owner)
    {
        if(auto it_value = std::ranges::find_if(func_get_nested_container_from_nested_container_owner(*it_nested_container_owner), func_find); it_value != std::ranges::cend(func_get_nested_container_from_nested_container_owner(*it_nested_container_owner)))
            return std::make_pair(it_nested_container_owner, it_value);
    }
    auto it_nested_container_owner = push_back_if_did_not_find_if(container, func_find_nested_container_owner, func_generate_nested_container_owner);
    func_get_nested_container_from_nested_container_owner(*it_nested_container_owner).push_back(std::forward<Func_Generate>(func_generate)());
    return std::make_pair(it_nested_container_owner, --std::ranges::end(func_get_nested_container_from_nested_container_owner(*it_nested_container_owner)));
}

#endif // EXT_ALGORITHM_H
