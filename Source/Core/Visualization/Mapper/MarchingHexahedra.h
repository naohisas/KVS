/****************************************************************************/
/**
 *  @file   MarchingHexahedra.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MarchingHexahedra.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MARCHING_HEXAHEDRA_H_INCLUDE
#define KVS__MARCHING_HEXAHEDRA_H_INCLUDE

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
class MarchingHexahedra : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::MarchingHexahedra, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:

    double m_isolevel; ///< isosurface level
    bool m_duplication; ///< duplication flag

public:

    MarchingHexahedra();
    MarchingHexahedra(
        const kvs::UnstructuredVolumeObject* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );
    virtual ~MarchingHexahedra();

    void setIsolevel( const double isolevel );

    kvs::ObjectBase* exec( const kvs::ObjectBase* object );

private:

    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> void extract_surfaces_with_duplication( const kvs::UnstructuredVolumeObject* volume );
    template <typename T> size_t calculate_table_index( const size_t* local_index ) const;
    template <typename T> const kvs::Vector3f interpolate_vertex( const int vertex0, const int vertex1 ) const;
    template <typename T> const kvs::RGBColor calculate_color();
};

} // end of namespace kvs

#endif // KVS__MARCHING_HEXAHEDRA_H_INCLUDE
