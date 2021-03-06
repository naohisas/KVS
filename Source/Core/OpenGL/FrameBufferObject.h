/****************************************************************************/
/**
 *  @file   FrameBufferObject.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
 *  @brief  Frame buffer object class.
 */
/*===========================================================================*/
class FrameBufferObject
{
private:
    GLuint m_id = 0; ///< object ID

public:
    class Binder;
    class GuardedBinder;

private:
    static GLuint m_unbind_id; ///< Initial frame buffer id.

public:
    static void SetUnbindID( const GLuint id ) { m_unbind_id = id; }
    static GLuint UnbindID() { return m_unbind_id; }

public:
    FrameBufferObject() = default;
    virtual ~FrameBufferObject() { this->release(); }

    GLuint id() const { return m_id; }
    void create() { this->createID(); }
    void release() { this->deleteID(); }
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
};

/*===========================================================================*/
/**
 *  @brief  Binder class for FrameBufferObject.
 */
/*===========================================================================*/
class FrameBufferObject::Binder
{
    const kvs::FrameBufferObject& m_fbo; ///< target frame buffer object
public:
    Binder( const kvs::FrameBufferObject& fbo ): m_fbo( fbo ) { m_fbo.bind(); }
    ~Binder() { m_fbo.unbind(); }
    Binder( const Binder& ) = delete;
    Binder& operator =( const Binder& ) = delete;
};

/*===========================================================================*/
/**
 *  @brief  Guarded binder class for FrameBufferObject.
 */
/*===========================================================================*/
class FrameBufferObject::GuardedBinder
{
    const kvs::FrameBufferObject& m_fbo; ///< target frame buffer object
    GLint m_id = 0; ///< target binding ID
public:
    GuardedBinder( const kvs::FrameBufferObject& fbo );
    ~GuardedBinder();
    GuardedBinder( const GuardedBinder& ) = delete;
    GuardedBinder& operator =( const GuardedBinder& ) = delete;
};

} // end of namespace kvs
