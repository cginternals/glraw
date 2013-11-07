
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

    bool propertyExist(const QString & key) const;
    QVariant property(const QString & key) const;
    void setProperty(const QString & key, const QVariant & value);

    const QVariantMap & properties() const;
    QVariantMap stringProperties() const;
    QVariantMap intProperties() const;
    QVariantMap doubleProperties() const;

protected:
    QVariantMap properties(QVariant::Type type) const;
    QVariantMap m_properties;

};

} // namespace glraw
