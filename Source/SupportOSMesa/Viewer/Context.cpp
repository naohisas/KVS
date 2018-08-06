#include "Context.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace osmesa
{

void Context::SetPixelStore( GLint pname, GLint value )
{
    OSMesaPixelStore( pname, value );
}

void Context::SetRowLength( GLint value )
{
    Context::SetPixelStore( OSMESA_ROW_LENGTH, value );
}

void Context::SetYAxisDirection( GLint value )
{
    // value: zero = Y coordinates increase downward,
    //        non-zero = Y coordinates increase upward
    Context::SetPixelStore( OSMESA_Y_UP, value );
}

void Context::SetYAxisDirectionToDown()
{
    Context::SetYAxisDirection( 0 );
}

void Context::SetYAxisDirectionToUp()
{
    Context::SetYAxisDirection( 1 );
}

void Context::GetIntegerv( GLint pname, GLint* value )
{
    OSMesaGetIntegerv( pname, value );
}

GLint Context::GetWidth()
{
    GLint value = 0;
    Context::GetIntegerv( OSMESA_WIDTH, &value );
    return value;
}

GLint Context::GetHeight()
{
    GLint value = 0;
    Context::GetIntegerv( OSMESA_HEIGHT, &value );
    return value;
}

GLint Context::GetFormat()
{
    GLint value = 0;
    Context::GetIntegerv( OSMESA_FORMAT, &value );
    return value;
}

GLint Context::GetType()
{
    GLint value = 0;
    Context::GetIntegerv( OSMESA_TYPE, &value );
    return value;
}

GLint Context::GetRowLength()
{
    GLint value = 0;
    Context::GetIntegerv( OSMESA_ROW_LENGTH, &value );
    return value;
}

GLint Context::GetYAxisDirection()
{
    // value: zero = Y coordinates increase downward,
    //        non-zero = Y coordinates increase upward
    GLint value = 0;
    Context::GetIntegerv( OSMESA_Y_UP, &value );
    return value;
}

/*===========================================================================*/
/**
 *  @brief  Construct a Context class.
 */
/*===========================================================================*/
Context::Context():
    m_handle( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroy the Context class.
 */
/*===========================================================================*/
Context::~Context()
{
    this->destroy();
}

/*===========================================================================*/
/**
 *  @brief  Create a OSMesa rendering context
 *  @param  format [in] pixel format (OSMESA_*: COLOR_INDEX, RGBA, BGRA, ARGB, RGB, BGR)
 *  @param  depth_bits [in] desired bit size of depth buffer
 *  @param  stencil_bits [in] desired bit size of stencil buffer
 *  @param  accum_bits [in] desired bit size of accumulation buffer
 *  @return true if success, false if the context is invalid (error)
 */
/*===========================================================================*/
bool Context::create( GLenum format, GLint depth_bits, GLint stencil_bits, GLint accum_bits )
{
#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
    m_handle = OSMesaCreateContextExt( format, depth_bits, stencil_bits, accum_bits, NULL );
#else
    kvs::IgnoreUnusedVariable( depth_bits );
    kvs::IgnoreUnusedVariable( stencil_bits );
    kvs::IgnoreUnusedVariable( accum_bits );
    m_handle = OSMesaCreateContext( format, NULL );
#endif
    return this->isValid();
}

/*===========================================================================*/
/**
 *  @brief  Destroy the Context class.
 */
/*===========================================================================*/
void Context::destroy()
{
    if ( m_handle )
    {
        OSMesaDestroyContext( m_handle );
        m_handle = NULL;
    }
}

/*===========================================================================*/
/**
 *  @brief  Bind the context to a buffer and make it current buffer for rendering.
 *  @param  buffer [in] rendering buffer
 *  @param  width [in] width of rendering buffer
 *  @param  height [in] height of rendering buffer
 *  @return true if success, false if error because of invalid ctx,
 *          invalid buffer address, type!=GL_UNSIGNED_BYTE, width<1, height<1,
 *          width>internal limit or height>internal limit
 */
/*===========================================================================*/
bool Context::makeCurrent( kvs::osmesa::Surface& surface )
{
    kvs::UInt8* buffer = const_cast<kvs::UInt8*>( surface.buffer().data() );
    GLsizei width = surface.width();
    GLsizei height = surface.height();
    GLboolean status = OSMesaMakeCurrent( m_handle, buffer, GL_UNSIGNED_BYTE, width, height );
    return status == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Release the current buffer.
 */
/*===========================================================================*/
void Context::releaseCurrent()
{
    OSMesaMakeCurrent( NULL, NULL, GL_UNSIGNED_BYTE, 0, 0 );
}

} // end of namespace osmesa

} // end of namespace kvs
