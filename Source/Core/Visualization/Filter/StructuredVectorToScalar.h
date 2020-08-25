/*****************************************************************************/
/**
 *  @file   StructuredVectorToScalar.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__STRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE
#define KVS__STRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE

#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/FilterBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Conversion class from vector to scalar value for structured volume.
 */
/*===========================================================================*/
class StructuredVectorToScalar : public kvs::FilterBase, public kvs::StructuredVolumeObject
{
    kvsModule( kvs::StructuredVectorToScalar, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::StructuredVolumeObject );

public:

    StructuredVectorToScalar();
    StructuredVectorToScalar( const kvs::StructuredVolumeObject* volume );
    virtual ~StructuredVectorToScalar();

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    template <typename T>
    void calculate_magnitude( const kvs::StructuredVolumeObject* volume );
};

} // end of namespace kvs

#endif // KVS__STRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE
