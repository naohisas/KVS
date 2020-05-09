/*****************************************************************************/
/**
 *  @file   ExternalFaces.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExternalFaces.h 1761 2014-05-07 04:18:15Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/MapperBase>
#include <kvs/PolygonObject>


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
class ExternalFaces : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::ExternalFaces, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

public:
    ExternalFaces();
    ExternalFaces( const kvs::VolumeObjectBase* volume );
    ExternalFaces( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );
    virtual ~ExternalFaces();

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    void mapping( const kvs::StructuredVolumeObject* volume );
    void calculate_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_uniform_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_rectilinear_coords( const kvs::StructuredVolumeObject* volume );
    void calculate_curvilinear_coords( const kvs::StructuredVolumeObject* volume );
    template <typename T> void calculate_colors( const kvs::StructuredVolumeObject* volume );

    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void calculate_tetrahedral_faces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void calculate_quadratic_tetrahedral_faces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void calculate_hexahedral_faces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void calculate_quadratic_hexahedral_faces( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs
