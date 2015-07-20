#pragma once

#include <QVariant>

#include <glraw/glraw_api.h>


namespace glraw
{

/** @brief
 * Stores general asset information of type QString, int or double
 */
class GLRAW_API AssetInformation
{
public:
    AssetInformation() = default;
	~AssetInformation() = default;

    bool propertyExists(const QString & key) const;
    QVariant property(const QString & key) const;
    void setProperty(const QString & key, const QVariant & value);

    const QVariantMap & properties() const;

protected:
    QVariantMap m_properties;

};

} // namespace glraw
