
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

unsigned int AbstractConverter::numUniforms() const
{
    return m_uniforms.size();
}

bool AbstractConverter::setUniform(const QString & assignment)
{
    QStringList terms = assignment.split("=");

    // there should be "=" and exactly two non-empty terms
    if (2 != terms.size() || terms[0].isEmpty() || terms[1].isEmpty())
        return false;

    // if identifier was alread defined, skip redifinitions
    if (m_uniforms.contains(terms[0]))
        return false;

    m_uniforms.insert(terms[0], terms[1]);

    return true;
}

} // namespace glraw
