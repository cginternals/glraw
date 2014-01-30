
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

class GLRawFile
{
public:
    static uint16_t s_magicNumber;

    enum PropertyType {
        Unknown = 0,
        IntType = 1,
        DoubleType = 2,
        StringType = 3
    };

    GLRawFile(const std::string & filePath, bool parseProperties = true);
    virtual ~GLRawFile();

    const char * data() const;
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
    bool readFile(bool parseProperties);
    
    void readProperties(std::ifstream & ifs, uint64_t offset);
    void readStringProperties(std::ifstream & ifs);
    void readIntProperties(std::ifstream & ifs);
    void readDoubleProperties(std::ifstream & ifs);
    
    void readRawData(std::ifstream & ifs, uint64_t offset);

protected:
    const std::string m_filePath;
    std::vector<char> m_data;

    std::map<std::string, std::string> m_stringProperties;
    std::map<std::string, int32_t> m_intProperties;
    std::map<std::string, double> m_doubleProperties;

    bool m_valid;

};

