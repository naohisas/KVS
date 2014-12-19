/*****************************************************************************/
/**
 *  @file   EnsembleAverageBuffer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EnsembleAverageBuffer.h 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__ENSEMBLE_AVERAGE_BUFFER_H_INCLUDE
#define KVS__ENSEMBLE_AVERAGE_BUFFER_H_INCLUDE

#include <kvs/Texture2D>
#include <kvs/FrameBufferObject>


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

#endif // KVS__ENSEMBLE_AVERAGE_BUFFER_H_INCLUDE
