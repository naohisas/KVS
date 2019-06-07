#pragma once
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace osmesa
{

class Surface
{
private:
    size_t m_width; ///< width of image buffer
    size_t m_height; ///< height of image buffer
    kvs::ValueArray<kvs::UInt8> m_buffer; ///< image buffer

public:
    Surface();
    ~Surface();

    void* handle() { return m_buffer.data(); }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    const kvs::ValueArray<kvs::UInt8>& buffer() const { return m_buffer; }

    bool create( const size_t width, const size_t height, const int format );
    void destroy();
};

} // end of namespace osmesa

} // end of namespace kvs
