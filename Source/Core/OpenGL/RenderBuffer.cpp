/*****************************************************************************/
/**
 *  @file   RenderBuffer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "RenderBuffer.h"
#include <kvs/OpenGL>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Creates a render buffer.
 *  @param  width [in] buffer width
 *  @param  height [in] buffer height
 */
/*===========================================================================*/
void RenderBuffer::create( const size_t width, const size_t height )
{
    KVS_ASSERT( m_id == 0 );

    m_width = width;
    m_height = height;
    this->createID();
    GuardedBinder binder( *this );
    this->setRenderbufferStorage( m_internal_format, m_width, m_height );
}

/*===========================================================================*/
/**
 *  @brief  Release render buffer.
 */
/*===========================================================================*/
void RenderBuffer::release()
{
    this->deleteID();
    m_width = 0;
    m_height = 0;
}

/*===========================================================================*/
/**
 *  @brief  Binds render buffer.
 */
/*===========================================================================*/
void RenderBuffer::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindRenderbuffer( GL_RENDERBUFFER, m_id ) );
}

/*===========================================================================*/
/**
 *  @brief  Unbinds render buffer.
 */
/*===========================================================================*/
void RenderBuffer::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindRenderbuffer( GL_RENDERBUFFER, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Determins if a render buffer is created.
 *  @return true if the render buffer has been already created
 */
/*===========================================================================*/
bool RenderBuffer::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determins if a render buffer is valid.
 *  @return true if the render buffer has been already allocated
 */
/*===========================================================================*/
bool RenderBuffer::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsRenderbuffer( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if a render buffer is bound.
 *  @return true if the render buffer has been already bound
 */
/*===========================================================================*/
bool RenderBuffer::isBound() const
{
    if ( this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( GL_RENDERBUFFER_BINDING );
    return static_cast<GLuint>( id ) == m_id;
}

/*===========================================================================*/
/**
 *  @brief  Creates render buffer ID.
 */
/*===========================================================================*/
void RenderBuffer::createID()
{
//    if ( !this->isValid() )
    if ( !this->isCreated() )
    {
        KVS_GL_CALL( glGenRenderbuffers( 1, &m_id ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes render buffer ID.
 */
/*===========================================================================*/
void RenderBuffer::deleteID()
{
//    if ( this->isValid() )
    if ( this->isCreated() )
    {
        if ( this->isBound() ) { this->unbind(); }
        KVS_GL_CALL( glDeleteRenderbuffers( 1, &m_id ) );
        m_id = 0;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets data storage for render buffer.
 *  @param  internal_format [in] internal format to use for the render buffer
 *  @param  width [in] buffer width
 *  @param  height [in] buffer height
 */
/*===========================================================================*/
void RenderBuffer::setRenderbufferStorage( GLenum internal_format, GLsizei width, GLsizei height )
{
    KVS_ASSERT( width > 0 );
    KVS_ASSERT( width <= kvs::OpenGL::MaxRenderBufferSize() );
    KVS_ASSERT( height > 0 );
    KVS_ASSERT( height <= kvs::OpenGL::MaxRenderBufferSize() );
    KVS_GL_CALL( glRenderbufferStorage( GL_RENDERBUFFER, internal_format, width, height ) );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GuardedBinder class.
 *  @param  rb [in] target render buffer that will be bound
 */
/*===========================================================================*/
RenderBuffer::GuardedBinder::GuardedBinder( const kvs::RenderBuffer& rb ):
    m_rb( rb )
{
    m_id = kvs::OpenGL::Integer( GL_RENDERBUFFER_BINDING );
    if ( m_rb.id() != static_cast<GLuint>( m_id ) ) { m_rb.bind(); }
}

/*===========================================================================*/
/**
 *  @brief  Destroy the GuardedBinder class.
 */
/*===========================================================================*/
RenderBuffer::GuardedBinder::~GuardedBinder()
{
    if ( static_cast<GLuint>( m_id ) != m_rb.id() )
    {
        KVS_GL_CALL( glBindRenderbuffer( GL_RENDERBUFFER, m_id ) );
    }
}

} // end of namespace kvs
