/*****************************************************************************/
/**
 *  @file   CellAdjacencyGraph.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__CELL_ADJACENCY_GRAPH_H_INCLUDE
#define KVS__CELL_ADJACENCY_GRAPH_H_INCLUDE

#include <kvs/UnstructuredVolumeObject>
#include <kvs/BitArray>
#include <kvs/ValueArray>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell adjacency class for a unstructured volume object.
 */
/*===========================================================================*/
class CellAdjacencyGraph
{
private:

    kvs::ValueArray<kvs::UInt32> m_graph; ///< cell adjacency table
    kvs::BitArray m_mask; ///< mask for the external faces

public:

    explicit CellAdjacencyGraph( const kvs::UnstructuredVolumeObject* volume );
    ~CellAdjacencyGraph();

    void create( const kvs::UnstructuredVolumeObject* volume );

    const kvs::ValueArray<kvs::UInt32>& graph() const;
    const kvs::BitArray& mask() const;

private:

    void create_for_tetrahedral_cell( const kvs::UnstructuredVolumeObject* volume );
    void create_for_hexahedral_cell( const kvs::UnstructuredVolumeObject* volume );
    void set_external_face_number();
};

} // end of namespace kvs

#endif // KVS__CELL_ADJACENCY_GRAPH_H_INCLUDE
