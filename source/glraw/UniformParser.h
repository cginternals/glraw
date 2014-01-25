
#pragma once

#include <QMap>
#include <QString>

class QOpenGLFunctions_3_2_Core;
class QOpenGLShaderProgram;

namespace glraw
{

class UniformParser
{
public:
    static void setUniforms(
        QOpenGLFunctions_3_2_Core & gl
    ,   QOpenGLShaderProgram & program
    ,   const QMap<QString, QString> & uniforms);

    static void setFloat(
        QOpenGLShaderProgram & program
    ,   const QString & uniform
    ,   QString value);

    static void setInt(
        QOpenGLShaderProgram & program
    ,   const QString & uniform
    ,   QString value);

    static void setUInt(
        QOpenGLShaderProgram & program
        , const QString & uniform
        , QString value);

    static void setVec2(
        QOpenGLShaderProgram & program
    ,   const QString & uniform
    ,   QString value);

    static void setVec3(
        QOpenGLShaderProgram & program
    ,   const QString & uniform
    ,   QString value);

    static void setVec4(
        QOpenGLShaderProgram & program
    ,   const QString & uniform
    ,   QString value);

protected:
    static void typeMismatch(const QString & uniform);
    static void typeUnsupported(const QString & uniform);
    static void parsingFailed(const QString & uniform, const QString & value);
};

} // namespace glraw
