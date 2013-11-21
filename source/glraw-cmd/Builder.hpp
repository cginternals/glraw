
#pragma once

template <class Editor>
Editor * Builder::editor(const QString & key)
{
    return static_cast<Editor *>(m_editors.value(key, nullptr));
}
