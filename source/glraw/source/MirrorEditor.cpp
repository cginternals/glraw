
#include <glraw/MirrorEditor.h>

#include <QImage>


namespace glraw
{

MirrorEditor::MirrorEditor()
:   m_horizontal(false)
,   m_vertical(false)
{

}

MirrorEditor::MirrorEditor(bool horizontal, bool vertical)
:   m_horizontal(horizontal)
,   m_vertical(vertical)
{
}

MirrorEditor::~MirrorEditor()
{
}

void MirrorEditor::editImage(QImage & image, AssetInformation & /*info*/)
{
    image = image.mirrored(m_horizontal, m_vertical);
}

void MirrorEditor::setHorizontal(bool b)
{
    m_horizontal = b;
}

void MirrorEditor::setVertical(bool b)
{
    m_vertical = b;
}

} // namespace glraw
