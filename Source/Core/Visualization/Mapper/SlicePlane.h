/****************************************************************************/
/**
 *  @file   SlicePlane.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SlicePlane.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SLICE_PLANE_H_INCLUDE
#define KVS__SLICE_PLANE_H_INCLUDE

#include <kvs/PolygonObject>
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Slice plane class.
 */
/*==========================================================================*/
class SlicePlane : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::SlicePlane, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:

    kvs::Vec4 m_coefficients; ///< coeficients of a slice plane

public:

    SlicePlane();
    SlicePlane(
        const kvs::VolumeObjectBase* volume,
        const kvs::Vec4& coefficients,
        const kvs::TransferFunction& transfer_function );
    SlicePlane(
        const kvs::VolumeObjectBase* volume,
        const kvs::Vec3& point,
        const kvs::Vec3& normal,
        const kvs::TransferFunction& transfer_function );
    virtual ~SlicePlane();

    void setPlane( const kvs::Vec4& coefficients );
    void setPlane( const kvs::Vec3& point, const kvs::Vec3& normal );

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    void mapping( const kvs::VolumeObjectBase* volume );
    template <typename T> void extract_plane( const kvs::StructuredVolumeObject* volume );
    template <typename T> void extract_plane( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_tetrahedra_plane( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_hexahedra_plane( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_pyramid_plane( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_prism_plane( const kvs::UnstructuredVolumeObject* volume );
    size_t calculate_table_index( const size_t x, const size_t y, const size_t z ) const;
    size_t calculate_tetrahedra_table_index( const size_t* local_index ) const;
    size_t calculate_hexahedra_table_index( const size_t* local_index ) const;
    size_t calculate_pyramid_table_index( const size_t* local_index ) const;
    size_t calculate_prism_table_index( const size_t* local_index ) const;
    float substitute_plane_equation( const size_t x, const size_t y, const size_t z ) const;
    float substitute_plane_equation( const kvs::Vec3& vertex ) const;
    const kvs::Vec3 interpolate_vertex( const kvs::Vec3& vertex0, const kvs::Vec3& vertex1 ) const;
    template <typename T> double interpolate_value(
        const kvs::StructuredVolumeObject* volume,
        const kvs::Vec3& vertex0,
        const kvs::Vec3& vertex1 ) const;
    template <typename T> double interpolate_value(
        const kvs::UnstructuredVolumeObject* volume,
        const size_t index0,
        const size_t index1 ) const;
};

} // end of namespace kvs

#endif // KVS__SLICE_PLANE_H_INCLUDE
