#include "Surface.h"
#include "Context.h"
#include <kvs/Message>


namespace
{

inline size_t BytePerPixel( const GLint format )
{
    switch ( format )
    {
    case OSMESA_COLOR_INDEX:
        return 1;
    case OSMESA_RGBA:
    case OSMESA_BGRA:
    case OSMESA_ARGB:
        return 4;
    case OSMESA_RGB:
    case OSMESA_BGR:
        return 3;
    case OSMESA_RGB_565:
        return 2;
    default:
        break;
    }

    // Unknown pixel format.
    return 0;
}

} // end of namespace

namespace kvs
{

namespace osmesa
{

Surface::Surface():
    m_width( 0 ),
    m_height( 0 )
{
}

Surface::~Surface()
{
    this->destroy();
}

bool Surface::create( const size_t width, const size_t height, const int format )
{
    const size_t bpp = ::BytePerPixel( format );
    m_width = width;
    m_height = height;
    m_buffer.allocate( m_width * m_height * bpp );
    return !m_buffer.empty();
}

void Surface::destroy()
{
    m_buffer.release();
    m_width = 0;
    m_height = 0;
}

} // end of namespace osmesa

} // end of namespace kvs
