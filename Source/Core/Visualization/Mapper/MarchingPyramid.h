/****************************************************************************/
/**
 *  @file   MarchingPyramid.h
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Marching cubes class.
 */
/*==========================================================================*/
class MarchingPyramid : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::MarchingPyramid, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:
    double m_isolevel = 0; ///< isosurface level
    bool m_duplication = true; ///< duplication flag

public:
    MarchingPyramid() = default;
    virtual ~MarchingPyramid() = default;

    MarchingPyramid(
        const kvs::UnstructuredVolumeObject* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }

    kvs::ObjectBase* exec( const kvs::ObjectBase* object );

private:
    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces_with_duplication( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> size_t calculate_table_index( const size_t* local_index ) const;
    template <typename T> const kvs::Vec3 interpolate_vertex( const int vertex0, const int vertex1 ) const;
    template <typename T> size_t calculate_special_table_index( const size_t* local_index, const size_t index ) const;
};

} // end of namespace kvs
