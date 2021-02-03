/*****************************************************************************/
/**
 *  @file   StochasticRendererBase.h
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Shader>
#include <kvs/Matrix44>
#include <kvs/Module>
#include <kvs/Deprecated>
#include "EnsembleAverageBuffer.h"
#include "StochasticRenderingEngine.h"


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Stochastic renderer base class.
 */
/*===========================================================================*/
class StochasticRendererBase : public kvs::RendererBase
{
    kvsModule( kvs::StochasticRendererBase, Renderer );

    friend class StochasticRenderingCompositor;

private:
    size_t m_window_width; ///< window width
    size_t m_window_height; ///< window height
    float m_device_pixel_ratio; ///< device pixel ratio
    size_t m_repetition_level; ///< repetition level
    size_t m_coarse_level; ///< repetition level for the coarse rendering (LOD)
    bool m_enable_lod; ///< flag for LOD rendering
    bool m_enable_refinement; ///< flag for progressive refinement rendering
    kvs::Mat4 m_modelview; ///< modelview matrix used for LOD control
    kvs::Vec3 m_light_position; ///< light position used for LOD control
    kvs::EnsembleAverageBuffer m_ensemble_buffer; ///< ensemble averaging buffer
    kvs::Shader::ShadingModel* m_shader; ///< shading method
    kvs::StochasticRenderingEngine* m_engine; ///< rendering engine

public:
    StochasticRendererBase( kvs::StochasticRenderingEngine* engine );
    virtual ~StochasticRendererBase();

    virtual void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void release();
    size_t windowWidth() const { return m_window_width; }
    size_t windowHeight() const { return m_window_height; }
    size_t framebufferWidth() const { return static_cast<size_t>( m_window_width * m_device_pixel_ratio ); }
    size_t framebufferHeight() const { return static_cast<size_t>( m_window_height * m_device_pixel_ratio ); }
    float devicePixelRatio() const { return m_device_pixel_ratio; }
    size_t repetitionLevel() const { return m_repetition_level; }

    void setWindowSize( const size_t width, const size_t height ) { m_window_width = width; m_window_height = height; }
    void setDevicePixelRatio( const float dpr ) { m_device_pixel_ratio = dpr; }
    void setRepetitionLevel( const size_t repetition_level ) { m_repetition_level = repetition_level; }

    void setLODControlEnabled( const bool enable = true ) { m_enable_lod = enable; }
    bool isLODControlEnabled() const { return m_enable_lod; }
    void enableLODControl() { this->setLODControlEnabled( true ); }
    void disableLODControl() { this->setLODControlEnabled( false ); }

    void setRefinementEnabled( const bool enable = true ) { m_enable_refinement = enable; }
    bool isRefinementEnabled() const { return m_enable_refinement; }
    void enableRefinement() { this->setRefinementEnabled( true ); }
    void disableRefinement() { this->setRefinementEnabled( false ); }

    const kvs::Shader::ShadingModel& shader() const { return *m_shader; }
    const kvs::StochasticRenderingEngine& engine() const { return *m_engine; }
    template <typename ShadingType>
    void setShader( const ShadingType shader );

protected:
    kvs::Shader::ShadingModel& shader() { return *m_shader; }
    kvs::StochasticRenderingEngine& engine() { return *m_engine; }

public:
    KVS_DEPRECATED( void setEnabledLODControl( const bool enable ) ) { this->setLODControlEnabled( enable ); }
    KVS_DEPRECATED( bool isEnabledLODControl() const ) { return this->isLODControlEnabled(); }
    KVS_DEPRECATED( void setEnabledRefinement( const bool enable ) ) { this->setRefinementEnabled( enable ); }
    KVS_DEPRECATED( bool isEnabledRefinement() const ) { return this->isRefinementEnabled(); }
};

template <typename ShadingType>
inline void StochasticRendererBase::setShader( const ShadingType shader )
{
    if ( m_shader ) { delete m_shader; }
    m_shader = new ShadingType( shader );
}

} // end of namespace kvs
