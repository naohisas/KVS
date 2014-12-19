/*****************************************************************************/
/**
 *  @file   StochasticRenderingEngine.h
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__STOCHASTIC_RENDERING_ENGINE_H_INCLUDE
#define KVS__STOCHASTIC_RENDERING_ENGINE_H_INCLUDE

#include <kvs/Shader>
#include <kvs/Texture2D>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  StochasticRenderingEngine class.
 */
/*===========================================================================*/
class StochasticRenderingEngine
{
    friend class StochasticRendererBase;
    friend class StochasticRenderingCompositor;
    /*DEPRECATED*/ friend class StochasticMultipleTetrahedraCompositor;

private:

    const kvs::ObjectBase* m_object; ///< pointer to the object
    const kvs::Shader::ShadingModel* m_shader; ///< pointer to the shader
    bool m_enable_shading; ///< shading flag
    size_t m_repetition_level; ///< repetition level
    size_t m_repetition_count; ///< repetition count
    size_t m_random_texture_size; ///< size of the random texture
    kvs::Texture2D m_random_texture; ///< random number texture
    kvs::Texture2D m_depth_texture; ///< depth texture

public:

    StochasticRenderingEngine();
    virtual ~StochasticRenderingEngine(){}

    void setShader( const kvs::Shader::ShadingModel* shader ) { m_shader = shader; }
    void setEnabledShading( const bool enable ) { m_enable_shading = enable; }
    void setRepetitionLevel( const size_t repetition_level ) { m_repetition_level = repetition_level; }
    void setRandomTextureSize( const size_t size ) { m_random_texture_size = size; }
    void setDepthTexture( const kvs::Texture2D& depth_texture ) { m_depth_texture = depth_texture; }
    bool isEnabledShading() const { return m_enable_shading; }
    size_t repetitionLevel() const { return m_repetition_level; }
    size_t repetitionCount() const { return m_repetition_count; }
    size_t randomTextureSize() const { return m_random_texture_size; }
    const kvs::Texture2D& randomTexture() const { return m_random_texture; }
    const kvs::Texture2D& depthTexture() const { return m_depth_texture; }
    const kvs::ObjectBase* object() const { return m_object; }

    virtual void release() = 0;
    virtual void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light ) = 0;
    virtual void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light ) = 0;
    virtual void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light ) = 0;
    virtual void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light ) = 0;

protected:

    const kvs::Shader::ShadingModel& shader() const { return *m_shader; }
    void resetRepetitions() { m_repetition_count = 0; }
    void countRepetitions() { m_repetition_count++; }
    void attachObject( const kvs::ObjectBase* object ) { m_object = object; }
    void createRandomTexture();
};

} // end of namespace kvs

#endif // KVS__STOCHASTIC_RENDERING_ENGINE_H_INCLUDE
