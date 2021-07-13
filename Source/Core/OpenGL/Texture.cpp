/****************************************************************************/
/**
 *  @file   Texture.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Texture.h"
#include <kvs/Message>
#include <kvs/Assert>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Binds the specified texture.
 *  @param  texture [in] texture that will be bound
 *  @param  unit [in] active texture unit number
 */
/*===========================================================================*/
void Texture::Bind( const Texture& texture, const GLint unit )
{
    KVS_ASSERT( texture.isCreated() );
    Texture::SelectActiveUnit( unit );
    texture.bind();
}

/*===========================================================================*/
/**
 *  @brief  Unbinds the specified texture.
 *  @param  texture [in] texture that will be unbound
 *  @param  unit [in] active texture unit number
 */
/*===========================================================================*/
void Texture::Unbind( const Texture& texture, const GLint unit )
{
    KVS_ASSERT( texture.isCreated() );
    Texture::SelectActiveUnit( unit );
    texture.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Unbinds the specified named texture.
 *  @param  target [in] target texture name
 */
/*===========================================================================*/
void Texture::Unbind( const GLenum target )
{
    KVS_GL_CALL( glBindTexture( target, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Selects active texture unit.
 *  @param  unit [in] unit number
 */
/*===========================================================================*/
void Texture::SelectActiveUnit( const GLint unit )
{
    KVS_ASSERT( unit >= 0 );
    KVS_ASSERT( unit < kvs::OpenGL::MaxCombinedTextureImageUnits() );
    KVS_GL_CALL( glActiveTexture( GL_TEXTURE0 + unit ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture environment parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::SetEnv( GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glTexEnvf( GL_TEXTURE_ENV, pname, param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture environment parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::SetEnv( GLenum pname, GLint param )
{
    KVS_GL_CALL( glTexEnvi( GL_TEXTURE_ENV, pname, param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture environment parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::SetEnv( GLenum pname, const GLfloat* params )
{
    KVS_GL_CALL( glTexEnvfv( GL_TEXTURE_ENV, pname, params ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture environment parameters.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter values
 */
/*===========================================================================*/
void Texture::SetEnv( GLenum pname, const GLint* params )
{
    KVS_GL_CALL( glTexEnviv( GL_TEXTURE_ENV, pname, params ) );
}

/*==========================================================================*/
/**
 *  @brief  Sets the pixel format.
 *  @param  internal_format [in] internal pixel format
 *  @param  external_format [in] external pixel format
 *  @param  external_type [in] external pixel data type
 */
/*==========================================================================*/
void Texture::setPixelFormat(
    const GLint internal_format,
    const GLenum external_format,
    const GLenum external_type )
{
    m_internal_format = internal_format;
    m_external_format = external_format;
    m_external_type = external_type;
}

/*==========================================================================*/
/**
 *  @brief  Set the pixel format by specifying a number of channels and a bytes per channel.
 *  @param  nchannels [in] number of channels (1, 2, 3 or 4)
 *  @param  bytes_per_channel [in] bytes per channel (1, 2 or 4)
 */
/*==========================================================================*/
void Texture::setPixelFormat( const size_t nchannels, const size_t bytes_per_channel )
{
    this->estimate_pixel_format( nchannels, bytes_per_channel );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture sizes.
 *  @param  width [in] texture width
 *  @param  height [in] texture height
 *  @param  depth [in] texture depth
 */
/*===========================================================================*/
void Texture::setSize( const size_t width, const size_t height, const size_t depth )
{
    m_width = width;
    m_height = height;
    m_depth = depth;
}

/*===========================================================================*/
/**
 *  @brief  Binds texture.
 */
/*===========================================================================*/
void Texture::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindTexture( m_target, m_id ) );
}

/*===========================================================================*/
/**
 *  @brief  Unbinds texture.
 */
/*===========================================================================*/
void Texture::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindTexture( m_target, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Determines if a texture is created.
 *  @return true if the texture has been already created
 */
/*===========================================================================*/
bool Texture::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determines if a texture is valid.
 *  @return true if the texture has been already allocated
 */
/*===========================================================================*/
bool Texture::isValid() const
{
    if ( m_id == 0 ) { return false; }
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsTexture( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if a texture is bound.
 *  @return true if the texture has been already bound
 */
/*===========================================================================*/
bool Texture::isBound() const
{
    if ( !this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( m_target_binding );
    return static_cast<GLuint>( id ) == m_id;
}

/*===========================================================================*/
/**
 *  @brief  Creates texture ID.
 */
/*===========================================================================*/
void Texture::createID()
{
//    if ( !this->isValid() )
    if ( !this->isCreated() )
    {
        KVS_GL_CALL( glGenTextures( 1, &m_id ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes texture ID.
 */
/*===========================================================================*/
void Texture::deleteID()
{
//    if ( this->isValid() )
    if ( this->isCreated() )
    {
        if ( this->isBound() ) { this->unbind(); }
        KVS_GL_CALL( glDeleteTextures( 1, &m_id ) );
        m_id = 0;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets 1D texture image.
 *  @param  width [in] image width
 *  @param  data [in] pointer to the image data
 */
/*===========================================================================*/
void Texture::setImage1D( GLsizei width, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage1D( m_target, level, m_internal_format, width, border, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets 2D texture image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  data [in] pointer to the image data
 */
/*===========================================================================*/
void Texture::setImage2D( GLsizei width, GLsizei height, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage2D( m_target, level, m_internal_format, width, height, border, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets 3D texture image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  depth [in] image depth
 *  @param  data [in] pointer to the image data
 */
/*===========================================================================*/
void Texture::setImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage3D( m_target, level, m_internal_format, width, height, depth, border, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets image rectanble.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  data [in] pointer to the image data
 */
/*===========================================================================*/
void Texture::setImageRectangle( GLsizei width, GLsizei height, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_RECTANGLE_ARB );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage2D( m_target, level, m_internal_format, width, height, border, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets 1D texture sub-image.
 *  @param  width [in] image width
 *  @param  data [in] pointer to the image data
 *  @param  xoffset [in] texel offset in the x-direction
 */
/*===========================================================================*/
void Texture::setSubImage1D( GLsizei width, const GLvoid* data, GLint xoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage1D( m_target, level, xoffset, width, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets 2D texture sub-image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  data [in] pointer to the image data
 *  @param  xoffset [in] texel offset in the x-direction
 *  @param  yoffset [in] texel offset in the y-direction
 */
/*===========================================================================*/
void Texture::setSubImage2D( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage2D( m_target, level, xoffset, yoffset, width, height, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets 3D texture sub-image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  depth [in] image depth
 *  @param  data [in] pointer to the image data
 *  @param  xoffset [in] texel offset in the x-direction
 *  @param  yoffset [in] texel offset in the y-direction
 *  @param  zoffset [in] texel offset in the z-direction
 */
/*===========================================================================*/
void Texture::setSubImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data, GLint xoffset, GLint yoffset, GLint zoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage3D( m_target, level, xoffset, yoffset, zoffset, width, height, depth, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets sub-image rectanble.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  data [in] pointer to the image data
 *  @param  xoffset [in] texel offset in the x-direction
 *  @param  yoffset [in] texel offset in the y-direction
 */
/*===========================================================================*/
void Texture::setSubImageRectangle( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_RECTANGLE_ARB );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage2D( m_target, level, xoffset, yoffset, width, height, m_external_format, m_external_type, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Copy pixels into a 1D texture image.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture image width
 */
/*===========================================================================*/
void Texture::copyImage1D( GLint x, GLint y, GLsizei width )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glCopyTexImage1D( m_target, level, m_internal_format, x, y, width, border ) );
}

/*===========================================================================*/
/**
 *  @brief  Copy pixels into a 2D texture image.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture image width
 *  @param  height [in] texture image height
 */
/*===========================================================================*/
void Texture::copyImage2D( GLint x, GLint y, GLsizei width, GLsizei height )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glCopyTexImage2D( m_target, level, m_internal_format, x, y, width, height, border ) );
}

/*===========================================================================*/
/**
 *  @brief  Copy pixels into a 1D texture sub-image.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture image width
 *  @param  xoffset [in] texel offset in the x-direction
 */
/*===========================================================================*/
void Texture::copySubImage1D( GLint x, GLint y, GLsizei width, GLint xoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage1D( m_target, level, xoffset, x, y, width ) );
}

/*===========================================================================*/
/**
 *  @brief  Copy pixels into a 2D texture image.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture image width
 *  @param  height [in] texture image height
 *  @param  xoffset [in] texel offset in the x-direction
 *  @param  yoffset [in] texel offset in the y-direction
 */
/*===========================================================================*/
void Texture::copySubImage2D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage2D( m_target, level, xoffset, yoffset, x, y, width, height ) );
}

/*===========================================================================*/
/**
 *  @brief  Copy pixels into a 3D texture image.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture image width
 *  @param  height [in] texture image height
 *  @param  xoffset [in] texel offset in the x-direction
 *  @param  yoffset [in] texel offset in the y-direction
 *  @param  zoffset [in] texel offset in the z-direction
 */
/*===========================================================================*/
void Texture::copySubImage3D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset, GLint yoffset, GLint zoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage3D( m_target, level, xoffset, yoffset, zoffset, x, y, width, height ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::setParameter( GLenum pname, GLfloat param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameterf( m_target, pname, param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::setParameter( GLenum pname, GLint param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameteri( m_target, pname, param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture parameter.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::setParameter( GLenum pname, GLenum param )
{
    this->setParameter( pname, static_cast<GLint>( param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture parameters.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter values
 */
/*===========================================================================*/
void Texture::setParameters( GLenum pname, const GLfloat* params )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameterfv( m_target, pname, params ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets texture parameters.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter values
 */
/*===========================================================================*/
void Texture::setParameters( GLenum pname, const GLint* params )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameteriv( m_target, pname, params ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets pixel strage mode.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::setPixelStorageMode( GLenum pname, GLfloat param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glPixelStoref( pname, param ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets pixel strage mode.
 *  @param  pname [in] parameter name
 *  @param  param [in] parameter value
 */
/*===========================================================================*/
void Texture::setPixelStorageMode( GLenum pname, GLint param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glPixelStorei( pname, param ) );
}

/*==========================================================================*/
/**
 *  Returns the number of channels.
 *  @param external_format [in] external pixel format
 */
/*==========================================================================*/
/*
size_t Texture::get_nchannels( const GLenum external_format ) const
{
    size_t nchannels = 0;
    switch( external_format )
    {
    case GL_COLOR_INDEX:
    case GL_STENCIL_INDEX:
    case GL_DEPTH_COMPONENT:
    case GL_RED:
    case GL_GREEN:
    case GL_BLUE:
    case GL_ALPHA:
    case GL_LUMINANCE:
        nchannels = 1;
        break;
    case GL_LUMINANCE_ALPHA:
        nchannels = 2;
        break;
    case GL_RGB:
#ifdef GL_BGR_EXT
    case GL_BGR_EXT:
#endif
        nchannels = 3;
        break;
    case GL_RGBA:
#ifdef GL_BGRA_EXT
    case GL_BGRA_EXT:
#endif
#ifdef GL_ABGR_EXT
    case GL_ABGR_EXT:
#endif
        nchannels = 4;
        break;
    default:
        kvsMessageError("Unsupported OpenGL external pixel format.");
        break;
    }

    return nchannels;
}
*/

/*==========================================================================*/
/**
 *  Returns the channel size.
 *  @param external_type [in] external_pixel data type
 */
/*==========================================================================*/
/*
size_t Texture::get_channel_size( const GLenum external_type ) const
{
    size_t channel_size = 0;
    switch( external_type )
    {
    case GL_UNSIGNED_BYTE:
    case GL_BYTE:
        channel_size = sizeof(char);
        break;
    case GL_UNSIGNED_SHORT:
    case GL_SHORT:
        channel_size = sizeof(short);
        break;
    case GL_UNSIGNED_INT:
    case GL_INT:
        channel_size = sizeof(int);
        break;
    case GL_FLOAT:
        channel_size = sizeof(float);
        break;
    default:
        kvsMessageError("Unsupported OpenGL external pixel data type.");
        break;
    }

    return channel_size;
}
*/

/*==========================================================================*/
/**
 *  Estimate a pixel format from a number of channels and a bytes per channel.
 *  @param nchannels [in] number of channels
 *  @param bytes_per_channel [in] bytes per channel
 */
/*==========================================================================*/
void Texture::estimate_pixel_format( const size_t nchannels, const size_t bytes_per_channel )
{
    // Initialize.
    this->setPixelFormat( 0, 0, 0 );

    switch( nchannels )
    {
    case 1: this->determine_pixel_format_for_1_channel( bytes_per_channel ); break;
    case 2: this->determine_pixel_format_for_2_channel( bytes_per_channel ); break;
    case 3: this->determine_pixel_format_for_3_channel( bytes_per_channel ); break;
    case 4: this->determine_pixel_format_for_4_channel( bytes_per_channel ); break;
    default:
        kvsMessageError("Invalid the number of channels is specified.");
        break;
    }
}

/*==========================================================================*/
/**
 *  Determine a pixel format for a channel.
 *  @param bytes_per_channel [in] bytes per channel
 */
/*==========================================================================*/
void Texture::determine_pixel_format_for_1_channel( const size_t bytes_per_channel )
{
    switch( bytes_per_channel )
    {
    case 1: this->setPixelFormat( GL_INTENSITY8,  GL_LUMINANCE, GL_UNSIGNED_BYTE  ); break;
    case 2: this->setPixelFormat( GL_INTENSITY16, GL_LUMINANCE, GL_UNSIGNED_SHORT ); break;
    case 4: this->setPixelFormat( GL_INTENSITY,   GL_LUMINANCE, GL_FLOAT          ); break;
    default:
        kvsMessageError("Bytes per channel must be 1, 2 or 4.");
        break;
    }
}

/*==========================================================================*/
/**
 *  Determine a pixel format for two channels.
 *  @param bytes_per_channel [in] bytes per channel
 */
/*==========================================================================*/
void Texture::determine_pixel_format_for_2_channel( const size_t bytes_per_channel )
{
    switch( bytes_per_channel )
    {
    case 1: this->setPixelFormat( GL_LUMINANCE8_ALPHA8,   GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE  ); break;
    case 2: this->setPixelFormat( GL_LUMINANCE16_ALPHA16, GL_LUMINANCE_ALPHA, GL_UNSIGNED_SHORT ); break;
    case 4: this->setPixelFormat( GL_LUMINANCE_ALPHA,     GL_LUMINANCE_ALPHA, GL_FLOAT          ); break;
    default:
        kvsMessageError("Bytes per channel must be 1, 2 or 4.");
        break;
    }
}

/*==========================================================================*/
/**
 *  Determine a pixel format for three channels.
 *  @param bytes_per_channel [in] bytes per channel
 */
/*==========================================================================*/
void Texture::determine_pixel_format_for_3_channel( const size_t bytes_per_channel )
{
    switch( bytes_per_channel )
    {
    case 1: this->setPixelFormat( GL_RGB8,  GL_RGB, GL_UNSIGNED_BYTE  ); break;
    case 2: this->setPixelFormat( GL_RGB16, GL_RGB, GL_UNSIGNED_SHORT ); break;
    case 4: this->setPixelFormat( GL_RGB,   GL_RGB, GL_FLOAT          ); break;
    default:
        kvsMessageError("Bytes per channel must be 1, 2 or 4.");
        break;
    }
}

/*==========================================================================*/
/**
 *  Determine a pixel format for four channels.
 *  @param bytes_per_channel [in] bytes per channel
 */
/*==========================================================================*/
void Texture::determine_pixel_format_for_4_channel( const size_t bytes_per_channel )
{
    switch( bytes_per_channel )
    {
    case 1: this->setPixelFormat( GL_RGBA8,  GL_RGBA, GL_UNSIGNED_BYTE  ); break;
    case 2: this->setPixelFormat( GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT ); break;
    case 4: this->setPixelFormat( GL_RGBA,   GL_RGBA, GL_FLOAT          ); break;
    default:
        kvsMessageError("Bytes per channel must be 1, 2 or 4.");
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a new Texture::Binder class.
 *  @param  texture [in] texture that will be bound
 *  @param  unit [in] texture unit number
 */
/*===========================================================================*/
Texture::Binder::Binder( const Texture& texture, GLint unit ) :
    m_texture( texture ),
    m_unit( unit )
{
    Texture::SelectActiveUnit( m_unit );
    m_texture.bind();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Texture::Binder class.
 */
/*===========================================================================*/
Texture::Binder::~Binder()
{
    Texture::SelectActiveUnit( m_unit );
    Texture::Unbind( m_texture.target() );
}

/*===========================================================================*/
/**
 *  @brief  Creates a new Texture::GuardedBinder class.
 *  @param  texture [in] texture that will be bound
 */
/*===========================================================================*/
Texture::GuardedBinder::GuardedBinder( const Texture& texture ):
    m_texture( texture ),
    m_id( kvs::OpenGL::Integer( texture.targetBinding() ) )
{
    if ( m_texture.id() != static_cast<GLuint>( m_id ) )
    {
        m_texture.bind();
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Texture::GuardedBinder class.
 */
/*===========================================================================*/
Texture::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_texture.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_texture.id() )
    {
        KVS_GL_CALL( glBindTexture( m_texture.target(), m_id ) );
    }
}

} // end of namespace kvs
