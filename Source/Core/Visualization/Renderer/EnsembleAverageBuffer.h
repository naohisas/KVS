/*****************************************************************************/
/**
 *  @file   EnsembleAverageBuffer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Texture2D>
#include <kvs/FrameBufferObject>
#include <kvs/ProgramObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Ensemble averagin buffer class.
 */
/*===========================================================================*/
class EnsembleAverageBuffer
{
private:
    size_t m_count; ///< number of ensembles (repetitions)
    kvs::Texture2D m_current_color_texture; ///< current color buffer
    kvs::Texture2D m_current_depth_texture; ///< current depth buffer
    kvs::FrameBufferObject m_current_framebuffer; ///< current framebuffer
    kvs::Texture2D m_accum_texture; ///< accumulated color buffer
    kvs::FrameBufferObject m_accum_framebuffer; ///< accumulated framebuffer
    kvs::ProgramObject m_average_shader;
    kvs::ProgramObject m_drawing_shader;

public:
    const kvs::Texture2D& currentColorTexture() const { return m_current_color_texture; }
    const kvs::Texture2D& currentDepthTexture() const { return m_current_depth_texture; }
    const kvs::FrameBufferObject& currentFrameBufferObject() const { return m_current_framebuffer; }

    void create( const size_t width, const size_t height );
    void release();
    void clear();
    void bind();
    void unbind();
    void add();
    void draw();
};

} // end of namespace kvs
