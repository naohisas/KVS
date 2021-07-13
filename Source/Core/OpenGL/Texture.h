/****************************************************************************/
/**
 *  @file   Texture.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <cctype>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
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
    GLenum m_target = 0; ///< target
    GLenum m_target_binding = 0; ///< target binding
    GLuint m_id = 0; ///< texture ID
    GLint m_internal_format = 0; ///< internal pixel data format
    GLenum m_external_format = 0; ///< external pixel data format
    GLenum m_external_type = 0; ///< external pixel data type
    GLenum m_mag_filter = GL_LINEAR; ///< filtering method for magnification
    GLenum m_min_filter = GL_LINEAR; ///< filtering method for minification
    GLenum m_wrap_s = GL_CLAMP; ///< wrap method for s-axis
    GLenum m_wrap_t = GL_CLAMP; ///< wrap method for t-axis
    GLenum m_wrap_r = GL_CLAMP; ///< wrap method for r-axis
    size_t m_width = 0; ///< texture width
    size_t m_height = 0; ///< texture height
    size_t m_depth = 0; ///< texture depth

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
    Texture( const GLenum target, const GLenum target_binding ): m_target( target ), m_target_binding( target_binding ) {}
    virtual ~Texture() {}

    /*
    Texture() = delete;
    Texture( const Texture& ) = delete;
    Texture &operator = ( const Texture& ) = delete;

    Texture( Texture&& other )
    {
        m_target = other.m_target;
        m_target_binding = other.m_target_binding;
        m_id = other.m_id;
        m_internal_format = other.m_internal_format;
        m_external_format = other.m_external_format;
        m_external_type = other.m_external_type;
        m_mag_filter = other.m_mag_filter;
        m_min_filter = other.m_min_filter;
        m_wrap_s = other.m_wrap_s;
        m_wrap_t = other.m_wrap_t;
        m_wrap_r = other.m_wrap_r;
        m_width = other.m_width;
        m_height = other.m_height;
        m_depth = other.m_depth;
        other.m_id = 0;
    }

    Texture& operator = ( Texture&& other )
    {
        if ( this != &other )
        {
            this->deleteID();
            m_target = other.m_target;
            m_target_binding = other.m_target_binding;
            m_id = other.m_id;
            m_internal_format = other.m_internal_format;
            m_external_format = other.m_external_format;
            m_external_type = other.m_external_type;
            m_mag_filter = other.m_mag_filter;
            m_min_filter = other.m_min_filter;
            m_wrap_s = other.m_wrap_s;
            m_wrap_t = other.m_wrap_t;
            m_wrap_r = other.m_wrap_r;
            m_width = other.m_width;
            m_height = other.m_height;
            m_depth = other.m_depth;
            other.m_id = 0;
        }
        return *this;
    }
    */

    GLenum target() const { return m_target; }
    GLenum targetBinding() const { return m_target_binding; }
    GLuint id() const { return m_id; }
    GLenum magFilter() const { return m_mag_filter; }
    GLenum minFilter() const { return m_min_filter; }
    GLint internalFormat() const { return m_internal_format; }
    GLenum externalFormat() const { return m_external_format; }
    GLenum externalType() const { return m_external_type; }
    GLenum wrapS() const { return m_wrap_s; }
    GLenum wrapT() const { return m_wrap_t; }
    GLenum wrapR() const { return m_wrap_r; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t depth() const { return m_depth; }

    void setMagFilter( const GLenum mag_filter ) { m_mag_filter = mag_filter; }
    void setMinFilter( const GLenum min_filter ) { m_min_filter = min_filter; }
    void setWrapS( const GLenum wrap_s ) { m_wrap_s = wrap_s; }
    void setWrapT( const GLenum wrap_t ) { m_wrap_t = wrap_t; }
    void setWrapR( const GLenum wrap_r ) { m_wrap_r = wrap_r; }
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
//    size_t get_nchannels( const GLenum external_format ) const;
//    size_t get_channel_size( const GLenum external_type ) const;
    void estimate_pixel_format( const size_t nchannels, const size_t bytes_per_channel );
    void determine_pixel_format_for_1_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_2_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_3_channel( const size_t bytes_per_channel );
    void determine_pixel_format_for_4_channel( const size_t bytes_per_channel );
};

/*===========================================================================*/
/**
 *  @brief  Binder class for Texture class.
 */
/*===========================================================================*/
class Texture::Binder
{
private:
    const Texture& m_texture;
    GLint m_unit = 0;
public:
    Binder( const Texture& texture, const GLint unit = 0 );
    ~Binder();
    Binder( const Binder& ) = delete;
    Binder& operator =( const Binder& ) = delete;
};

class Texture::GuardedBinder
{
private:
    const Texture& m_texture;
    GLint m_id = 0;
public:
    GuardedBinder( const kvs::Texture& texture );
    ~GuardedBinder();
    GuardedBinder( const GuardedBinder& ) = delete;
    GuardedBinder& operator =( const GuardedBinder& ) = delete;
};


} // end of namespace kvs
