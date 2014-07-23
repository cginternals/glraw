
#pragma once

#include <functional>

#include <Qt>

#include <glraw/glraw_api.h>

#include <glraw/ImageEditorInterface.h>

class QSize;

namespace glraw
{

class GLRAW_API ScaleEditor : public ImageEditorInterface
{
public:
    ScaleEditor();
    virtual ~ScaleEditor();

    virtual void editImage(QImage & image, AssetInformation & info);

    void setAspectRatioMode(Qt::AspectRatioMode mode);
    void setTransformationMode(Qt::TransformationMode mode);

    void setWidth(int width);
    void setHeight(int height);

    void setWidthScale(float scale);
    void setHeightScale(float scale);

    void setScale(float scale);
    
protected:
    QSize newSize(const QSize & size);

protected:
    Qt::AspectRatioMode m_aspectRatioMode;
    Qt::TransformationMode m_transformationMode;

    std::function<int(int)> m_widthScaling;
    std::function<int(int)> m_heightScaling;

};

} // namespace glraw
