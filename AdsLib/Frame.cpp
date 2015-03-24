#include "Frame.h"
#include <algorithm>
#include <cstring>
#include <new>

Frame::Frame(size_t length, const void* data)
    : m_Data(new uint8_t[length])
{
    m_Size = m_Data ? length : 0;
    m_Pos = m_Data.get() + m_Size;

    if (m_Pos && data) {
        m_Pos -= length;
        memcpy(m_Pos, data, length);
    }
}

uint8_t Frame::operator[](size_t index) const
{
    return m_Pos[index];
}

size_t Frame::capacity() const
{
    return m_Size;
}

Frame& Frame::clear()
{
    return remove(size());
}

const uint8_t* Frame::data() const
{
    return m_Pos;
}

Frame& Frame::limit(size_t newSize)
{
	m_Size = std::min(m_Size, newSize);
	m_Pos = m_Data.get();
	return *this;
}

Frame& Frame::reset(size_t newSize)
{
	m_Size = newSize;
	m_Pos = m_Data.get() + m_Size;
	return *this;
}

Frame& Frame::prepend(const void *const data, const size_t size)
{
    const size_t bytesFree = m_Pos - m_Data.get();
    if (size > bytesFree) {
        auto newData = new uint8_t[size + m_Size];
        if (NULL == newData) {
			throw std::bad_alloc();
        }

        m_Pos = newData + bytesFree + size;
        memcpy(m_Pos, m_Data.get() + bytesFree, m_Size - bytesFree);
        m_Data.reset(newData);
        m_Size += size;
        m_Pos = m_Data.get() + bytesFree ;
    } else {
        m_Pos -= size;
    }
    memcpy(m_Pos, data, size);
    return *this;
}

uint8_t* Frame::rawData() const
{
    return m_Data.get();
}

Frame& Frame::remove(size_t numBytes)
{
    m_Pos = std::min<uint8_t*>(m_Pos + numBytes, m_Data.get() + m_Size);
    return *this;
}

size_t Frame::size() const
{
    return m_Size - (m_Pos - m_Data.get());
}
