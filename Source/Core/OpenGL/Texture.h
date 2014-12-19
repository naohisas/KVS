/****************************************************************************/
/**
 *  @file   Texture.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture.h 1555 2013-04-21 02:31:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TEXTURE_H_INCLUDE
#define KVS__TEXTURE_H_INCLUDE

#include <cctype>
#include <kvs/GL>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Texture class.
 */
/*==========================================================================*/
class Texture
{
private:

    GLenum m_target; ///< target
    GLenum m_target_binding; ///< target binding
    GLuint m_id; ///< texture ID
    GLint m_internal_format; ///< internal pixel data format
    GLenum m_external_format; ///< external pixel data format
    GLenum m_external_type; ///< external pixel data type
    GLenum m_mag_filter; ///< filtering method for magnification
    GLenum m_min_filter; ///< filtering method for minification
    GLenum m_wrap_s; ///< wrap method for s-axis
    GLenum m_wrap_t; ///< wrap method for t-axis
    GLenum m_wrap_r; ///< wrap method for r-axis
    size_t m_width; ///< texture width
    size_t m_height; ///< texture height
    size_t m_depth; ///< texture depth

public:

    class Binder;
    class GuardedBinder;

    static void Bind( const Texture& texture, const GLint unit );
    static void Unbind( const Texture& texture, const GLint unit );
    static void Unbind( const GLenum target );
    static void SelectActiveUnit( const GLint unit );
    static void SetEnv( GLenum pname, GLfloat param );
    static void SetEnv( GLenum pname, GLint param );
    static void SetEnv( GLenum pname, const GLfloat* params );
    static void SetEnv( GLenum pname, const GLint* params );

public:

    Texture( const GLenum target, const GLenum target_binding );

    GLenum target() const;
    GLenum targetBinding() const;
    GLuint id() const;
    GLenum magFilter() const;
    GLenum minFilter() const;
    GLint internalFormat() const;
    GLenum externalFormat() const;
    GLenum externalType() const;
    GLenum wrapS() const;
    GLenum wrapT() const;
    GLenum wrapR() const;
    size_t width() const;
    size_t height() const;
    size_t depth() const;

    void setMagFilter( const GLenum mag_filter );
    void setMinFilter( const GLenum min_filter );
    void setWrapS( const GLenum wrap_s );
    void setWrapT( const GLenum wrap_t );
    void setWrapR( const GLenum wrap_r );
    void setPixelFormat( const GLint  internal_format, const GLenum external_format, const GLenum external_type );
    void setPixelFormat( const size_t nchannels, const size_t bytes_per_channel );
    void setSize( const size_t width, const size_t height = 0, const size_t depth = 0 );

    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;

protected:

    void createID();
    void deleteID();
    void setImage1D( GLsizei width, const GLvoid* data );
    void setImage2D( GLsizei width, GLsizei height, const GLvoid* data );
    void setImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data );
    void setImageRectangle( GLsizei width, GLsizei height, const GLvoid* data );
    void setSubImage1D( GLsizei width, const GLvoid* data, GLint xoffset = 0 );
    void setSubImage2D( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset = 0, GLint yoffset = 0 );
    void setSubImage3D( GLsizei width, GLsizei height, GLsizei depth, const GLvoid* data, GLint xoffset = 0, GLint yoffset = 0, GLint zoffset = 0 );
    void setSubImageRectangle( GLsizei width, GLsizei height, const GLvoid* data, GLint xoffset = 0, GLint yoffset = 0 );
    void copyImage1D( GLint x, GLint y, GLsizei width );
    void copyImage2D( GLint x, GLint y, GLsizei width, GLsizei height );
    void copySubImage1D( GLint x, GLint y, GLsizei width, GLint xoffset = 0 );
    void copySubImage2D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset = 0, GLint yoffset = 0 );
    void copySubImage3D( GLint x, GLint y, GLsizei width, GLsizei height, GLint xoffset = 0, GLint yoffset = 0, GLint zoffset = 0 );
    void setParameter( GLenum pname, GLfloat param );
    void setParameter( GLenum pname, GLint param );
    void setParameter( GLenum pname, GLenum param );
    void setParameters( GLenum pname, const GLfloat* params );
    void setParameters( GLenum pname, const GLint* params );
    void setPixelStorageMode( GLenum pname, GLfloat param );
    void setPixelStorageMode( GLenum pname, GLint param );

private:

    size_t get_nchannels( const GLenum external_format ) const;
    size_t get_channel_size( const GLenum external_type ) const;
    void estimate_pixel_format( const size_t nchannels, const size_t bytes_per_channel );
    void determine_pixel_format_for_1_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_2_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_3_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_4_channel( const size_t bytes_per_channel );

public:

    KVS_DEPRECATED( bool isTexture() const ) { return this->isValid(); }
};

class Texture::Binder
{
private:

    const Texture& m_texture;
    GLint m_unit;

public:

    Binder( const Texture& texture, const GLint unit = 0 );
    ~Binder();

private:

    Binder( const Binder& );
    Binder& operator =( const Binder& );
};

class Texture::GuardedBinder
{
private:

    const Texture& m_texture;
    GLint m_id;

public:

    GuardedBinder( const kvs::Texture& texture );
    ~GuardedBinder();

private:

    GuardedBinder( const GuardedBinder& );
    GuardedBinder& operator =( const GuardedBinder& );
};


} // end of namespace kvs

#endif // KVS__TEXTURE_H_INCLUDE
