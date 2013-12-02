
#pragma once

#include <string>
#include <vector>


namespace glraw
{

template<typename T>
class RawFile
{
public:
    RawFile(const std::string & filePath);
    virtual ~RawFile();

    const T * data() const;
    const size_t size() const;

    bool valid() const;
    const std::string & filePath() const;

protected:
    bool read();

protected:
	const std::string m_filePath;
    std::vector<T> m_data;

    bool m_valid;
};

} // namespace glraw

#include "RawFile.hpp"
