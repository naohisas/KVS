#pragma once
#include <kvs/PolygonObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeObjectList>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Marching cubes class.
 */
/*==========================================================================*/
class ColoredMarchingCubes : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::ColoredMarchingCubes, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:
    double m_isolevel = 0; ///< isosurface level
    bool m_duplication = true; ///< duplication flag

    // add {
    kvs::ColorMap m_cmap{}; ///< colormap used for color mapping
    kvs::AnyValueArray m_values{}; ///< 2nd scalar values
    kvs::Real64 m_min_value = 0.0; ///< min. value of the 2nd scalars
    kvs::Real64 m_max_value = 0.0; ///< max. value of the 2nd scalars
    // }

public:
    ColoredMarchingCubes() = default;
    virtual ~ColoredMarchingCubes() = default;

    ColoredMarchingCubes(
        const kvs::StructuredVolumeObjectList* volume_list,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    using VolumeList = kvs::StructuredVolumeObjectList; // add
    using Volume = kvs::StructuredVolumeObject;
    using Coords = std::vector<kvs::Real32>;
    using Connects = std::vector<kvs::UInt32>;
    using Normals = std::vector<kvs::Real32>;
    using Colors = std::vector<kvs::UInt8>; // add

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

    // add {
    template <typename T> void extract_colored_surfaces_with_duplication( const Volume* volume );
    template <typename T> void extract_colored_surfaces_without_duplication( const Volume* volume );
    template <typename T> const kvs::RGBColor interpolate_color( const kvs::Vec3& vertex0, const kvs::Vec3& vertex1 ) const;
    template <typename T> void calculate_isopoints( kvs::UInt32*& vertex_map, Coords& coords, Colors& colors );
    // }
};

} // end of namespace kvs
