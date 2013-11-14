
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace glraw
{

template<typename T>
class GLRawFile
{
public:
    static char s_magicNumber[4];

    GLRawFile(const std::string & filePath, bool readProperties = true);
    virtual ~GLRawFile();

    const T * data() const;
    const size_t size() const;

    bool isValid() const;
    const std::string & filePath() const;
    
    const std::string & stringProperty(const std::string & key) const;
    int32_t intProperty(const std::string & key) const;
    double doubleProperty(const std::string & key) const;
    
    bool hasStringProperty(const std::string & key) const;
    bool hasIntProperty(const std::string & key) const;
    bool hasDoubleProperty(const std::string & key) const;

protected:
    bool read(bool readProperties);
    
    bool checkMagicNumber(std::ifstream & ifs);
    void readRawDataOffset(std::ifstream & ifs, uint64_t & rawDataOffset);

    void readStringProperties(std::ifstream & ifs);
    void readIntProperties(std::ifstream & ifs);
    void readDoubleProperties(std::ifstream & ifs);
    
    void readRawData(std::ifstream & ifs, uint64_t rawDataOffset);

protected:
    const std::string m_filePath;
    std::vector<T> m_data;

    std::map<std::string, std::string> m_stringProperties;
    std::map<std::string, int32_t> m_intProperties;
    std::map<std::string, double> m_doubleProperties;

    bool m_valid;

};

} // namespace glraw

#include "GLRawFile.hpp"
