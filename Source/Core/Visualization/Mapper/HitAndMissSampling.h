/****************************************************************************/
/**
 *  @file   HitAndMissSampling.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__HIT_AND_MISS_SAMPLING_H_INCLUDE
#define KVS__HIT_AND_MISS_SAMPLING_H_INCLUDE

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
 *  Hit and Miss sampling class.
 */
/*==========================================================================*/
class HitAndMissSampling : public kvs::MapperBase, public kvs::PointObject
{
    kvsModule( kvs::HitAndMissSampling, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PointObject );

public:

    HitAndMissSampling();
    HitAndMissSampling( const kvs::VolumeObjectBase* volume );
    HitAndMissSampling( const kvs::VolumeObjectBase* object, const kvs::TransferFunction& transfer_function );
    virtual ~HitAndMissSampling();

    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void mapping( const kvs::StructuredVolumeObject* volume );
    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void generate_particles( const kvs::StructuredVolumeObject* volume );
    template <typename T> void generate_particles( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs

#endif // KVS__HIT_AND_MISS_SAMPLING_H_INCLUDE
