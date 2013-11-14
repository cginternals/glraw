#pragma once

#include <algorithm>
#include <fstream>

namespace glraw
{

template<typename T>
RawFile<T>::RawFile(const std::string & filePath)
:   m_filePath(filePath)
,   m_valid(false)
{
    if (read())
        m_valid = true;
}

template<typename T>
RawFile<T>::~RawFile()
{
}

template<typename T>
bool RawFile<T>::valid() const
{
    return m_valid;
}

template<typename T>
const T * RawFile<T>::data() const
{
    return &m_data.data()[0];
}

template<typename T>
const size_t RawFile<T>::size() const
{
    return m_data.size();
}

template<typename T>
bool RawFile<T>::read()
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!ifs)
    {
        std::cerr << "Reading from file \"" << m_filePath << "\" failed." << endl;
        return false;
    }

    const size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    m_data.resize(size / sizeof(T));

    ifs.read(reinterpret_cast<char*>(&m_data[0]), size);
    ifs.close();

    return true;
}

} // namespace glraw