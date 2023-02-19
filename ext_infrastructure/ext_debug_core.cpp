
#include "ext_debug.hpp"

#include <boost/container_hash/hash.hpp>

namespace std
{
    template<typename... T>
    struct hash<std::set<T...>>
    {
        std::size_t operator()(auto const &v) const noexcept
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };
    template<typename... T>
    struct hash<std::multiset<T...>>
    {
        std::size_t operator()(auto const &v) const noexcept
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };
    template<typename... T>
    struct hash<std::map<T...>>
    {
        std::size_t operator()(auto const &v) const noexcept
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };
    template<typename... T>
    struct hash<std::multimap<T...>>
    {
        std::size_t operator()(auto const &v) const noexcept
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };
} // namespace std

namespace ext_debug
{
    thread_local std::size_t current_indent_level = 0;
    onullstream cnull;
    wonullstream wcnull;

    void ext_debug_core_test(void)
    {
        if(true)
        {
            std::nullptr_t a1 = nullptr;
            int p21 = 0, *p22 = &p21, **a2 = &p22;
            std::unique_ptr<std::unique_ptr<int>> a3 = std::make_unique<std::unique_ptr<int>>(std::make_unique<int>(1));
            std::shared_ptr<std::shared_ptr<int>> a4 = std::make_shared<std::shared_ptr<int>>(std::make_shared<int>(1));
            int p51 = 0;
            std::reference_wrapper<int> p52 = p51;
            std::reference_wrapper<std::reference_wrapper<int>> a5 = p52;
            declare_ext_debug_guard((a1, p21, p22, a2, a3, a4, p51, p52, a5, std::move(a1), std::move(p21), std::move(p22), std::move(a2), std::move(a3), std::move(a4), std::move(p51), std::move(p52), std::move(a5)), cout_synced());
            declare_ext_debug_guard((a1, p21, p22, a2, a3, a4, p51, p52, a5, std::move(a1), std::move(p21), std::move(p22), std::move(a2), std::move(a3), std::move(a4), std::move(p51), std::move(p52), std::move(a5)), wcout_synced());
            declare_ext_debug_guard((a1, p21, p22, a2, a3, a4, p51, p52, a5, std::move(a1), std::move(p21), std::move(p22), std::move(a2), std::move(a3), std::move(a4), std::move(p51), std::move(p52), std::move(a5)), qDebug_compact());

            std::pair<int, std::pair<int, int>> b1 = {1, {2, 3}};
            std::tuple<int, int, std::tuple<int, int, int>> b2 = {1, 2, {3, 4, 5}};
            declare_ext_debug_guard((b1, b2, std::move(b1), std::move(b2)), cout_synced());
            declare_ext_debug_guard((b1, b2, std::move(b1), std::move(b2)), wcout_synced());
            declare_ext_debug_guard((b1, b2, std::move(b1), std::move(b2)), qDebug_compact());

            int c1[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
            std::span r1 = c1;
            std::initializer_list<std::initializer_list<int>> c2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
            std::span r2 = c2;
            std::array<std::array<int, 3>, 3> c3 = {{{{1, 2, 3}}, {{4, 5, 6}}, {{7, 8, 9}}}};
            std::span r3 = c3;
            declare_ext_debug_guard((c1, r1, c2, r2, c3, r3 /*, std::move(c1)*/, std::move(r1), std::move(c2), std::move(r2), std::move(c3), std::move(r3)), cout_synced());
            declare_ext_debug_guard((c1, r1, c2, r2, c3, r3 /*, std::move(c1)*/, std::move(r1), std::move(c2), std::move(r2), std::move(c3), std::move(r3)), wcout_synced());
            declare_ext_debug_guard((c1, r1, c2, r2, c3, r3 /*, std::move(c1)*/, std::move(r1), std::move(c2), std::move(r2), std::move(c3), std::move(r3)), qDebug_compact());

            std::vector<std::vector<int>> d1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
            std::span s1 = d1;
            std::deque<std::deque<int>> d2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
            declare_ext_debug_guard((d1, s1, d2, std::move(d1), std::move(s1), std::move(d2)), cout_synced());
            declare_ext_debug_guard((d1, s1, d2, std::move(d1), std::move(s1), std::move(d2)), wcout_synced());
            declare_ext_debug_guard((d1, s1, d2, std::move(d1), std::move(s1), std::move(d2)), qDebug_compact());

            std::set<std::set<int>> e1{
                {
                    1,
                    1,
                    2,
                },
                {
                    1,
                    1,
                    2,
                },
                {
                    3,
                    3,
                    4,
                },
            };
            std::multiset<std::multiset<int>> e2{
                {
                    1,
                    1,
                    2,
                },
                {
                    1,
                    1,
                    2,
                },
                {
                    3,
                    3,
                    4,
                },
            };
            std::unordered_set<std::set<int>> e3{
                {
                    1,
                    1,
                    2,
                },
                {
                    1,
                    1,
                    2,
                },
                {
                    3,
                    3,
                    4,
                },
            };
            std::unordered_multiset<std::multiset<int>> e4{
                {
                    1,
                    1,
                    2,
                },
                {
                    1,
                    1,
                    2,
                },
                {
                    3,
                    3,
                    4,
                },
            };
            std::map<std::map<int, int>, std::map<int, int>> e5{
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                },
            };
            std::multimap<std::multimap<int, int>, std::multimap<int, int>> e6{
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                },
            };
            std::unordered_map<std::map<int, int>, std::unordered_map<int, int>> e7{
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                },
            };
            std::unordered_multimap<std::multimap<int, int>, std::unordered_multimap<int, int>> e8{
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                    {
                        {1, 10},
                        {1, 11},
                        {2, 20},
                    },
                },
                {
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                    {
                        {3, 30},
                        {3, 31},
                        {4, 40},
                    },
                },
            };
            declare_ext_debug_guard((e1, e2, e3, e4, e5, e6, e7, e8, std::move(e1), std::move(e2), std::move(e3), std::move(e4), std::move(e5), std::move(e6), std::move(e7), std::move(e8)), cout_synced());
            declare_ext_debug_guard((e1, e2, e3, e4, e5, e6, e7, e8, std::move(e1), std::move(e2), std::move(e3), std::move(e4), std::move(e5), std::move(e6), std::move(e7), std::move(e8)), wcout_synced());
            declare_ext_debug_guard((e1, e2, e3, e4, e5, e6, e7, e8, std::move(e1), std::move(e2), std::move(e3), std::move(e4), std::move(e5), std::move(e6), std::move(e7), std::move(e8)), qDebug_compact());
        }
    }
} // namespace ext_debug
