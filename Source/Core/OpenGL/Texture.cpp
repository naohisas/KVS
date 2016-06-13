/****************************************************************************/
/**
 *  @file   Texture.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture.cpp 1555 2013-04-21 02:31:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Texture.h"
#include <kvs/Message>
#include <kvs/Assert>
#include <kvs/OpenGL>


namespace kvs
{

void Texture::Bind( const Texture& texture, const GLint unit )
{
    KVS_ASSERT( texture.isCreated() );
    Texture::SelectActiveUnit( unit );
    texture.bind();
}

void Texture::Unbind( const Texture& texture, const GLint unit )
{
    KVS_ASSERT( texture.isCreated() );
    Texture::SelectActiveUnit( unit );
    texture.unbind();
}

void Texture::Unbind( const GLenum target )
{
    KVS_GL_CALL( glBindTexture( target, 0 ) );
}

void Texture::SelectActiveUnit( const GLint unit )
{
    KVS_ASSERT( unit >= 0 );
    KVS_ASSERT( unit < kvs::OpenGL::MaxCombinedTextureImageUnits() );
    KVS_GL_CALL( glActiveTexture( GL_TEXTURE0 + unit ) );
}

void Texture::SetEnv( GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glTexEnvf( GL_TEXTURE_ENV, pname, param ) );
}

void Texture::SetEnv( GLenum pname, GLint param )
{
    KVS_GL_CALL( glTexEnvi( GL_TEXTURE_ENV, pname, param ) );
}

void Texture::SetEnv( GLenum pname, const GLfloat* params )
{
    KVS_GL_CALL( glTexEnvfv( GL_TEXTURE_ENV, pname, params ) );
}

void Texture::SetEnv( GLenum pname, const GLint* params )
{
    KVS_GL_CALL( glTexEnviv( GL_TEXTURE_ENV, pname, params ) );
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Texture::Texture( const GLenum target, const GLenum target_binding ):
    m_target( target ),
    m_target_binding( target_binding ),
    m_id( 0 ),
    m_internal_format( 0 ),
    m_external_format( 0 ),
    m_external_type( 0 ),
    m_mag_filter( GL_LINEAR ),
    m_min_filter( GL_LINEAR ),
    m_wrap_s( GL_CLAMP ),
    m_wrap_t( GL_CLAMP ),
    m_wrap_r( GL_CLAMP ),
    m_width( 0 ),
    m_height( 0 ),
    m_depth( 0 )
{
}

GLenum Texture::target() const
{
    return m_target;
}


GLenum Texture::targetBinding() const
{
    return m_target_binding;
}

/*==========================================================================*/
/**
 *  Returns the texture ID.
 */
/*==========================================================================*/
GLuint Texture::id() const
{
    return m_id;
}

/*==========================================================================*/
/**
 *  Returns the magnification filter.
 */
/*==========================================================================*/
GLenum Texture::magFilter() const
{
    return m_mag_filter;
}

/*==========================================================================*/
/**
 *  Returns the minification filter.
 */
/*==========================================================================*/
GLenum Texture::minFilter() const
{
    return m_min_filter;
}

/*==========================================================================*/
/**
 *  Returns the internal pixel format.
 */
/*==========================================================================*/
GLint Texture::internalFormat() const
{
    return m_internal_format;
}

/*==========================================================================*/
/**
 *  Returns the external pixel format.
 */
/*==========================================================================*/
GLenum Texture::externalFormat() const
{
    return m_external_format;
}

/*==========================================================================*/
/**
 *  Returns the external pixel data type.
 */
/*==========================================================================*/
GLenum Texture::externalType() const
{
    return m_external_type;
}

/*==========================================================================*/
/**
 *  Returns the wrap method for s-axis.
 */
/*==========================================================================*/
GLenum Texture::wrapS() const
{
    return m_wrap_s;
}

/*==========================================================================*/
/**
 *  Returns the wrap method for t-axis.
 */
/*==========================================================================*/
GLenum Texture::wrapT() const
{
    return m_wrap_t;
}

/*==========================================================================*/
/**
 *  Returns the wrap method for r-axis.
 */
