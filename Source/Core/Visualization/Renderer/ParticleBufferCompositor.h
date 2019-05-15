/****************************************************************************/
/**
 *  @file   ParticleBufferCompositor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBufferCompositor.h 1641 2013-09-11 06:39:57Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PARTICLE_BUFFER_COMPOSITOR_H_INCLUDE
#define KVS__PARTICLE_BUFFER_COMPOSITOR_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/VolumeRendererBase>
#include <kvs/ParticleBufferAccumulator>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Particle buffer compositor.
 */
/*==========================================================================*/
class ParticleBufferCompositor : public kvs::VolumeRendererBase
{
public:

    typedef kvs::VolumeRendererBase BaseClass;
    typedef ParticleBufferAccumulator::ObjectList ObjectList;
    typedef ParticleBufferAccumulator::RendererList RendererList;

private:

    double m_accumulation_time; ///< accumulation time
    size_t m_num_projected_particles; ///< number of projected points
    size_t m_num_stored_particles; ///< number of stored points
    size_t m_subpixel_level; ///< subpixel level
    kvs::ObjectManager* m_object_manager; ///< pointer to the object manager
    kvs::RendererManager* m_renderer_manager; ///< pointer to the renderer manager
    kvs::IDManager* m_id_manager; ///< pointer to the ID manager
    ObjectList m_point_object_list; ///< object list
    RendererList m_point_renderer_list; ///< renderer list
    kvs::ParticleBufferAccumulator* m_accumulator; ///< pointer to the accumulator

public:

    ParticleBufferCompositor(
        kvs::ObjectManager* object_manager,
        kvs::RendererManager* renderer_manager,
        kvs::IDManager* id_manager );
    virtual ~ParticleBufferCompositor();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    size_t accumulationTime() const { return m_accumulation_time; }
    size_t numberOfProjectedParticles() const { return m_num_projected_particles; }
    size_t numberOfStoredParticles() const { return m_num_stored_particles; }
    void link( kvs::PointObject* object, kvs::ParticleVolumeRenderer* renderer );
    void clearList();

private:

    bool create_accumulator();
    void clean_accumulator();
    void delete_accumulator();
    void create_image( kvs::Camera* camera, kvs::Light* light );
    void accumulate( kvs::Camera* camera, kvs::Light* light );
    void update_particle_buffer(
        kvs::PointObject* object,
        kvs::ParticleVolumeRenderer* renderer,
        kvs::Camera* camera,
        kvs::Light* light );

public:
    KVS_DEPRECATED( size_t numOfProjectedParticles() const ) { return this->numberOfProjectedParticles(); }
    KVS_DEPRECATED( size_t numOfStoredParticles() const ) { return this->numberOfStoredParticles(); }
};

} // end of namespace kvs

#endif // KVS__PARTICLE_BUFFER_COMPOSITOR_H_INCLUDE
