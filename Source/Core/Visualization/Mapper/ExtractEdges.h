/*****************************************************************************/
/**
 *  @file   ExtractEdges.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractEdges.h 1766 2014-05-10 15:22:50Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__EXTRACT_EDGES_H_INCLUDE
#define KVS__EXTRACT_EDGES_H_INCLUDE

#include <kvs/Module>
#include <kvs/MapperBase>
#include <kvs/LineObject>


namespace kvs
{

class VolumeObjectBase;
class StructuredVolumeObject;
class UnstructuredVolumeObject;
class TransferFunction;

/*===========================================================================*/
/**
 *  @brief  Extract external faces.
 */
/*===========================================================================*/
class ExtractEdges : public kvs::MapperBase, public kvs::LineObject
{
    kvsModule( kvs::ExtractEdges, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::LineObject );

public:

    ExtractEdges();
    ExtractEdges( const kvs::VolumeObjectBase* volume );
    ExtractEdges( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );
    virtual ~ExtractEdges();

    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void mapping( const kvs::StructuredVolumeObject* volume );
    void calculate_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_uniform_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_rectilinear_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_connections( const kvs::StructuredVolumeObject* volume );

    void mapping( const kvs::UnstructuredVolumeObject* volume );
    void calculate_coords( const kvs::UnstructuredVolumeObject* volume );
    void calculate_connections( const kvs::UnstructuredVolumeObject* volume );
    void calculate_tetrahedra_connections( const kvs::UnstructuredVolumeObject* volume );
    void calculate_hexahedra_connections( const kvs::UnstructuredVolumeObject* volume );
    void calculate_quadratic_tetrahedra_connections( const kvs::UnstructuredVolumeObject* volume );
    void calculate_quadratic_hexahedra_connections( const kvs::UnstructuredVolumeObject* volume );
    void calculate_prism_connections( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void calculate_colors( const kvs::VolumeObjectBase* volume );
};

} // end of namespace kvs

#endif // KVS__EXTRACT_EDGES_H_INCLUDE
