
#include <ext_infrastructure/ext_debug.hpp>

namespace ext_debug
{
    void ext_debug_test(void)
    {
        {
            declare_ext_debug_guard((), cout_synced());
            declare_ext_debug_guard((), wcout_synced());
            declare_ext_debug_guard((), qDebug_compact());
            ext_debug_log((), cout_synced());
            ext_debug_log((), wcout_synced());
            ext_debug_log((), qDebug_compact());
            ext_debug_log((1), cout_synced());
            ext_debug_log((1), wcout_synced());
            ext_debug_log((1), qDebug_compact());
            declare_ext_debug_guard((1), cout_synced());
            declare_ext_debug_guard((1), wcout_synced());
            declare_ext_debug_guard((1), qDebug_compact());
        }
        ext_debug_core_test();
        ext_debug_overrides_test();
    }
} // namespace ext_debug
