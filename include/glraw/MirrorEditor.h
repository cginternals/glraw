#pragma once

#include <glraw/glraw.h>
#include <glraw/ImageEditorInterface.h>

namespace glraw
{

class GLRAW_API MirrorEditor : public ImageEditorInterface
{
public:
    MirrorEditor(bool horizontal, bool vertical);
    virtual ~MirrorEditor();

    virtual void editImage(QImage & image, AssetInformation & info);

protected:
    bool m_horizontal;
    bool m_vertical;

};

} // namespace glraw
