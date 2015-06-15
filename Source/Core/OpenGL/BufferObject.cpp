/*****************************************************************************/
/**
 *  @file   BufferObject.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BufferObject.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "BufferObject.h"
#include <kvs/Assert>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  Constructs a new BufferObject class.
 *  @param  target [in] bind target
 *  @param  target_binding [in] target binding
 *  @param  usage [in] buffer usage
 */
/*===========================================================================*/
BufferObject::BufferObject( const GLenum target, const GLenum target_binding, const GLenum usage ):
    m_target( target ),
    m_target_binding( target_binding ),
    m_id( 0 ),
    m_usage( usage ),
    m_size( 0 ),
    m_is_loaded( false )
{
}

/*===========================================================================*/
/**
 *  Destroys the BufferObject class.
 */
/*===========================================================================*/
BufferObject::~BufferObject()
{
    this->release();
}

GLenum BufferObject::target() const
{
    return m_target;
}

GLenum BufferObject::targetBinding() const
{
    return m_target_binding;
}

/*===========================================================================*/
/**
 *  Returns the buffer ID.
 *  @return buffer ID
 */
/*===========================================================================*/
GLuint BufferObject::id() const
{
    return m_id;
}

/*===========================================================================*/
/**
 *  Returns the buffer size.
 *  @return buufer size
 */
/*===========================================================================*/
size_t BufferObject::size() const
{
    return m_size;
}

/*===========================================================================*/
/**
 *  Sets buffer usage.
 *  @param  usage [in] buffer usage
 */
/*===========================================================================*/
void BufferObject::setUsage( const GLenum usage )
{
    m_usage = usage;
}

void BufferObject::setSize( const size_t size )
{
    m_size = size;
}

/*===========================================================================*/
/**
 *  Create buffer.
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
 *  Release buffer.
 */
/*===========================================================================*/
void BufferObject::release()
{
    this->deleteID();
    m_is_loaded = false;
}

/*===========================================================================*/
/**
 *  Bind buffer.
 */
/*===========================================================================*/
void BufferObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindBuffer( m_target, m_id ) );
}

/*===========================================================================*/
/**
 *  Unbind buffer.
 */
/*===========================================================================*/
void BufferObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindBuffer( m_target, 0 ) );
}

bool BufferObject::isCreated() const
{
    return m_id > 0;
}

bool BufferObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsBuffer( m_id ) );
    return result == GL_TRUE;
}

bool BufferObject::isBound() const
{
    if ( !this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( m_target_binding );
    return static_cast<GLuint>( id ) == m_id;
}

/*===========================================================================*/
/**
 *  Load buffer data from CPU to GPU.
 *  @param  size [in] buffer data size
 *  @param  data [in] pointer to loaded buffer data
 *  @param  offset [in] texel offset within the existing buffer data array
 */
/*===========================================================================*/
void BufferObject::load( const size_t size, const void* data, const size_t offset )
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
}

/*===========================================================================*/
/**
 *  Map buffer data.
 *  @param  access_type [in] access policy
 *  @return NULL if an error is generated
 */
/*===========================================================================*/
void* BufferObject::map( const GLenum access_type )
{
    return this->mapBuffer( access_type );
}

/*===========================================================================*/
/**
 *  Unmap buffer object data.
 */
/*===========================================================================*/
void BufferObject::unmap()
{
    this->unmapBuffer();
}

void BufferObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( glGenBuffers( 1, &m_id ) );
    }
}

void BufferObject::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteBuffers( 1, &m_id ) );
    }
    m_id = 0;
}

void BufferObject::setBufferData( GLsizei size, const GLvoid* data )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBufferData( m_target, size, data, m_usage ) );
}

void BufferObject::setBufferSubData( GLsizei size, const GLvoid* data, GLint offset )
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBufferSubData( m_target, offset, size, data ) );
}

void* BufferObject::mapBuffer( const GLenum access_type )
{
    KVS_ASSERT( this->isBound() );
    void* result = 0;
    KVS_GL_CALL( result = glMapBuffer( m_target, access_type ) );
    return result;
}

void BufferObject::unmapBuffer()
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glUnmapBuffer( m_target ) );
}

BufferObject::Binder::Binder( const BufferObject& bo ) :
    m_bo( bo )
{
    KVS_ASSERT( bo.isCreated() );
    bo.bind();
}

BufferObject::Binder::~Binder()
{
    KVS_ASSERT( m_bo.isCreated() );
    KVS_GL_CALL( glBindBuffer( m_bo.target(), 0 ) );
}

BufferObject::GuardedBinder::GuardedBinder( const kvs::BufferObject& bo ):
    m_bo( bo ),
    m_id( 0 )
{
    KVS_ASSERT( bo.isCreated() );
    m_id = kvs::OpenGL::Integer( bo.targetBinding() );
    if ( bo.id() != static_cast<GLuint>( m_id ) ) { bo.bind(); }
}

BufferObject::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_bo.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_bo.id() )
    {
        KVS_GL_CALL( glBindBuffer( m_bo.target(), m_id ) );
    }
}

} // end of namespace kvs
