
#pragma once

#include <glraw/glraw.h>

#include <QVariant>

namespace glraw
{

/** @brief
 * Stores general asset information of type QString, int or double
 */
class GLRAW_API AssetInformation
{
public:
    AssetInformation();
    ~AssetInformation();

    bool propertyExists(const QString & key) const;
    QVariant property(const QString & key) const;
    void setProperty(const QString & key, const QVariant & value);

    const QVariantMap & properties() const;

protected:
    QVariantMap m_properties;

};

} // namespace glraw
