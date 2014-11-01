
#include <glraw/RawFile.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>


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

}

namespace glraw
{


uint16_t RawFile::s_signature = 0xC6F5;


RawFile::RawFile(const std::string & filePath, bool parseProperties)
: m_filePath(filePath)
, m_valid(false)
{
    m_valid = readFile(parseProperties);
}


RawFile::~RawFile()
{
}


bool RawFile::isValid() const
{
    return m_valid;
}


const char * RawFile::data() const
{
    return m_data.data();
}


const size_t RawFile::size() const
{
    return m_data.size();
}


const std::string & RawFile::stringProperty(const std::string & key) const
{
    return m_stringProperties.at(key);
}


int32_t RawFile::intProperty(const std::string & key) const
{
    return m_intProperties.at(key);
}


double RawFile::doubleProperty(const std::string & key) const
{
    return m_doubleProperties.at(key);
}


bool RawFile::hasStringProperty(const std::string & key) const
{
    return m_stringProperties.find(key) != m_stringProperties.end();
}


bool RawFile::hasIntProperty(const std::string & key) const
{
    return m_intProperties.find(key) != m_intProperties.end();
}


bool RawFile::hasDoubleProperty(const std::string & key) const
{
    return m_doubleProperties.find(key) != m_doubleProperties.end();
}

bool RawFile::readFile(bool parseProperties)
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
    {
        perror("Error");
        return false;
    }
    
    uint64_t offset = 0;

    if (read<uint16_t>(ifs) == s_signature)
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

void RawFile::readProperties(std::ifstream & ifs, uint64_t offset)
{
    while (ifs.tellg() < static_cast<int64_t>(offset) && ifs.good())
    {
        uint8_t type = read<uint8_t>(ifs);

        std::string key = readString(ifs);

		switch (static_cast<PropertyType>(type))
        {
		case PropertyType::Int:
            m_intProperties[key] = read<int32_t>(ifs);
            break;

		case PropertyType::Double:
            m_doubleProperties[key] = read<double>(ifs);
            break;

		case PropertyType::String:
            m_stringProperties[key] = readString(ifs);
            break;

        default:
            return;
        }
    }
}

void RawFile::readRawData(std::ifstream & ifs, uint64_t rawDataOffset)
{
    ifs.seekg(0, std::ios::end);
    
    size_t endPosition = ifs.tellg();
    const size_t size = endPosition - rawDataOffset;
    
    ifs.seekg(rawDataOffset, std::ios::beg);

    m_data.resize(size);

    ifs.read(m_data.data(), size);
}

} // namespace glraw
