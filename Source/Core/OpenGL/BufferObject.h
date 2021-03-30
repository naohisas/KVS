/*****************************************************************************/
/**
 *  @file   BufferObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/GL>
#include <kvs/Deprecated>
#include <cstddef>


namespace kvs
{

/*===========================================================================*/
/**
 *  Buffer object class.
 */
/*===========================================================================*/
class BufferObject
{
public:
    enum AccessType
    {
        ReadOnly  = GL_READ_ONLY,  ///< reading pixel data from PBO
        WriteOnly = GL_WRITE_ONLY, ///< writing pixel data to PBO
        ReadWrite = GL_READ_WRITE  ///< reading and writing
    };

    enum UsageType
    {
        StaticDraw  = GL_STATIC_DRAW,  ///< data will not be changed and sent to GPU
        StaticRead  = GL_STATIC_READ,  ///< data will not be changed and read by the application
        StaticCopy  = GL_STATIC_COPY,  ///< data will not be changed and used both drawing and reading
        DynamicDraw = GL_DYNAMIC_DRAW, ///< data will be changed frequently and sent to GPU
        DynamicRead = GL_DYNAMIC_READ, ///< data will be changed frequently and read by the application
        DynamicCopy = GL_DYNAMIC_COPY, ///< data will be changed frequently and used both drawing and reading
        StreamDraw  = GL_STREAM_DRAW,  ///< data will be changed every frame and sent to GPU
        StreamRead  = GL_STREAM_READ,  ///< data will be changed every frame and read by the application
        StreamCopy  = GL_STREAM_COPY   ///< data will be changed every frame and used both drawing and reading
    };

private:
    GLenum m_target = 0; ///< target (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.)
    GLenum m_target_binding = 0; ///< target binding
    GLenum m_usage = 0; ///< usage
    GLuint m_id = 0; ///< buffer ID
    size_t m_size = 0; ///< buffer size [byte]
    bool m_is_loaded = false; ///< test whether the memory is allocated on the GPU or not

public:
    class Binder;
    class GuardedBinder;

    static GLsizei PaddedBufferSize( GLsizei size );

public:
    BufferObject( const GLenum target, const GLenum target_binding, const GLenum usage );
    virtual ~BufferObject() { this->release(); }
    BufferObject( const BufferObject& ) = delete;
    BufferObject& operator =( const BufferObject& ) = delete;

    GLuint id() const { return m_id; }
    GLenum target() const { return m_target; }
    GLenum targetBinding() const { return m_target_binding; }
    size_t size() const { return m_size; }

    void setUsage( const GLenum usage ) { m_usage = usage; }
    void setSize( const size_t size ) { m_size = size; }

    void create( const size_t size, const void* data = NULL );
    void release();
    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;

    GLsizei load( const size_t size, const void* data, const size_t offset = 0 );
    void* map( const GLenum type = AccessType::ReadWrite ) { return this->mapBuffer( type ); }
    void unmap() { this->unmapBuffer(); }

protected:
    void createID();
    void deleteID();
    void setBufferData( GLsizei width, const GLvoid* data );
    void setBufferSubData( GLsizei width, const GLvoid* data, GLint xoffset = 0 );
    void* mapBuffer( const GLenum access_type );
    void unmapBuffer();
};

/*===========================================================================*/
/**
 *  @brief  Binder class for BufferObject.
 */
/*===========================================================================*/
class BufferObject::Binder
{
    const kvs::BufferObject& m_bo;
public:
    Binder( const kvs::BufferObject& bo );
    ~Binder();
    Binder( const Binder& ) = delete;
    Binder& operator =( const Binder& ) = delete;
};

/*===========================================================================*/
/**
 *  @brief  Guarded binder class for BufferObject.
 */
/*===========================================================================*/
class BufferObject::GuardedBinder
{
    const kvs::BufferObject& m_bo;
    GLint m_id = 0;
public:
    GuardedBinder( const kvs::BufferObject& bo );
    ~GuardedBinder();
    GuardedBinder( const GuardedBinder& ) = delete;
    GuardedBinder& operator =( const GuardedBinder& ) = delete;
};

} // end of namespace kvs
