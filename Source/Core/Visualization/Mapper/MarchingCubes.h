/****************************************************************************/
/**
 *  @file   MarchingCubes.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Marching cubes class.
 */
/*==========================================================================*/
class MarchingCubes : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::MarchingCubes, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:
    double m_isolevel = 0; ///< isosurface level
    bool m_duplication = true; ///< duplication flag

public:
    MarchingCubes() = default;
    virtual ~MarchingCubes() = default;

    MarchingCubes(
        const kvs::StructuredVolumeObject* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    using Volume = kvs::StructuredVolumeObject;
    using Coords = std::vector<kvs::Real32>;
    using Connects = std::vector<kvs::UInt32>;
    using Normals = std::vector<kvs::Real32>;

    void mapping( const Volume* volume );
    template <typename T> void extract_surfaces( const Volume* volume );
    template <typename T> void extract_surfaces_with_duplication( const Volume* volume );
    template <typename T> void extract_surfaces_without_duplication( const Volume* volume );
    template <typename T> size_t calculate_table_index( const size_t* local_index ) const;
    template <typename T> const kvs::Vec3 interpolate_vertex( const kvs::Vec3& vertex0, const kvs::Vec3& vertex1 ) const;
    template <typename T> void calculate_isopoints( kvs::UInt32*& vertex_map, Coords& coords );
    template <typename T> void connect_isopoints( kvs::UInt32*& vertex_map, Connects& connections );
    void calculate_normals_on_polygon( const Coords& coords, const Connects& connections, Normals& normals );
    void calculate_normals_on_vertex( const Coords& coords, const Connects& connections, Normals& normals );
};

} // end of namespace kvs
