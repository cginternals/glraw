
#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>

namespace glraw
{

template<typename T>
char GLRawFile<T>::s_magicNumber[4] = { 'c', '6', 'f', '5' };

template<typename T>
GLRawFile<T>::GLRawFile(const std::string & filePath)
:   m_filePath(filePath)
,   m_valid(false)
{
    if (read())
        m_valid = true;
}

template<typename T>
GLRawFile<T>::~GLRawFile()
{
}

template<typename T>
bool GLRawFile<T>::isValid() const
{
    return m_valid;
}

template<typename T>
const T * GLRawFile<T>::data() const
{
    return &m_data.data()[0];
}

template<typename T>
const size_t GLRawFile<T>::size() const
{
    return m_data.size();
}

template<typename T>
const std::string & GLRawFile<T>::stringProperty(const std::string & key) const
{
    m_stringProperties.at(key);
}

template<typename T>
int32_t GLRawFile<T>::intProperty(const std::string & key) const
{
    m_intProperties.at(key);
}

template<typename T>
double GLRawFile<T>::doubleProperty(const std::string & key) const
{
    m_doubleProperties.at(key);
}

template<typename T>
bool GLRawFile<T>::hasStringProperty(const std::string & key) const
{
    return m_stringProperties.find(key) == m_stringProperties.end();
}

template<typename T>
bool GLRawFile<T>::hasIntProperty(const std::string & key) const
{
    return m_intProperties.find(key) == m_intProperties.end();
}

template<typename T>
bool GLRawFile<T>::hasDoubleProperty(const std::string & key) const
{
    return m_doubleProperties.find(key) == m_doubleProperties.end();
}

template<typename T>
bool GLRawFile<T>::read()
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
    {
        std::cerr << "Reading from file \"" << m_filePath << "\" failed." << std::endl;
        return false;
    }
    
    uint64_t rawDataPosition;
    
    if (!readHeader(ifs, rawDataPosition))
        return false;

    readRawData(ifs, rawDataPosition);

    ifs.close();

    return true;
}

template<typename T>
bool GLRawFile<T>::readHeader(std::ifstream & ifs, uint64_t & rawDataPosition)
{
    char magicNumber[sizeof(s_magicNumber)];
    ifs.read(magicNumber, sizeof(s_magicNumber));
    if (strncmp(magicNumber, s_magicNumber, sizeof(s_magicNumber)) != 0)
    {
        std::cerr << "File \"" << m_filePath << "\" is not a glraw file." << std::endl;
        return false;
    }

    ifs.read(reinterpret_cast<char *>(&rawDataPosition), sizeof(rawDataPosition));
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

    return true;
}

template<typename T>
void GLRawFile<T>::readRawData(std::ifstream & ifs, uint64_t rawDataPosition)
{
    ifs.seekg(0, std::ios::end);
    
    size_t endPosition = ifs.tellg();
    const size_t size = endPosition - rawDataPosition;
    
    ifs.seekg(rawDataPosition, std::ios::beg);

    m_data.resize(size / sizeof(T));

    ifs.read(reinterpret_cast<char *>(&m_data[0]), size);
}

} // namespace glraw
