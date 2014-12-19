/*****************************************************************************/
/**
 *  @file   UnstructuredVectorToScalar.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVectorToScalar.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__UNSTRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE
#define KVS__UNSTRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE

#include <kvs/VolumeObjectBase>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/FilterBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Conversion class from vector to scalar value for unstructured volume.
 */
/*===========================================================================*/
class UnstructuredVectorToScalar : public kvs::FilterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::UnstructuredVectorToScalar, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:

    UnstructuredVectorToScalar();
    UnstructuredVectorToScalar( const kvs::UnstructuredVolumeObject* volume );
    virtual ~UnstructuredVectorToScalar();

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    template <typename T>
    void calculate_magnitude( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs

#endif // KVS__UNSTRUCTURED_VECTOR_TO_SCALAR_H_INCLUDE
