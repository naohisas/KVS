/****************************************************************************/
/**
 *  @file   MetropolisSampling.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MetropolisSampling.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__METROPOLIS_SAMPLING_H_INCLUDE
#define KVS__METROPOLIS_SAMPLING_H_INCLUDE

#include <kvs/PointObject>
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Metropolis sampling class.
 */
/*==========================================================================*/
class MetropolisSampling : public MapperBase, public PointObject
{
    kvsModule( kvs::MetropolisSampling, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PointObject );

protected:

    size_t m_nparticles; ///< number of generated particles

public:

    MetropolisSampling();
    MetropolisSampling( const kvs::VolumeObjectBase* volume, const size_t nparticles );
    MetropolisSampling(
        const kvs::VolumeObjectBase* volume,
        const size_t nparticles,
        const kvs::TransferFunction& transfer_function );
    virtual ~MetropolisSampling();

    size_t nparticles() const;
    void setNParticles( const size_t nparticles );

    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void mapping( const kvs::StructuredVolumeObject* volume );
    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void generate_particles( const kvs::StructuredVolumeObject* volume );
/*
    void adopt_particle(
        const size_t index,
        const kvs::Vector3f& coord,
        const size_t scalar,
        const kvs::Vector3f& gradient );
*/
};

} // end of namespace viz

#endif // KVS__METROPOLIS_SAMPLING_H_INCLUDE
