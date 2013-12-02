
#include <glraw/AssetInformation.h>

#include <assert.h>

namespace glraw
{
    
AssetInformation::AssetInformation()
{
}

AssetInformation::~AssetInformation()
{
}

bool AssetInformation::propertyExist(const QString & key) const
{
    return m_properties.contains(key);
}

QVariant AssetInformation::property(const QString & key) const
{
    return m_properties.value(key, QVariant());
}

void AssetInformation::setProperty(const QString & key, const QVariant & value)
{
    assert(value.type() == QVariant::String ||
           value.type() == QVariant::Int ||
           value.type() == QVariant::Double);
    
    m_properties.insert(key, value);
}

const QVariantMap & AssetInformation::properties() const
{
    return m_properties;
}

QVariantMap AssetInformation::properties(QVariant::Type type) const
{
    QVariantMap map;
    for (auto it = m_properties.begin(); it != m_properties.end(); it++)
        if (it.value().type() == type)
            map.insert(it.key(), it.value());
    return map;
}

QVariantMap AssetInformation::stringProperties() const
{
    return this->properties(QVariant::String);
}

QVariantMap AssetInformation::intProperties() const
{
    return this->properties(QVariant::Int);
}

QVariantMap AssetInformation::doubleProperties() const
{
    return this->properties(QVariant::Double);
}

} // namespace glraw
