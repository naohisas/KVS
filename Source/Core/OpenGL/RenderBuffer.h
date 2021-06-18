/*****************************************************************************/
/**
 *  @file   RenderBuffer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
private:
    GLuint m_id = 0; ///< buffer ID
    GLenum m_internal_format = 0; ///< render buffer internal format
    size_t m_width = 0; ///< buffer width
    size_t m_height = 0; ///< buffer height

public:
    class Binder;
    class GuardedBinder;

public:
    RenderBuffer() = default;
    RenderBuffer( const GLenum format ): m_internal_format( format ) {}
    virtual ~RenderBuffer() { this->release(); }

    GLuint id() const { return m_id; }
    GLenum internalFormat() const { return m_internal_format; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }

    void setInternalFormat( const GLenum format ) { m_internal_format = format; }

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

/*===========================================================================*/
/**
 *  @brief  Binder class for RenderBuffer.
 */
/*===========================================================================*/
class RenderBuffer::Binder
{
    const kvs::RenderBuffer& m_rb; ///< target render buffer
public:
    Binder( const kvs::RenderBuffer& rb ): m_rb( rb ) { m_rb.bind(); }
    ~Binder() { m_rb.unbind(); }
    Binder( const Binder& ) = delete;
    Binder& operator =( const Binder& ) = delete;
};

/*===========================================================================*/
/**
 *  @brief  Guarded binder class for RenderBuffer.
 */
/*===========================================================================*/
class RenderBuffer::GuardedBinder
{
    const kvs::RenderBuffer& m_rb; ///< target render buffer
    GLint m_id = 0; ///< target binding ID
public:
    GuardedBinder( const kvs::RenderBuffer& rb );
    ~GuardedBinder();
    GuardedBinder( const GuardedBinder& ) = delete;
    GuardedBinder& operator =( const GuardedBinder& ) = delete;
};

} // end of namespace kvs
