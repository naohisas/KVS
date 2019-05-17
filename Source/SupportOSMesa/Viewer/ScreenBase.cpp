#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>
#include <cstdio>


namespace
{

template <typename T>
inline void Flip( T* data, const size_t width, const size_t height, const size_t ncomps )
{
    // NOTE: Gallium softpipe driver doesn't support "upside-down" rendering
    // which would be needed for the OSMESA_Y_UP=TRUE case. Therefore, the
    // rendering pixel data read backed from GPU with glReadPixels need to be
    // flipped. In the current implementation, it is necessary to specify the
    // gallium driver "softpipe" or "llvmpipe" by environment parameter
    // 'KVS_OSMESA_GALLIUM_DRIVER'.
    // e.g.) export KVS_OSMESA_GALLIUM_DRIVER=softpipe
    //
    const char* driver( std::getenv( "KVS_OSMESA_GALLIUM_DRIVER" ) );
    if ( !driver ) return;

    const bool y_down = ( kvs::osmesa::Context::GetYAxisDirection() == 0 );
    if ( std::string( driver ) == "softpipe" && y_down )
    {
        const size_t stride = width * ncomps;

        T* pdata = data;
        const size_t end_line = height / 2;
        for ( size_t i = 0; i < end_line; i++ )
        {
            T* src = pdata + ( i * stride );
            T* dst = pdata + ( ( height - i - 1 ) * stride );
            for ( size_t j = 0; j < stride; j++ )
            {
                std::swap( *src, *dst );
                src++; dst++;
            }
        }
    }
}

}


namespace kvs
{

namespace osmesa
{

ScreenBase::ScreenBase()
{
}

ScreenBase::~ScreenBase()
{
}

kvs::ValueArray<kvs::UInt8> ScreenBase::readbackColorBuffer() const
{
    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, GLint(1) );

    const size_t width = this->width();
    const size_t height = this->height();
    kvs::ValueArray<kvs::UInt8> buffer( width * height * 4 );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data() );
   ::Flip( buffer.data(), width, height, 4 );

    return buffer;
}

kvs::ValueArray<kvs::Real32> ScreenBase::readbackDepthBuffer() const
{
    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, GLint(1) );

    const size_t width = this->width();
    const size_t height = this->height();
    kvs::ValueArray<kvs::Real32> buffer( width * height );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, buffer.data() );
    ::Flip( buffer.data(), width, height, 1 );

    return buffer;
}

void ScreenBase::create()
{
    // Create OSMesa context
    const GLenum format = OSMESA_RGBA;
    const GLint depth_bits = 24;
    const GLint stencil_bits = 0;
    const GLint accum_bits = 0;
    m_context.create( format, depth_bits, stencil_bits, accum_bits );
    if ( !m_context.isValid() )
    {
        kvsMessageError( "Cannot create OSMesa context." );
        return;
    }

    // Create OSMesa drawing surface
    const GLsizei width = GLsizei( BaseClass::width() );
    const GLsizei height = GLsizei( BaseClass::height() );
    m_surface.create( width, height, format );

    // Bind the context to the surface
    if ( !m_context.makeCurrent( m_surface ) )
    {
        kvsMessageError( "Cannot bind buffer." );
        return;
    }

    // Y coordinates increase downward
    kvs::osmesa::Context::SetYAxisDirectionToDown();

    this->initializeEvent();
}

void ScreenBase::show()
{
    if ( !m_context.isValid() ) { this->create(); }
}

void ScreenBase::redraw()
{
    this->draw();
}

void ScreenBase::draw()
{
    if ( !m_context.isValid() ) { this->create(); }
    this->paintEvent();
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();

    // RGBA to RGB
    const kvs::ValueArray<kvs::UInt8>& buffer = m_surface.buffer();
    kvs::ValueArray<kvs::UInt8> pixels( width * height * 3 );
    for ( size_t i = 0; i < width * height; i++ )
    {
        pixels[ 3 * i + 0 ] = buffer[ 4 * i + 0 ];
        pixels[ 3 * i + 1 ] = buffer[ 4 * i + 1 ];
        pixels[ 3 * i + 2 ] = buffer[ 4 * i + 2 ];
    }

    return kvs::ColorImage( width, height, pixels );
}

} // end of namespace osmesa

} // end of namespace kvs
