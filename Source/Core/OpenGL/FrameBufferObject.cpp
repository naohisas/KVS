/*****************************************************************************/
/**
 *  @file   FrameBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "FrameBufferObject.h"
#include <kvs/Texture2D>
#include <kvs/Assert>
#include <kvs/OpenGL>
#include <kvs/Message>

GLuint kvs::FrameBufferObject::m_unbind_id = 0; // Initialize static unbind id


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Bind frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindFramebuffer( GL_FRAMEBUFFER, m_id ) );
}

/*===========================================================================*/
/**
 *  @brief  Disable frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindFramebuffer( GL_FRAMEBUFFER, m_unbind_id ) );
}

/*===========================================================================*/
/**
 *  @brief  Determins if a frame buffer object is created.
 *  @return true if the frame buffer object has been already created
 */
/*===========================================================================*/
bool FrameBufferObject::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determins if a frame buffer object is valid.
 *  @return true if the frame buffer object has been already allocated
 */
/*===========================================================================*/
bool FrameBufferObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsFramebuffer( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if a frame buffer object is bound.
 *  @return true if the frame buffer object has been already bound
 */
/*===========================================================================*/
bool FrameBufferObject::isBound() const
{
    if ( !this->isCreated() ) { return false; }
    GLint id = kvs::OpenGL::Integer( GL_FRAMEBUFFER_BINDING );
    return static_cast<GLuint>( id ) == m_id;
}

/*===========================================================================*/
/**
 *  @brief  Check the completness status of a frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::checkStatus() const
{
    const GLenum status = this->checkFramebufferStatus();
    if ( status == GL_FRAMEBUFFER_COMPLETE ) { return; }

    switch ( status )
    {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        kvsMessageError( "Framebuffer incomplete attachement" );
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        kvsMessageError( "Framebuffer incomplete, missing attachment" );
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        kvsMessageError( "Framebuffer incomplete, missing draw buffer" );
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        kvsMessageError( "Framebuffer incomplete, missing read buffer" );
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        kvsMessageError( "Unsupported framebuffer format" );
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        kvsMessageError( "Framebuffer incomplete multisample" );
        break;
    case GL_FRAMEBUFFER_UNDEFINED:
        kvsMessageError( "Framebuffer undefined" );
        break;
    default:
        kvsMessageError( "Unknown framebuffer error" );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Attaches 1D color texture.
 *  @param  texture [in] 1D texture
 *  @param  color_buffer [in] attachment point (0 to GL_MAX_COLOR_ATTACHMENTS)
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::attachColorTexture(
    const kvs::Texture1D& texture,
    const size_t color_buffer,
    const int mip_level ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_1D;
    KVS_GL_CALL( glFramebufferTexture1D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches 2D color texture.
 *  @param  texture [in] 2D texture
 *  @param  color_buffer [in] attachment point (zero to GL_MAX_COLOR_ATTACHMENTS)
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::attachColorTexture(
    const kvs::Texture2D& texture,
    const size_t color_buffer,
    const int mip_level ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches 3D color texture.
 *  @param  texture [in] 3D texture
 *  @param  color_buffer [in] attachment point (zero to GL_MAX_COLOR_ATTACHMENTS)
 *  @param  mip_level [in] mip level
 *  @param  zoffset [in] offset along z-axis of the 3D texture
 */
/*===========================================================================*/
void FrameBufferObject::attachColorTexture(
    const kvs::Texture3D& texture,
    const size_t color_buffer,
    const int mip_level,
    const int zoffset ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_3D;
    KVS_GL_CALL( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment, type, id, mip_level, zoffset ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches 1D depth texture.
 *  @param  texture [in] 1D texture
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::attachDepthTexture(
    const kvs::Texture1D& texture,
    const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_1D;
    KVS_GL_CALL( glFramebufferTexture1D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches 2D depth texture.
 *  @param  texture [in] 2D texture
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::attachDepthTexture(
    const kvs::Texture2D& texture,
    const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches 3D depth texture.
 *  @param  texture [in] 3D texture
 *  @param  mip_level [in] mip level
 *  @param  zoffset [in] offset along z-axis of the 3D texture
 */
/*===========================================================================*/
void FrameBufferObject::attachDepthTexture(
    const kvs::Texture3D& texture,
    const int mip_level,
    const int zoffset ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_3D;
    KVS_GL_CALL( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment, type, id, mip_level, zoffset ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches color render buffer.
 *  @param  render_buffer [in] render buffer
 *  @param  color_buffer [in] attachment point (zero to GL_MAX_COLOR_ATTACHMENTS)
 */
/*===========================================================================*/
void FrameBufferObject::attachColorRenderBuffer(
    const kvs::RenderBuffer& render_buffer,
    const size_t color_buffer ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = render_buffer.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches depth render buffer.
 *  @param  render_buffer [in] render buffer
 */
/*===========================================================================*/
void FrameBufferObject::attachDepthRenderBuffer(
    const kvs::RenderBuffer& render_buffer ) const
{
    GuardedBinder binder( *this );
    const GLuint id = render_buffer.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id ) );
}

/*===========================================================================*/
/**
 *  @brief  Dettaches 2D color texture.
 *  @param  color_buffer [in] attachment point (zero to GL_MAX_COLOR_ATTACHMENTS)
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::detachColorTexture(
    const size_t color_buffer,
    const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, 0, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Dettaches 2D depth texture.
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::detachDepthTexture( const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, 0, mip_level ) );
}

/*===========================================================================*/
/**
 *  @brief  Dettaches color render buffer.
 *  @param  color_buffer [in] attachment point (zero to GL_MAX_COLOR_ATTACHMENTS)
 */
/*===========================================================================*/
void FrameBufferObject::detachColorRenderBuffer( const size_t color_buffer ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Dettaches depth render buffer.
 */
/*===========================================================================*/
void FrameBufferObject::detachDepthRenderBuffer() const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Creates frame bufffer object ID.
 */
/*===========================================================================*/
void FrameBufferObject::createID()
{
//    if ( !this->isValid() )
    if ( !this->isCreated() )
    {
        KVS_GL_CALL( glGenFramebuffers( 1, &m_id ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes frame buffer object ID.
 */
/*===========================================================================*/
void FrameBufferObject::deleteID()
{
//    if ( this->isValid() )
    if ( this->isCreated() )
    {
        if ( this->isBound() ) { this->unbind(); }
        KVS_GL_CALL( glDeleteFramebuffers( 1, &m_id ) );
        m_id = 0;
    }
}

/*===========================================================================*/
/**
 *  @brief  Check the completness status of a frame buffer object.
 *  @return status of the frame buffer object
 */
/*===========================================================================*/
GLenum FrameBufferObject::checkFramebufferStatus() const
{
    GuardedBinder binder( *this );
    GLenum status = GL_FRAMEBUFFER_COMPLETE;
    KVS_GL_CALL( status = glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
    return status;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GuardedBinder class.
 *  @param  fbo [in] target frame buffer object that will be bound
 */
/*===========================================================================*/
FrameBufferObject::GuardedBinder::GuardedBinder( const kvs::FrameBufferObject& fbo ):
    m_fbo( fbo )
{
    KVS_ASSERT( fbo.isCreated() );
    m_id = kvs::OpenGL::Integer( GL_FRAMEBUFFER_BINDING );
    if ( fbo.id() != static_cast<GLuint>( m_id ) ) { fbo.bind(); }
}

/*===========================================================================*/
/**
 *  @brief  Destroy the GuardedBinder class.
 */
/*===========================================================================*/
FrameBufferObject::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_fbo.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_fbo.id() )
    {
        KVS_GL_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER, m_id ) );
    }
}

} // end of namespace kvs
