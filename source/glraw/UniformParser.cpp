
#include "UniformParser.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_2_Core>


namespace glraw
{

void UniformParser::setUniforms(
    QOpenGLFunctions_3_2_Core & gl
,   QOpenGLShaderProgram & program
,   const QMap<QString, QString> & uniforms)
{
    if (uniforms.isEmpty())
        return;

    if (!program.isLinked())
        return;

    program.bind();

    GLint activeUniforms = 0;
    gl.glGetProgramiv(program.programId(), GL_ACTIVE_UNIFORMS, &activeUniforms);
    GLint activeUniformMaxLength = 0;
    gl.glGetProgramiv(program.programId(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeUniformMaxLength);

    QMap<QString, GLenum> uniformTypesByName;

    // gather all active uniform names and types
    for (int i = 0; i < activeUniforms; ++i)
    {
        GLenum uniformType = 0;
        GLchar * uniformName = new GLchar[activeUniformMaxLength];
        GLsizei uniformNameLength = -1;
        GLint  uniformSize = -1;

        gl.glGetActiveUniform(program.programId(), i, activeUniformMaxLength
            , &uniformNameLength, &uniformSize, &uniformType, uniformName);

        uniformTypesByName.insert(QString(uniformName), uniformType);

        delete[] uniformName;
    }


    for (const QString & uniform : uniforms.keys())
    {
        if (!uniformTypesByName.contains(uniform))
            continue;

        const GLenum type = uniformTypesByName[uniform];
        QString value = uniforms[uniform];

        switch (type)
        {
        case GL_FLOAT:
        case GL_DOUBLE:
            setFloat(program, uniform, value);
            break;
        case GL_FLOAT_VEC2:
        case GL_DOUBLE_VEC2:
            setVec2(program, uniform, value);
            break;
        case GL_FLOAT_VEC3:
        case GL_DOUBLE_VEC3:
            setVec3(program, uniform, value);
            break;
        case GL_FLOAT_VEC4:
        case GL_DOUBLE_VEC4:
            setVec4(program, uniform, value);
            break;
        case GL_INT:
            setInt(program, uniform, value);
            break;
        case GL_INT_VEC2:
        case GL_INT_VEC3:
        case GL_INT_VEC4:
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_INT_VEC2:
        case GL_UNSIGNED_INT_VEC3:
        case GL_UNSIGNED_INT_VEC4:
        case GL_BOOL:
        case GL_BOOL_VEC2:
        case GL_BOOL_VEC3:
        case GL_BOOL_VEC4:
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT4x3:
        case GL_DOUBLE_MAT2:
        case GL_DOUBLE_MAT3:
        case GL_DOUBLE_MAT4:
        case GL_DOUBLE_MAT2x3:
        case GL_DOUBLE_MAT2x4:
        case GL_DOUBLE_MAT3x2:
        case GL_DOUBLE_MAT3x4:
        case GL_DOUBLE_MAT4x2:
        case GL_DOUBLE_MAT4x3:
        default:
             typeUnsupported(uniform);
             break;
//        case GL_SAMPLER_1D:
//        case GL_SAMPLER_2D:
//        case GL_SAMPLER_3D:
//        case GL_SAMPLER_CUBE:
//        case GL_SAMPLER_1D_SHADOW:
//        case GL_SAMPLER_2D_SHADOW:
//        case GL_SAMPLER_1D_ARRAY:
//        case GL_SAMPLER_2D_ARRAY:
//        case GL_SAMPLER_1D_ARRAY_SHADOW:
//        case GL_SAMPLER_2D_ARRAY_SHADOW:
//        case GL_SAMPLER_2D_MULTISAMPLE:
//        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_SAMPLER_CUBE_SHADOW:
//        case GL_SAMPLER_BUFFER:
//        case GL_SAMPLER_2D_RECT:
//        case GL_SAMPLER_2D_RECT_SHADOW:
//        case GL_INT_SAMPLER_1D:
//        case GL_INT_SAMPLER_2D:
//        case GL_INT_SAMPLER_3D:
//        case GL_INT_SAMPLER_CUBE:
//        case GL_INT_SAMPLER_1D_ARRAY:
//        case GL_INT_SAMPLER_2D_ARRAY:
//        case GL_INT_SAMPLER_2D_MULTISAMPLE:
//        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_INT_SAMPLER_BUFFER:
//        case GL_INT_SAMPLER_2D_RECT:
//        case GL_UNSIGNED_INT_SAMPLER_1D:
//        case GL_UNSIGNED_INT_SAMPLER_2D:
//        case GL_UNSIGNED_INT_SAMPLER_3D:
//        case GL_UNSIGNED_INT_SAMPLER_CUBE:
//        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
//        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
//        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
//        case GL_IMAGE_1D:
//        case GL_IMAGE_2D:
//        case GL_IMAGE_3D:
//        case GL_IMAGE_2D_RECT:
//        case GL_IMAGE_CUBE:
//        case GL_IMAGE_BUFFER:
//        case GL_IMAGE_1D_ARRAY:
//        case GL_IMAGE_2D_ARRAY:
//        case GL_IMAGE_2D_MULTISAMPLE:
//        case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
//        case GL_INT_IMAGE_1D:
//        case GL_INT_IMAGE_2D:
//        case GL_INT_IMAGE_3D:
//        case GL_INT_IMAGE_2D_RECT:
//        case GL_INT_IMAGE_CUBE:
//        case GL_INT_IMAGE_BUFFER:
//        case GL_INT_IMAGE_1D_ARRAY:
//        case GL_INT_IMAGE_2D_ARRAY:
//        case GL_INT_IMAGE_2D_MULTISAMPLE:
//        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
//        case GL_UNSIGNED_INT_IMAGE_1D:
//        case GL_UNSIGNED_INT_IMAGE_2D:
//        case GL_UNSIGNED_INT_IMAGE_3D:
//        case GL_UNSIGNED_INT_IMAGE_2D_RECT:
//        case GL_UNSIGNED_INT_IMAGE_CUBE:
//        case GL_UNSIGNED_INT_IMAGE_BUFFER:
//        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
//        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
//        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
//        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
//        case GL_UNSIGNED_INT_ATOMIC_COUNTER:
//            setInt(program, uniform, value);
//            break;
        }
    }
}


void UniformParser::setFloat(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    bool ok;
    float v = value.toFloat(&ok);

    if (ok)
        program.setUniformValue(uniform.toStdString().c_str(), v);
    else
        parsingFailed(uniform, value);
}

void UniformParser::setInt(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    bool ok;
    int v = value.toInt(&ok);
    if (!ok)
        v = static_cast<int>(value.toFloat(&ok));
    
    if (ok)
        program.setUniformValue(uniform.toStdString().c_str(), v);
    else
        parsingFailed(uniform, value);
}

void UniformParser::setUInt(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    bool ok;
    int v = value.toUInt(&ok);
    if (!ok)
        v = static_cast<unsigned int>(value.toFloat(&ok));

    if (ok)
        program.setUniformValue(uniform.toStdString().c_str(), v);
    else
        parsingFailed(uniform, value);
}

void UniformParser::setVec2(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    if (!value.startsWith("dvec2") && !value.startsWith("vec2"))
    {
        typeMismatch(uniform);
        return;
    }

    value.remove("dvec2(");
    value.remove("vec2(");
    value.remove(")");

    const QStringList values = value.split(",");
    if (values.size() == 2)
    {
        bool ok[2];
        const QVector2D vec2(
            values[0].toFloat(&ok[0])
            , values[1].toFloat(&ok[1]));
        if (ok[0] && ok[1])
        {
            program.setUniformValue(uniform.toStdString().c_str(), vec2);
            return;
        }
    }
    parsingFailed(uniform, value);
}

void UniformParser::setVec3(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    if (!value.startsWith("dvec3") && !value.startsWith("vec3"))
    {
        typeMismatch(uniform);
        return;
    }

    value.remove("dvec3(");
    value.remove("vec3(");
    value.remove(")");

    const QStringList values = value.split(",");
    if (values.size() == 3)
    {
        bool ok[3];
        const QVector3D vec3(
            values[0].toFloat(&ok[0])
            , values[1].toFloat(&ok[1])
            , values[2].toFloat(&ok[2]));
        if (ok[0] && ok[1] && ok[2])
        {
            program.setUniformValue(uniform.toStdString().c_str(), vec3);
            return;
        }
    }
    parsingFailed(uniform, value);
}

void UniformParser::setVec4(
    QOpenGLShaderProgram & program
,   const QString & uniform
,   QString value)
{
    if (!value.startsWith("dvec4") && !value.startsWith("vec4"))
    {
        typeMismatch(uniform);
        return;
    }

    value.remove("dvec4(");
    value.remove("vec4(");
    value.remove(")");

    const QStringList values = value.split(",");
    if (values.size() == 4)
    {
        bool ok[4];
        const QVector4D vec4(
            values[0].toFloat(&ok[0])
          , values[1].toFloat(&ok[1])
          , values[2].toFloat(&ok[2])
          , values[3].toFloat(&ok[3]));
        if (ok[0] && ok[1] && ok[2] && ok[3])
        {
            program.setUniformValue(uniform.toStdString().c_str(), vec4);
            return;
        }
    }
    parsingFailed(uniform, value);
}

void UniformParser::typeMismatch(const QString & uniform)
{
    qDebug() << "Uniform value-type missmatch for" << uniform << ".";
}

void UniformParser::typeUnsupported(const QString & uniform)
{
    qDebug() << "Uniform value-type" << uniform << " is not supported.";
}

void UniformParser::parsingFailed(const QString & uniform, const QString & value)
{
    qDebug() << "Parsing uniform value" << value << "for" << uniform << "failed.";
}

} // namespace glraw
