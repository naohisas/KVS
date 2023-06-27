/*****************************************************************************/
/**
 *  @file   BufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "BufferObject.h"
#include <kvs/Assert>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns the padded buffer size for input size.
 *  @return buffer size
 */
/*===========================================================================*/
GLsizeiptr BufferObject::PaddedBufferSize( GLsizeiptr size )
{
     int x = size;
     return (x + 15) & ~15;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new BufferObject class.
 *  @param  target [in] bind target
 *  @param  target_binding [in] target binding
 *  @param  usage [in] buffer usage
 */
/*===========================================================================*/
BufferObject::BufferObject(
    const GLenum target,
    const GLenum target_binding,
    const GLenum usage ):
    m_target( target ),
    m_target_binding( target_binding ),
    m_usage( usage )
{
}

/*===========================================================================*/
/**
 *  @brief  Create buffer.
 *  @param  size [in] buffer size
 *  @param  data [in] pointer to loaded buffer data
 */
/*===========================================================================*/
void BufferObject::create( const size_t size, const void* data )
{
    this->createID();
    this->setSize( size );
    GuardedBinder binder( *this );
    this->load( size, data );
}

/*===========================================================================*/
/**
 *  @brief  Release buffer.
 */
/*===========================================================================*/
void BufferObject::release()
{
    this->deleteID();
    m_is_loaded = false;
}

/*===========================================================================*/
/**
 *  @brief  Bind buffer.
 */
/*===========================================================================*/
void BufferObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindBuffer( m_target, m_id ) );
}

/*===========================================================================*/
/**
 *  @brief  Unbind buffer.
 */
/*===========================================================================*/
void BufferObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindBuffer( m_target, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Determins if a buffer object is created.
 *  @return true if the buffer object has been already created
 */
/*===========================================================================*/
bool BufferObject::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determins if a buffer object is valid.
 *  @return true if the buffer object has been already allocated
 */
/*===========================================================================*/
bool BufferObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsBuffer( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if a buffer object is bound.
 *  @return true if the buffer object has been already bound
 */
/*===========================================================================*/
bool BufferObject::isBound() const
{
    if ( !this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( m_target_binding );
    return static_cast<GLuint>( id ) == m_id;
}

/*===========================================================================*/
/**
 *  @brief  Load buffer data from CPU to GPU.
 *  @param  size [in] buffer data size
 *  @param  data [in] pointer to loaded buffer data
 *  @param  offset [in] texel offset within the existing buffer data array
 */
/*===========================================================================*/
GLsizeiptr BufferObject::load( const size_t size, const void* data, const size_t offset )
{
    if ( !m_is_loaded )
    {
        this->setBufferData( size, data );
        m_is_loaded = true;
    }
    else
    {
        this->setBufferSubData( size, data, offset );
    }
    return BufferObject::PaddedBufferSize( size );
}

/*===========================================================================*/
/**
 *  @brief  Creates bufffer object ID.
 */
/*===========================================================================*/
void BufferObject::createID()
{
//    if ( !this->isValid() )
    if ( !this->isCreated() )
    {
        KVS_GL_CALL( glGenBuffers( 1, &m_id ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes buffer object ID.
 */
/*===========================================================================*/
void BufferObject::deleteID()
{
//    if ( this->isValid() )
    if ( this->isCreated() )
    {
        if ( this->isBound() ) { this->unbind(); }
        KVS_GL_CALL( glDeleteBuffers( 1, &m_id ) );
        m_id = 0;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets data for buffer object.
 *  @param  size [in] data size in bytes
 *  @param  data [in] pointer to data that will be copied
 */
/*===========================================================================*/
void BufferObject::setBufferData( GLsizeiptr size, const GLvoid* data )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBufferData( m_target, size, data, m_usage ) );
}

/*===========================================================================*/
/**
 *  @brief  Sets subset of data for buffer object.
 *  @param  size [in] data size in bytes
 *  @param  data [in] pointer to data that will be copied
 *  @param  offset [in] offset into the buffer object's data in bytes
 */
/*===========================================================================*/
void BufferObject::setBufferSubData( GLsizeiptr size, const GLvoid* data, GLint offset )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBufferSubData( m_target, offset, size, data ) );
}

/*===========================================================================*/
/**
 *  @brief  Maps buffer object'data into the client's address space and returns it.
 *  @param  access_type [in] access type (ReadOnly, WriteOnly or ReadWrite)
 *  @return Mapped address of the buffer object's data
 */
/*===========================================================================*/
void* BufferObject::mapBuffer( const GLenum access_type )
{
    KVS_ASSERT( this->isBound() );
    void* result = 0;
    KVS_GL_CALL( result = glMapBuffer( m_target, access_type ) );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Releases the mapped address of the buffer object's data.
 */
/*===========================================================================*/
void BufferObject::unmapBuffer()
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glUnmapBuffer( m_target ) );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GuardedBinder class.
 *  @param  bo [in] target buffer object that will be bound
 */
/*===========================================================================*/
BufferObject::GuardedBinder::GuardedBinder( const kvs::BufferObject& bo ):
    m_bo( bo ),
    m_id( kvs::OpenGL::Integer( bo.targetBinding() ) )
{
    KVS_ASSERT( bo.isCreated() );
    if ( bo.id() != static_cast<GLuint>( m_id ) ) { bo.bind(); }
}

/*===========================================================================*/
/**
 *  @brief  Destroy the GuardedBinder class.
 */
/*===========================================================================*/
BufferObject::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_bo.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_bo.id() )
    {
        KVS_GL_CALL( glBindBuffer( m_bo.target(), m_id ) );
    }
}

} // end of namespace kvs
