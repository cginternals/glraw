
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

#include "GLRawFile.h"

namespace
{

template<typename T>
T read(std::ifstream & stream)
{
    T value;
    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

std::string readString(std::ifstream & stream)
{
    std::stringstream ss;
    char c;

    while (stream.good())
    {
        stream.get(c);
        if (c == '\0')
            break;

        ss << c;
    }

    return ss.str();
}

} // namespace


uint16_t GLRawFile::s_magicNumber = 0xC6F5;


GLRawFile::GLRawFile(const std::string & filePath, bool parseProperties)
: m_filePath(filePath)
, m_valid(false)
{
    m_valid = readFile(parseProperties);
}


GLRawFile::~GLRawFile()
{
}


bool GLRawFile::isValid() const
{
    return m_valid;
}


const char * GLRawFile::data() const
{
    return m_data.data();
}


const size_t GLRawFile::size() const
{
    return m_data.size();
}


const std::string & GLRawFile::stringProperty(const std::string & key) const
{
    return m_stringProperties.at(key);
}


int32_t GLRawFile::intProperty(const std::string & key) const
{
    return m_intProperties.at(key);
}


double GLRawFile::doubleProperty(const std::string & key) const
{
    return m_doubleProperties.at(key);
}


bool GLRawFile::hasStringProperty(const std::string & key) const
{
    return m_stringProperties.find(key) != m_stringProperties.end();
}


bool GLRawFile::hasIntProperty(const std::string & key) const
{
    return m_intProperties.find(key) != m_intProperties.end();
}


bool GLRawFile::hasDoubleProperty(const std::string & key) const
{
    return m_doubleProperties.find(key) != m_doubleProperties.end();
}

bool GLRawFile::readFile(bool parseProperties)
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
    {
        perror("Error");
        return false;
    }
    
    uint64_t offset = 0;

    if (read<uint16_t>(ifs) == s_magicNumber)
    {
        offset = read<uint64_t>(ifs);

        if (parseProperties)
        {
            readProperties(ifs, offset);
        }
    }
    else
    {
        ifs.seekg(0);
    }
    
    readRawData(ifs, offset);

    ifs.close();

    return true;
}

void GLRawFile::readProperties(std::ifstream & ifs, uint64_t offset)
{
    while (ifs.tellg() < static_cast<int64_t>(offset) && ifs.good())
    {
        uint8_t type = read<uint8_t>(ifs);

        std::string key = readString(ifs);

        switch (type)
        {
            case IntType:
                m_intProperties[key] = read<int32_t>(ifs);
                break;
            case DoubleType:
                m_doubleProperties[key] = read<double>(ifs);
                break;
            case StringType:
                m_stringProperties[key] = readString(ifs);
                break;
            default:
                return;
        }
    }
}

void GLRawFile::readRawData(std::ifstream & ifs, uint64_t rawDataOffset)
{
    ifs.seekg(0, std::ios::end);
    
    size_t endPosition = ifs.tellg();
    const size_t size = endPosition - rawDataOffset;
    
    ifs.seekg(rawDataOffset, std::ios::beg);

    m_data.resize(size);

    ifs.read(m_data.data(), size);
}
