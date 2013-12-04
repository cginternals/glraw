
#include <algorithm>
#include <fstream>
#include <iostream>

#include <glraw/GLRawFile.h>

namespace glraw
{


uint16_t GLRawFile::s_magicNumber = 0xC6F5;


GLRawFile::GLRawFile(const std::string & filePath, bool parseProperties)
: m_filePath(filePath)
, m_valid(false)
{
    m_valid = read(parseProperties);
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
    return &m_data.data()[0];
}


const size_t GLRawFile::size() const
{
    return m_data.size();
}


const std::string & GLRawFile::stringProperty(const std::string & key) const
{
    m_stringProperties.at(key);
}


int32_t GLRawFile::intProperty(const std::string & key) const
{
    m_intProperties.at(key);
}


double GLRawFile::doubleProperty(const std::string & key) const
{
    m_doubleProperties.at(key);
}


bool GLRawFile::hasStringProperty(const std::string & key) const
{
    return m_stringProperties.find(key) == m_stringProperties.end();
}


bool GLRawFile::hasIntProperty(const std::string & key) const
{
    return m_intProperties.find(key) == m_intProperties.end();
}


bool GLRawFile::hasDoubleProperty(const std::string & key) const
{
    return m_doubleProperties.find(key) == m_doubleProperties.end();
}


bool GLRawFile::read(bool parseProperties)
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
    {
        std::cerr << "Reading from file \"" << m_filePath << "\" failed." << std::endl;
        return false;
    }
    
    uint64_t offset = 0;

    if (readUint16(ifs) == s_magicNumber)
    {
        offset = readUint64(ifs);

        if (parseProperties)
        {
            /*readStringProperties(ifs);
            readIntProperties(ifs);
            readDoubleProperties(ifs);*/
        }
    }
    else
    {
        ifs.seekg(0, std::ios::beg);
    }
    
    readRawData(ifs, offset);

    ifs.close();

    return true;
}

void GLRawFile::readProperties(std::ifstream & ifs)
{

}

void GLRawFile::readStringProperties(std::ifstream & ifs)
{
    uint32_t stringCount;
    ifs.read(reinterpret_cast<char *>(&stringCount), sizeof(stringCount));

    for (uint32_t i = 0; i < stringCount; i++)
    {
        uint32_t keySize;
        ifs.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));
        char * key = new char[keySize];
        ifs.read(key, keySize);
        
        uint32_t valueSize;
        ifs.read(reinterpret_cast<char *>(&valueSize), sizeof(valueSize));
        char * value = new char[valueSize];
        ifs.read(value, valueSize);
        
        std::pair<std::string, std::string> pair(
            std::string(key, keySize),
            std::string(value, valueSize)
        );
        
        delete[] key;
        delete[] value;
        
        m_stringProperties.insert(pair);
    }
}


void GLRawFile::readIntProperties(std::ifstream & ifs)
{
    uint32_t intCount;
    ifs.read(reinterpret_cast<char *>(&intCount), sizeof(intCount));

    for (uint32_t i = 0; i < intCount; i++)
    {
        uint32_t keySize;
        ifs.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));
        char * key = new char[keySize];
        ifs.read(key, keySize);
        
        int32_t value;
        ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
        
        std::pair<std::string, int32_t> pair(
            std::string(key, keySize),
            value
        );
        
        delete[] key;
        
        m_intProperties.insert(pair);
    }
}


void GLRawFile::readDoubleProperties(std::ifstream & ifs)
{
    uint32_t doubleCount;
    ifs.read(reinterpret_cast<char *>(&doubleCount), sizeof(doubleCount));

    for (uint32_t i = 0; i < doubleCount; i++)
    {
        uint32_t keySize;
        ifs.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));
        char * key = new char[keySize];
        ifs.read(key, keySize);
        
        double value;
        ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
        
        std::pair<std::string, double> pair(
            std::string(key, keySize),
            value
        );
        
        delete[] key;
        
        m_intProperties.insert(pair);
    }
}

uint8_t GLRawFile::readUint16(std::ifstream & ifs)
{
    uint8_t value;
    ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

uint16_t GLRawFile::readUint16(std::ifstream & ifs)
{
    uint16_t value;
    ifs.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

uint64_t GLRawFile::readUint64(std::ifstream & ifs)
{
    uint64_t value;
    ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
    return value;
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

} // namespace glraw
