
#include <ext_infrastructure/ext_json.hpp>

QJsonObject QJsonObject_extend(QJsonObject lhs, QJsonObject rhs)
{
    for(auto it = rhs.begin(); it != rhs.end(); ++it)
    {
        lhs.insert(it.key(), it.value());
    }
    return lhs;
}