/*==========================================================================*/
GLenum Texture::wrapR() const
{
    return m_wrap_r;
}

/*==========================================================================*/
/**
 *  Returns the texture width.
 */
/*==========================================================================*/
size_t Texture::width() const
{
    return m_width;
}

/*==========================================================================*/
/**
 *  Returns the texture height.
 */
/*==========================================================================*/
size_t Texture::height() const
{
    return m_height;
}

/*==========================================================================*/
/**
 *  Returns the texture depth.
 */
/*==========================================================================*/
size_t Texture::depth() const
{
    return m_depth;
}

/*==========================================================================*/
/**
 *  Set the magnification filter.
 *  @param mag_filter [in] magnification filter
 */
/*==========================================================================*/
void Texture::setMagFilter( const GLenum mag_filter )
{
    m_mag_filter = mag_filter;
}

/*==========================================================================*/
/**
 *  Set the minification filter.
 *  @param min_filter [in] minification filter
 */
/*==========================================================================*/
void Texture::setMinFilter( const GLenum min_filter )
{
    m_min_filter = min_filter;
}

/*==========================================================================*/
/**
 *  Set the wrap method for s-axis.
 *  @param wrap_s [in] wrap method for s-axis
 */
/*==========================================================================*/
void Texture::setWrapS( const GLenum wrap_s )
{
    m_wrap_s = wrap_s;
}

/*==========================================================================*/
/**
 *  Set the wrap method for t-axis.
 *  @param wrap_t [in] wrap method for t-axis
 */
/*==========================================================================*/
void Texture::setWrapT( const GLenum wrap_t )
{
    m_wrap_t = wrap_t;
}

/*==========================================================================*/
/**
 *  Set the wrap method for r-axis.
 *  @param wrap_r [in] wrap method for r-axis
 */
/*==========================================================================*/
void Texture::setWrapR( const GLenum wrap_r )
{
    m_wrap_r = wrap_r;
}

/*==========================================================================*/
/**
 *  Set the pixel format.
 *  @param internal_format [in] internal pixel format
 *  @param external_format [in] external pixel format
 *  @param external_type [in] external pixel data type
 */
/*==========================================================================*/
void Texture::setPixelFormat(
    const GLint  internal_format,
    const GLenum external_format,
    const GLenum external_type )
{
    m_internal_format = internal_format;
    m_external_format = external_format;
    m_external_type = external_type;
}

/*==========================================================================*/
/**
 *  Set the pixel format by specifying a number of channels and a bytes per channel.
 *  @param nchannels [in] number of channels (1, 2, 3 or 4)
 *  @param bytes_per_channel [in] bytes per channel (1, 2 or 4)
 */
/*==========================================================================*/
void Texture::setPixelFormat( const size_t nchannels, const size_t bytes_per_channel )
{
    this->estimate_pixel_format( nchannels, bytes_per_channel );
}

void Texture::setSize( const size_t width, const size_t height, const size_t depth )
{
    m_width = width;
    m_height = height;
    m_depth = depth;
}

void Texture::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindTexture( m_target, m_id ) );
}

void Texture::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindTexture( m_target, 0 ) );
}

bool Texture::isCreated() const
{
    return m_id > 0;
}

bool Texture::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsTexture( m_id ) );
    return result == GL_TRUE;
}

bool Texture::isBound() const
{
    if ( !this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( m_target_binding );
    return static_cast<GLuint>( id ) == m_id;
}

void Texture::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( glGenTextures( 1, &m_id ) );
    }
}

void Texture::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteTextures( 1, &m_id ) );
    }
    m_id = 0;
}

void Texture::setImage1D( GLsizei width, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage1D( m_target, level, m_internal_format, width, border, m_external_format, m_external_type, data ) );
}

void Texture::setImage2D( GLsizei width, GLsizei height, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage2D( m_target, level, m_internal_format, width, height, border, m_external_format, m_external_type, data ) );
}

void Texture::setImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage3D( m_target, level, m_internal_format, width, height, depth, border, m_external_format, m_external_type, data ) );
}

