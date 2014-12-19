/****************************************************************************/
/**
 *  @file   FrameBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrameBufferObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__FRAME_BUFFER_OBJECT_H_INCLUDE
#define KVS__FRAME_BUFFER_OBJECT_H_INCLUDE

#include <string>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/Texture3D>
#include <kvs/RenderBuffer>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  Frame buffer object class.
 */
/*===========================================================================*/
class FrameBufferObject
{
private:

    GLuint m_id; ///< object ID

public:

    class Binder;
    class GuardedBinder;

public:

    FrameBufferObject();
    virtual ~FrameBufferObject();

    GLuint id() const;

    void create();
    void release();
    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;
    void checkStatus() const;

    void attachColorTexture( const kvs::Texture1D& texture, const size_t color_buffer = 0, const int mip_level = 0 ) const;
    void attachColorTexture( const kvs::Texture2D& texture, const size_t color_buffer = 0, const int mip_level = 0 ) const;
    void attachColorTexture( const kvs::Texture3D& texture, const size_t color_buffer = 0, const int mip_level = 0, const int zoffset = 0 ) const;
    void attachDepthTexture( const kvs::Texture1D& texture, const int mip_level = 0 ) const;
    void attachDepthTexture( const kvs::Texture2D& texture, const int mip_level = 0 ) const;
    void attachDepthTexture( const kvs::Texture3D& texture, const int mip_level = 0, const int zoffset = 0 ) const;
    void attachColorRenderBuffer( const kvs::RenderBuffer& render_buffer, const size_t color_buffer = 0 ) const;
    void attachDepthRenderBuffer( const kvs::RenderBuffer& render_buffer ) const;

    void detachColorTexture( const size_t color_buffer = 0, const int mip_level = 0 ) const;
    void detachDepthTexture( const int mip_level = 0 ) const;
    void detachColorRenderBuffer( const size_t color_buffer = 0 ) const;
    void detachDepthRenderBuffer() const;

protected:

    void createID();
    void deleteID();
    GLenum checkFramebufferStatus() const;

public:

    KVS_DEPRECATED( void disable() const ) { this->unbind(); }
};

class FrameBufferObject::Binder
{
    const kvs::FrameBufferObject& m_fbo;

public:

    Binder( const kvs::FrameBufferObject& fbo );
    ~Binder();

private:

    Binder( const Binder& );
    Binder& operator =( const Binder& );
};

class FrameBufferObject::GuardedBinder
{
    const kvs::FrameBufferObject& m_fbo;
    GLint m_id;

public:

    GuardedBinder( const kvs::FrameBufferObject& fbo );
    ~GuardedBinder();

private:

    GuardedBinder( const GuardedBinder& );
    GuardedBinder& operator =( const GuardedBinder& );
};

} // end of namespace kvs

#endif // KVS__FRAME_BUFFER_OBJECT_H_INCLUDE
