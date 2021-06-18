/*****************************************************************************/
/**
 *  @file   UnstructuredGradient.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/FilterBase>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Gradient calculation class for unstructured volume object
 */
/*===========================================================================*/
class UnstructuredGradient : public kvs::FilterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::UnstructuredGradient, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:

    UnstructuredGradient( const kvs::UnstructuredVolumeObject* volume );
    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void scalar_gradient( const kvs::UnstructuredVolumeObject* volume );
    void vector_gradient( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs
