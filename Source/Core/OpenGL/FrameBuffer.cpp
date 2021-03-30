/****************************************************************************/
/**
 *  @file   FrameBuffer.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "FrameBuffer.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Set the pixel data format.
 *  @param format [in] pixel data format
 */
/*==========================================================================*/
void FrameBuffer::setFormat( const GLenum format )
{
    /* The following values are accepted:
     *     GL_COLOR_INDEX
     *     GL_STENCIL_INDEX
     *     GL_DEPTH_COMPONENT
     *     GL_RED
     *     GL_GREEN
     *     GL_BLUE
     *     GL_ALPHA
     *     GL_RGB
     *     GL_RGBA
     *     GL_LUMINANCE
     *     GL_LUMINANCE_ALPHA
     */
    m_format = format;
}

/*==========================================================================*/
/**
 *  Set the pixel data type.
 *  @param type [in] pixel data type
 */
/*==========================================================================*/
void FrameBuffer::setType( const GLenum type )
{
    /* The following values are accepted:
     *     GL_UNSIGNED_BYTE
     *     GL_BYTE
     *     GL_BITMAP
     *     GL_UNSIGNED_SHORT
     *     GL_SHORT
     *     GL_UNSIGNED_INT
     *     GL_INT
     *     GL_FLOAT
     */
    m_type = type;
}

/*==========================================================================*/
/**
 *  Read the pixel data from the frame buffer.
 *  @param x [in] x value of the window coordinates
 *  @param y [in] y value of the window coordinates
 *  @param width [in] width of the pixel rectangle
 *  @param height [in] height of the pixel rectangle
 *  @param pixels [out] pointer to the pixel data
 *  @param buffer [in] color buffer that is read from the frame buffer
 */
/*==========================================================================*/
void FrameBuffer::readPixels(
    const int x,
    const int y,
    const size_t width,
    const size_t height,
    void* pixels,
    const GLenum buffer ) const
{
    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, 1 );

    if ( buffer != 0 )
    {
        GLint current_buffer = kvs::OpenGL::Integer( GL_READ_BUFFER );
        kvs::OpenGL::SetReadBuffer( buffer );
        kvs::OpenGL::ReadPixels( x, y, width, height, m_format, m_type, pixels );
        kvs::OpenGL::SetReadBuffer( current_buffer );
    }
    else
    {
        kvs::OpenGL::ReadPixels( x, y, width, height, m_format, m_type, pixels );
    }
}

/*==========================================================================*/
/**
 *  Draw the pixel data to the frame buffer.
 *  @param x [in] x value of the window coordinates
 *  @param y [in] y value of the window coordinates
 *  @param width [in] width of the pixel rectangle
 *  @param height [in] height of the pixel rectangle
 *  @param pixels [in] pointer to the pixel data
 *  @param buffer [in] color buffer that is read from the frame buffer
 */
/*==========================================================================*/
void FrameBuffer::drawPixels(
    const int x,
    const int y,
    const size_t width,
    const size_t height,
    const void* pixels,
    const GLenum buffer ) const
{
    GLint current_buffer = 0;
    if ( buffer != 0 )
    {
        current_buffer = kvs::OpenGL::Integer( GL_DRAW_BUFFER );
        kvs::OpenGL::SetDrawBuffer( buffer );
    }

    kvs::OpenGL::WithDisabled d1( GL_TEXTURE_1D );
    kvs::OpenGL::WithDisabled d2( GL_TEXTURE_2D );
    kvs::OpenGL::WithDisabled d3( GL_TEXTURE_3D );
    {
        GLint viewport[4];
        kvs::OpenGL::GetViewport( viewport );
        const int left = viewport[0];
        const int bottom = viewport[1];
        const int right = viewport[0] + viewport[2];
        const int top = viewport[1] + viewport[3];

        kvs::OpenGL::WithPushedMatrix p1( GL_PROJECTION );
        p1.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix p2( GL_MODELVIEW );
            p2.loadIdentity();
            {
                kvs::OpenGL::SetOrtho( left, right, bottom, top, -1, 1 );
                kvs::OpenGL::SetPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );
                kvs::OpenGL::SetRasterPos( x, y );
                kvs::OpenGL::DrawPixels( width, height, m_format, m_type, pixels );
            }
        }
    }

    if ( buffer != 0 ) kvs::OpenGL::SetDrawBuffer( current_buffer );
}

} // end of namespace kvs
