/*****************************************************************************/
/**
 *  @file   MeshData.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MeshData.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF__MESH_DATA_H_INCLUDE
#define KVS__GF__MESH_DATA_H_INCLUDE

#include <iostream>
#include <string>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  GF Mesh data class.
 */
/*===========================================================================*/
class MeshData
{
private:

    size_t m_dimensions; ///< dimensions (2 or 3)
    size_t m_nnodes_per_element; ///< number of nodes per a element
    size_t m_nnodes; ///< number of nodes
    size_t m_nelements; ///< number of elements
    kvs::ValueArray<kvs::Real32> m_coords; ///< coordinate values
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection values

public:

    MeshData();
    MeshData( const std::string filename );

    size_t dimensions() const;
    size_t nnodesPerElement() const;
    size_t nnodes() const;
    size_t nelements() const;
    const kvs::ValueArray<kvs::Real32>& coords() const;
    const kvs::ValueArray<kvs::UInt32>& connections() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string filename );
};

} // end of namespace gf

} // end of namespace kvs

#endif // KVS__GF__MESH_DATA_H_INCLUDE
