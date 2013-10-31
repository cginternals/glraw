
#include <glraw/MirrorEditor.h>

#include <QImage>

namespace glraw
{

MirrorEditor::MirrorEditor(bool horizontal, bool vertical)
:   m_horizontal(horizontal)
,   m_vertical(vertical)
{
}

MirrorEditor::~MirrorEditor()
{
}

void MirrorEditor::editImage(QImage & image, AssetInformation & info)
{
    image = image.mirrored(m_horizontal, m_vertical);
}

} // namespace glraw