void Texture::setImageRectangle( GLsizei width, GLsizei height, const GLvoid* data )
{
    KVS_ASSERT( m_target == GL_TEXTURE_RECTANGLE_ARB );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glTexImage2D( m_target, level, m_internal_format, width, height, border, m_external_format, m_external_type, data ) );
}

void Texture::setSubImage1D( GLsizei width, const GLvoid* data, GLint xoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage1D( m_target, level, xoffset, width, m_external_format, m_external_type, data ) );
}

void Texture::setSubImage2D( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage2D( m_target, level, xoffset, yoffset, width, height, m_external_format, m_external_type, data ) );
}

void Texture::setSubImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data, GLint xoffset, GLint yoffset, GLint zoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage3D( m_target, level, xoffset, yoffset, zoffset, width, height, depth, m_external_format, m_external_type, data ) );
}

void Texture::setSubImageRectangle( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_RECTANGLE_ARB );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glTexSubImage2D( m_target, level, xoffset, yoffset, width, height, m_external_format, m_external_type, data ) );
}

void Texture::copyImage1D( GLint x, GLint y, GLsizei width )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glCopyTexImage1D( m_target, level, m_internal_format, x, y, width, border ) );
}

void Texture::copyImage2D( GLint x, GLint y, GLsizei width, GLsizei height )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    const GLint border = 0; // border width (0 or 1)
    KVS_GL_CALL( glCopyTexImage2D( m_target, level, m_internal_format, x, y, width, height, border ) );
}

void Texture::copySubImage1D( GLint x, GLint y, GLsizei width, GLint xoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_1D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage1D( m_target, level, xoffset, x, y, width ) );
}

void Texture::copySubImage2D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset, GLint yoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_2D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage2D( m_target, level, xoffset, yoffset, x, y, width, height ) );
}

void Texture::copySubImage3D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset, GLint yoffset, GLint zoffset )
{
    KVS_ASSERT( m_target == GL_TEXTURE_3D );
    KVS_ASSERT( this->isBound() );

    const GLint level = 0; // level-of-detail number
    KVS_GL_CALL( glCopyTexSubImage3D( m_target, level, xoffset, yoffset, zoffset, x, y, width, height ) );
}

void Texture::setParameter( GLenum pname, GLfloat param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameterf( m_target, pname, param ) );
}

void Texture::setParameter( GLenum pname, GLint param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameteri( m_target, pname, param ) );
}

void Texture::setParameter( GLenum pname, GLenum param )
{
    this->setParameter( pname, static_cast<GLint>( param ) );
}

void Texture::setParameters( GLenum pname, const GLfloat* params )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameterfv( m_target, pname, params ) );
}

void Texture::setParameters( GLenum pname, const GLint* params )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glTexParameteriv( m_target, pname, params ) );
}

void Texture::setPixelStorageMode( GLenum pname, GLfloat param )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glPixelStoref( pname, param ) );
}

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

/*==========================================================================*/
/**
 *  Returns the channel size.
 *  @param external_type [in] external_pixel data type
 */
/*==========================================================================*/
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

Texture::Binder::Binder( const Texture& texture, GLint unit ) :
    m_texture( texture ),
    m_unit( unit )
{
    KVS_ASSERT( texture.isCreated() );
    Texture::SelectActiveUnit( unit );
    texture.bind();
}

Texture::Binder::~Binder()
{
    KVS_ASSERT( m_texture.isCreated() );
    Texture::SelectActiveUnit( m_unit );
    KVS_GL_CALL( glBindTexture( m_texture.target(), 0 ) );
}

Texture::GuardedBinder::GuardedBinder( const Texture& texture ):
    m_texture( texture ),
    m_id( kvs::OpenGL::Integer( texture.targetBinding() ) )
{
    KVS_ASSERT( texture.isCreated() );
    if ( texture.id() != static_cast<GLuint>( m_id ) )
    {
        texture.bind();
    }
}

Texture::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_texture.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_texture.id() )
    {
        KVS_GL_CALL( glBindTexture( m_texture.target(), m_id ) );
    }
}

} // end of namespace kvs
