/*****************************************************************************/
/**
 *  @file   ScreenBase.cpp
 *  @author Naohisa Sakamoto
 *  @edit   Taro Murase
 */
/*****************************************************************************/
#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>
#include <kvs/Message>
#include <iostream>
#include <string>
#include <cstdio>
#include <cfenv>


namespace
{

template <typename T>
inline void Flip( T* data, const size_t width, const size_t height, const size_t ncomps )
{
    // NOTE: In case that a none-zero value is specified for the environment variable
    // 'KVS_EGL_Y_FLIP', the data will be flipped.
    // e.g.) export KVS_EGL_Y_FLIP=1   (flipped) default
    //       export KVS_EGL_Y_FLIP=0   (not flipped)
    //
    bool y_flip = true;
    const char* KVS_EGL_Y_FLIP( std::getenv( "KVS_EGL_Y_FLIP" ) );
    if ( KVS_EGL_Y_FLIP != nullptr )
    {
        if ( std::atoi( KVS_EGL_Y_FLIP ) == 0 )
        {
            y_flip = false;
        }
    }

    if ( y_flip )
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

namespace egl
{

ScreenBase::ScreenBase():
    m_context( m_display ),
    m_config( m_display ),
    m_surface( m_display )
{
    if ( !m_display.create( EGL_DEFAULT_DISPLAY ) )
    {
        kvsMessageError( "Cannot create EGL display connection." );
        return;
    }
}

ScreenBase::~ScreenBase()
{
    m_display.terminate();
    m_context.destroy();
    m_surface.destroy();
}

kvs::ValueArray<kvs::UInt8> ScreenBase::readbackColorBuffer( GLenum mode ) const
{
    kvs::OpenGL::SetReadBuffer( mode );
    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, GLint(1) );

    const size_t width = this->width();
    const size_t height = this->height();
    kvs::ValueArray<kvs::UInt8> buffer( width * height * 4 );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data() );
    ::Flip( buffer.data(), width, height, 4 );

    return buffer;
}

kvs::ValueArray<kvs::Real32> ScreenBase::readbackDepthBuffer( GLenum mode ) const
{
    kvs::OpenGL::SetReadBuffer( mode );
    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, GLint(1) );

    const size_t width = this->width();
    const size_t height = this->height();
    kvs::ValueArray<kvs::Real32> buffer( width * height );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, buffer.data() );
    ::Flip( buffer.data(), width, height, 1 );

    return buffer;
}

void ScreenBase::displayInfo()
{
  /*
    EGLDisplay display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    std::cout << "EGL_CLIENT_APIS : ";
    std::cout << eglQueryString( display, EGL_CLIENT_APIS ) << std::endl;
    std::cout << "EGL_VENDOR : ";
    std::cout << eglQueryString( display, EGL_VENDOR ) << std::endl;
    std::cout << "EGL_VERSION : ";
    std::cout << eglQueryString( display, EGL_VERSION ) << std::endl;
    std::cout << "EGL_EXTENSIONS : ";
    std::cout << eglQueryString( display, EGL_EXTENSIONS ) << std::endl;
  */
}

void ScreenBase::create()
{
    // Initialize EGL display
    if ( !m_display.initialize() )
    {
        kvsMessageError( "Cannot initialize EGL display." );
        return;
    }

    // Create an appropriate EGL frame buffer configuration.
    const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE,  8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE,   8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };

    if ( !m_config.create( config_attribs ) )
    {
        kvsMessageError( "Cannot choose configuration." );
        return;
    }

    // Create a drawing surface.
    const EGLint surface_attribs[] = {
        EGL_WIDTH,  BaseClass::width(),
        EGL_HEIGHT, BaseClass::height(),
        EGL_NONE,
    };

    if ( !m_surface.create( m_config, surface_attribs ) )
    {
        kvsMessageError( "Cannot create drawing surface." );
        return;
    }

    // Create EGL context
    m_context.create( m_config );
    if ( !m_context.isValid() )
    {
        kvsMessageError( "Cannot create EGL context." );
        return;
    }

    // Bind the context to the buffer
    if ( !m_context.makeCurrent( m_surface ) )
    {
        kvsMessageError( "Cannot bind buffer." );
        return;
    }

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
    m_context.swapBuffers( m_surface );
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();
    kvs::ValueArray<kvs::UInt8> buffer( width * height * 3 );

    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::ReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );
    return kvs::ColorImage( width, height, buffer );
}

void ScreenBase::errorMessage( const char* msg )
{
    std::cerr << "Error : " << msg << std::endl;
    EGLint code = eglGetError();
    std::string err_msg;
    switch ( code )
    {
        case 0x3000 : err_msg = "EGL_SUCCESS"; break;
        case 0x3001 : err_msg = "EGL_NOT_INITIALIZED"; break;
        case 0x3002 : err_msg = "EGL_BAD_ACCESS"; break;
        case 0x3003 : err_msg = "EGL_BAD_ALLOC"; break;
        case 0x3004 : err_msg = "EGL_BAD_ATTRIBUTE"; break;
        case 0x3005 : err_msg = "EGL_BAD_CONFIG"; break;
        case 0x3006 : err_msg = "EGL_BAD_CONTEXT"; break;
        case 0x3007 : err_msg = "EGL_BAD_CURRENT_SURFACE"; break;
        case 0x3008 : err_msg = "EGL_BAD_DISPLAY"; break;
        case 0x3009 : err_msg = "EGL_BAD_MATCH"; break;
        case 0x300A : err_msg = "EGL_BAD_NATIVE_PIXMAP"; break;
        case 0x300B : err_msg = "EGL_BAD_NATIVE_WINDOW"; break;
        case 0x300C : err_msg = "EGL_BAD_PARAMETER"; break;
        case 0x300D : err_msg = "EGL_BAD_SURFACE"; break;
        default : err_msg = "UNKNOWN_ERROR_CODE"; break;
    }
    std::cerr << "Code  : " << err_msg << " (0x" << std::hex << code << ")" << std::endl;
    exit( 1 );
}

} // end of namespace egl

} // end of namespace kvs
