/****************************************************************************/
/**
 *  @file   MarchingTetrahedra.h
 *  @author Naohisa Sakamoto
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
 *  Marching tetrahedra class.
 */
/*==========================================================================*/
class MarchingTetrahedra : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::MarchingTetrahedra, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:
    double m_isolevel = 0; ///< isosurface level
    bool m_duplication = true; ///< duplication flag

public:
    MarchingTetrahedra() = default;
    virtual ~MarchingTetrahedra() = default;

    MarchingTetrahedra(
        const kvs::UnstructuredVolumeObject* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces_with_duplication( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces_without_duplication( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> size_t calculate_table_index( const size_t* local_index ) const;
    template <typename T> const kvs::Vec3 interpolate_vertex( const int vertex0, const int vertex1 ) const;

#if NOT_YET_IMPLEMENTED
    template <typename T> void calculate_isopoints( kvs::UInt32*& vertex_map, std::vector<kvs::Real32>& coords );
    template <typename T> void connect_isopoints( kvs::UInt32*& vertex_map, std::vector<kvs::UInt32>& connections );
    void calculate_normals_on_polygon(
        const std::vector<kvs::Real32>& coords,
        const std::vector<kvs::UInt32>& connections,
        std::vector<kvs::Real32>&       normals );
    void calculate_normals_on_vertex(
        const std::vector<kvs::Real32>& coords,
        const std::vector<kvs::UInt32>& connections,
        std::vector<kvs::Real32>&       normals );
#endif // NOT_YET_IMPLEMENTED
};

} // end of namespace kvs
