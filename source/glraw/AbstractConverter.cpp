
#include <glraw/AbstractConverter.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace glraw
{

AbstractConverter::AbstractConverter()
:   m_fragmentShader("")
{
}

AbstractConverter::~AbstractConverter()
{
}

bool AbstractConverter::hasFragmentShader() const
{
    return !m_fragmentShader.isEmpty();
}

bool AbstractConverter::setFragmentShader(const QString & sourcePath)
{
    QFile file(sourcePath);
    
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Opening file" << sourcePath << "failed.";
        return false;
    }
    
    QTextStream stream(&file);
    
    m_fragmentShader = stream.readAll();
    
    return true;
}

} // namespace glraw
