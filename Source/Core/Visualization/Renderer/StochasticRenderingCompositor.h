/*****************************************************************************/
/**
 *  @file   StochasticRenderingCompositor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Timer>
#include <kvs/TrackballInteractor>
#include <kvs/Matrix44>
#include <kvs/Vector3>
#include <kvs/Deprecated>
#include "EnsembleAverageBuffer.h"


namespace kvs
{

class Scene;

/*===========================================================================*/
/**
 *  @brief  Stochastic rendering compositor class.
 */
/*===========================================================================*/
class StochasticRenderingCompositor : public kvs::TrackballInteractor
{
private:
    kvs::Timer m_timer;
    kvs::Scene* m_scene; ///< pointer to the scene
    size_t m_window_width; ///< window width
    size_t m_window_height; ///< window height
    size_t m_repetition_level; ///< repetition level
    size_t m_coarse_level; ///< repetition level for the coarse rendering (LOD)
    bool m_enable_lod; ///< flag for LOD rendering
    bool m_enable_refinement; ///< flag for progressive refinement rendering
    kvs::Mat4 m_object_xform; ///< object xform matrix used for LOD control
    kvs::Vec3 m_light_position; ///< light position used for LOD control
    kvs::Vec3 m_camera_position; ///< camera position used for LOD control
    kvs::EnsembleAverageBuffer m_ensemble_buffer; ///< ensemble averaging buffer

public:
    StochasticRenderingCompositor( kvs::Scene* scene );
    virtual ~StochasticRenderingCompositor() {}
    const kvs::Timer& timer() const { return m_timer; }
    size_t repetitionLevel() const { return m_repetition_level; }
    bool isLODControlEnabled() const { return m_enable_lod; }
    bool isRefinementEnabled() const { return m_enable_refinement; }
    void setRepetitionLevel( const size_t repetition_level ) { m_repetition_level = repetition_level; }
    void setLODControlEnabled( const bool enable = true ) { m_enable_lod = enable; }
    void setRefinementEnabled( const bool enable = true ) { m_enable_refinement = enable; }
    void enableLODControl() { this->setLODControlEnabled( true ); }
    void enableRefinement() { this->setRefinementEnabled( true ); }
    void disableLODControl() { this->setLODControlEnabled( false ); }
    void disableRefinement() { this->setRefinementEnabled( false ); }
    void update();

private:
    StochasticRenderingCompositor();
    void draw();
    void check_window_created();
    void check_window_resized();
    void check_object_changed();
    kvs::Mat4 object_xform();
    void engines_create();
    void engines_update();
    void engines_setup();
    void engines_draw();

private:
    void paintEvent() { this->update(); }

public:
    KVS_DEPRECATED( bool isEnabledLODControl() const ) { return this->isLODControlEnabled(); }
    KVS_DEPRECATED( bool isEnabledRefinement() const ) { return this->isRefinementEnabled(); }
    KVS_DEPRECATED( void setEnabledLODControl( const bool enable ) ) { this->setLODControlEnabled( enable ); }
    KVS_DEPRECATED( void setEnabledRefinement( const bool enable ) ) { this->setRefinementEnabled( enable ); }
};

} // end of namespace kvs
