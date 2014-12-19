/****************************************************************************/
/**
 *  @file   ParticleBufferAccumulator.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBufferAccumulator.h 1642 2013-09-15 12:41:47Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PARTICLE_BUFFER_ACCUMULATOR_H_INCLUDE
#define KVS__PARTICLE_BUFFER_ACCUMULATOR_H_INCLUDE

#include <kvs/PointObject>
#include <kvs/ParticleBuffer>
#include <kvs/ParticleVolumeRenderer>
#include <kvs/ValueArray>
#include <vector>


namespace kvs
{

/*==========================================================================*/
/**
 *  Particle buffer accumulator.
 */
/*==========================================================================*/
class ParticleBufferAccumulator : public kvs::ParticleBuffer
{
public:

    typedef kvs::ParticleBuffer SuperClass;
    typedef std::vector<kvs::PointObject*> ObjectList;
    typedef std::vector<kvs::ParticleVolumeRenderer*> RendererList;

private:

    kvs::ValueArray<kvs::UInt8> m_id_buffer; ///< object ID buffer [0,255]

public:

    ParticleBufferAccumulator(
        const size_t width,
        const size_t height,
        const size_t subpixel_level );
    virtual ~ParticleBufferAccumulator();

    bool create( const size_t width, const size_t height, const size_t subpixel_level );
    void clean();
    void clear();
    void accumulate( const size_t id, const kvs::ParticleBuffer* buffer );
    void add( const size_t bindex, const size_t id, const kvs::Real32 depth, const kvs::UInt32 vindex );
    void createImage(
        ObjectList& object_list,
        RendererList& renderer_list,
        kvs::ValueArray<kvs::UInt8>* color,
        kvs::ValueArray<kvs::Real32>* depth );
};

} // end of namespace kvs

#endif // KVS__PARTICLE_BUFFER_ACCUMULATOR_H_INCLUDE
