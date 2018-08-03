/*****************************************************************************/
/**
 *  @file   BufferObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BufferObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__BUFFER_OBJECT_H_INCLUDE
#define KVS__BUFFER_OBJECT_H_INCLUDE

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

    GLenum m_target; ///< target (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
    GLenum m_target_binding; ///< target binding
    GLuint m_id; ///< buffer ID
    GLenum m_usage; ///< usage
    size_t m_size; ///< buffer size [byte]
    bool m_is_loaded; ///< test whether the memory is allocated on the GPU or not

public:

    class Binder;
    class GuardedBinder;

public:

    BufferObject( const GLenum target, const GLenum target_binding, const GLenum usage );
    virtual ~BufferObject();

    GLuint id() const;
    GLenum target() const;
    GLenum targetBinding() const;
    size_t size() const;

    void setUsage( const GLenum usage );
    void setSize( const size_t size );

    void create( const size_t size, const void* data = NULL );
    void release();
    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;

    void load( const size_t size, const void* data, const size_t offset = 0 );
    void* map( const GLenum access_type = kvs::BufferObject::ReadWrite );
    void unmap();

    KVS_DEPRECATED( void download( const size_t size, const void* data, const size_t offset = 0 ) ) { this->load( size, data, offset ); }
protected:

    void createID();
    void deleteID();
    void setBufferData( GLsizei width, const GLvoid* data );
    void setBufferSubData( GLsizei width, const GLvoid* data, GLint xoffset = 0 );
    void* mapBuffer( const GLenum access_type );
    void unmapBuffer();

private:

    BufferObject( const BufferObject& );
    BufferObject& operator =( const BufferObject& );
};

class BufferObject::Binder
{
    const kvs::BufferObject& m_bo;

public:

    Binder( const kvs::BufferObject& bo );
    ~Binder();

private:

    Binder( const Binder& );
    Binder& operator =( const Binder& );
};

class BufferObject::GuardedBinder
{
    const kvs::BufferObject& m_bo;
    GLint m_id;

public:

    GuardedBinder( const kvs::BufferObject& bo );
    ~GuardedBinder();

private:

    GuardedBinder( const GuardedBinder& );
    GuardedBinder& operator =( const GuardedBinder& );
};

} // end of namespace kvs

#endif // KVS__BUFFER_OBJECT_H_INCLUDE
