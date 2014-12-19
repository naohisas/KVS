/****************************************************************************/
/**
 *  @file   ExtractVertices.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractVertices.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__EXTRACT_VERTICES_H_INCLUDE
#define KVS__EXTRACT_VERTICES_H_INCLUDE

#include <kvs/Module>
#include <kvs/PointObject>
#include "MapperBase.h"


namespace kvs
{

class VolumeObjectBase;

/*==========================================================================*/
/**
 *  ExtractVertices class.
 */
/*==========================================================================*/
class ExtractVertices : public kvs::MapperBase, public kvs::PointObject
{
    kvsModule( kvs::ExtractVertices, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PointObject );

public:

    ExtractVertices();
    ExtractVertices( const kvs::VolumeObjectBase* volume );
    ExtractVertices( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );
    virtual ~ExtractVertices();

    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void mapping( const kvs::VolumeObjectBase* volume );
    void calculate_coords();
    void calculate_uniform_coords();
    void calculate_rectiliner_coords();
    template <typename T> void calculate_colors();
};

}

#endif // KVS__EXTRACT_VERTICES_H_INCLUDE
