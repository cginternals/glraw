
#include <glraw/ScaleEditor.h>

#include <QSize>
#include <QImage>

#include <glraw/AssetInformation.h>

namespace glraw
{

ScaleEditor::ScaleEditor()
:   m_aspectRatioMode(Qt::IgnoreAspectRatio)
,   m_transformationMode(Qt::FastTransformation)
,   m_widthScaling([](int w) { return w; })
,   m_heightScaling([](int h) { return h; })
{
}

ScaleEditor::~ScaleEditor()
{
}

void ScaleEditor::editImage(QImage & image, AssetInformation & info)
{
    QSize size = this->newSize(image.size());
    info.setProperty("width", size.width());
    info.setProperty("height", size.height());

    image = image.scaled(size, m_aspectRatioMode, m_transformationMode);
}

void ScaleEditor::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    m_aspectRatioMode = mode;
}

void ScaleEditor::setTransformationMode(Qt::TransformationMode mode)
{
    m_transformationMode = mode;
}

void ScaleEditor::setWidth(int width)
{
    m_widthScaling = [width](int /*width*/) {
        return width;
    };
}

void ScaleEditor::setHeight(int height)
{
    m_heightScaling = [height](int /*height*/) {
        return height;
    };
}

void ScaleEditor::setWidthScale(float scale)
{
    m_widthScaling = [scale](int width) {
        return scale * width;
    };
}

void ScaleEditor::setHeightScale(float scale)
{
    m_widthScaling = [scale](int height) {
        return scale * height;
    };
}

void ScaleEditor::setScale(float scale)  
{
    m_widthScaling = [scale](int width) {
        return scale * width;
    };
    m_heightScaling = [scale](int height) {
        return scale * height;
    };
}

QSize ScaleEditor::newSize(const QSize & size)
{
    return QSize(
        m_widthScaling(size.width()), 
        m_heightScaling(size.height())
    );
}

} // namespace glraw
