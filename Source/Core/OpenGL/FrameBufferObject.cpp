/*****************************************************************************/
/**
 *  @file   FrameBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrameBufferObject.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "FrameBufferObject.h"
#include <kvs/Texture2D>
#include <kvs/Assert>
#include <kvs/OpenGL>
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new FrameBufferObject class.
 */
/*===========================================================================*/
FrameBufferObject::FrameBufferObject():
    m_id( 0 )
{
}

/*===========================================================================*/
/**
 *  Destroy the FrameBufferObject class.
 */
/*===========================================================================*/
FrameBufferObject::~FrameBufferObject()
{
    this->release();
}

/*===========================================================================*/
/**
 *  Return object ID.
 *  @return object ID
 */
/*===========================================================================*/
GLuint FrameBufferObject::id() const
{
    return m_id;
}

/*===========================================================================*/
/**
 *  Create frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::create()
{
    this->createID();
}

/*===========================================================================*/
/**
 *  Release frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::release()
{
    this->deleteID();
}

/*===========================================================================*/
/**
 *  Bind frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glBindFramebuffer( GL_FRAMEBUFFER, m_id ) );
}

/*===========================================================================*/
/**
 *  Disable frame buffer object.
 */
/*===========================================================================*/
void FrameBufferObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}

bool FrameBufferObject::isCreated() const
{
    return m_id > 0;
}

bool FrameBufferObject::isValid() const
{
    GLboolean result;
    KVS_GL_CALL( result = glIsFramebuffer( m_id ) );
    return result == GL_TRUE;
}

bool FrameBufferObject::isBound() const
{
    if ( !this->isCreated() ) return false;

    GLint id = kvs::OpenGL::Integer( GL_FRAMEBUFFER_BINDING );
    return static_cast<GLuint>( id ) == m_id;
}

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

void FrameBufferObject::attachColorTexture( const kvs::Texture1D& texture, const size_t color_buffer, const int mip_level ) const
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
 *  Attach color texture.
 *  @param  texture [in] 2D texture
 *  @param  color_buffer [in] color buffer
 *  @param  mip_level [in] mip level
 */
/*===========================================================================*/
void FrameBufferObject::attachColorTexture( const kvs::Texture2D& texture, const size_t color_buffer, const int mip_level ) const
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
 *  Attach color texture.
 *  @param  texture [in] 3D texture
 *  @param  color_buffer [in] color buffer
 *  @param  mip_level [in] mip level
 *  @param  zoffset [in] offset
 */
/*===========================================================================*/
void FrameBufferObject::attachColorTexture( const kvs::Texture3D& texture, const size_t color_buffer, const int mip_level, const int zoffset ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_3D;
    KVS_GL_CALL( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment, type, id, mip_level, zoffset ) );
}

void FrameBufferObject::attachDepthTexture( const kvs::Texture1D& texture, const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_1D;
    KVS_GL_CALL( glFramebufferTexture1D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

void FrameBufferObject::attachDepthTexture( const kvs::Texture2D& texture, const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, id, mip_level ) );
}

void FrameBufferObject::attachDepthTexture( const kvs::Texture3D& texture, const int mip_level, const int zoffset ) const
{
    GuardedBinder binder( *this );
    const GLuint id = texture.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_3D;
    KVS_GL_CALL( glFramebufferTexture3D( GL_FRAMEBUFFER, attachment, type, id, mip_level, zoffset ) );
}

void FrameBufferObject::attachColorRenderBuffer( const kvs::RenderBuffer& render_buffer, const size_t color_buffer ) const
{
    KVS_ASSERT( static_cast<GLint>( color_buffer ) < kvs::OpenGL::MaxColorAttachments() );
    GuardedBinder binder( *this );
    const GLuint id = render_buffer.id();
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id ) );
}

void FrameBufferObject::attachDepthRenderBuffer( const kvs::RenderBuffer& render_buffer ) const
{
    GuardedBinder binder( *this );
    const GLuint id = render_buffer.id();
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id ) );
}

void FrameBufferObject::detachColorTexture( const size_t color_buffer, const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, 0, mip_level ) );
}

void FrameBufferObject::detachDepthTexture( const int mip_level ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    const GLenum type = GL_TEXTURE_2D;
    KVS_GL_CALL( glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, type, 0, mip_level ) );
}

void FrameBufferObject::detachColorRenderBuffer( const size_t color_buffer ) const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + color_buffer;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0 ) );
}

void FrameBufferObject::detachDepthRenderBuffer() const
{
    GuardedBinder binder( *this );
    const GLenum attachment = GL_DEPTH_ATTACHMENT;
    KVS_GL_CALL( glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0 ) );
}

void FrameBufferObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( glGenFramebuffers( 1, &m_id ) );
    }
}

void FrameBufferObject::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteFramebuffers( 1, &m_id ) );
    }
    m_id = 0;
}

GLenum FrameBufferObject::checkFramebufferStatus() const
{
    GuardedBinder binder( *this );
    GLenum status = GL_FRAMEBUFFER_COMPLETE;
    KVS_GL_CALL( status = glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
    return status;
}

FrameBufferObject::Binder::Binder( const FrameBufferObject& fbo ) :
    m_fbo( fbo )
{
    KVS_ASSERT( fbo.isCreated() );
    fbo.bind();
}

FrameBufferObject::Binder::~Binder()
{
    KVS_ASSERT( m_fbo.isCreated() );
    KVS_GL_CALL( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}

FrameBufferObject::GuardedBinder::GuardedBinder( const kvs::FrameBufferObject& fbo ):
    m_fbo( fbo ),
    m_id( 0 )
{
    KVS_ASSERT( fbo.isCreated() );
    m_id = kvs::OpenGL::Integer( GL_FRAMEBUFFER_BINDING );
    if ( fbo.id() != static_cast<GLuint>( m_id ) ) { fbo.bind(); }
}

FrameBufferObject::GuardedBinder::~GuardedBinder()
{
    KVS_ASSERT( m_fbo.isCreated() );
    if ( static_cast<GLuint>( m_id ) != m_fbo.id() )
    {
        KVS_GL_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER, m_id ) );
    }
}

} // end of namespace kvs
