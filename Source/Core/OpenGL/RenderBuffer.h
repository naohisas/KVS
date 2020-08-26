/*****************************************************************************/
/**
 *  @file   RenderBuffer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__RENDER_BUFFER_H_INCLUDE
#define KVS__RENDER_BUFFER_H_INCLUDE

#include <kvs/GL>
#include <cstddef>


namespace kvs
{

/*===========================================================================*/
/**
 *  Render buffer class.
 */
/*===========================================================================*/
class RenderBuffer
{
protected:

    GLuint m_id; ///< buffer ID
    GLenum m_internal_format; ///< render buffer internal format
    size_t m_width; ///< buffer width
    size_t m_height; ///< buffer height

public:

    RenderBuffer();
    RenderBuffer( const GLenum internal_format );
    virtual ~RenderBuffer();

    GLuint id() const;
    GLenum internalFormat() const;
    size_t width() const;
    size_t height() const;

    void setInternalFormat( const GLenum internal_format );

    void create( const size_t width, const size_t height );
    void release();
    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;

protected:

    void createID();
    void deleteID();
    void setRenderbufferStorage( GLenum internal_format, GLsizei width, GLsizei height );
};

} // end of namespace kvs

#endif // KVS__RENDER_BUFFER_H_INCLUDE
