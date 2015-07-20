
#include <glraw/AssetInformation.h>

#include <assert.h>


namespace glraw
{
    
bool AssetInformation::propertyExists(const QString & key) const
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

} // namespace glraw
