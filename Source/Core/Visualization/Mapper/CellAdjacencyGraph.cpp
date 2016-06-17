/*****************************************************************************/
/**
 *  @file   CellAdjacencyGraph.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellAdjacencyGraph.cpp,v 1.2 2009/05/24 01:05:25 naohisa Exp $
 */
/*****************************************************************************/
#include "CellAdjacencyGraph.h"
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Message>
#include <kvs/Assert>
#include <map>
#include <algorithm>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Face class.
 */
/*===========================================================================*/
class Face
{
private:

    kvs::UInt32 m_id[5]; ///< node ID x 3, cell ID, face ID

public:

    Face(
        const kvs::UInt32 node_id0,
        const kvs::UInt32 node_id1,
        const kvs::UInt32 node_id2,
        const kvs::UInt32 cell_id,
        const kvs::UInt32 face_id );

    Face(
        const kvs::UInt32 node_id0,
        const kvs::UInt32 node_id1,
        const kvs::UInt32 node_id2,
        const kvs::UInt32 node_id3,
        const kvs::UInt32 cell_id,
        const kvs::UInt32 face_id );

    kvs::UInt32 nodeID( const size_t index ) const;
    kvs::UInt32 cellID() const;
    kvs::UInt32 faceID() const;
    friend bool operator == ( const Face& f0, const Face& f1 );
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new Face class.
 *  @param  node_id0 [in] node ID 0
 *  @param  node_id1 [in] node ID 1
 *  @param  node_id2 [in] node ID 2
 *  @param  cell_id [in] cell ID
 *  @param  face_id [in] face ID
 */
/*===========================================================================*/
inline Face::Face(
    const kvs::UInt32 node_id0,
    const kvs::UInt32 node_id1,
    const kvs::UInt32 node_id2,
    const kvs::UInt32 cell_id,
    const kvs::UInt32 face_id )
{
    m_id[0] = node_id0;
    m_id[1] = node_id1;
    m_id[2] = node_id2;
    m_id[3] = cell_id;
    m_id[4] = face_id;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Face class.
 *  @param  node_id0 [in] node ID 0
 *  @param  node_id1 [in] node ID 1
 *  @param  node_id2 [in] node ID 2
 *  @param  node_id3 [in] node ID 3
 *  @param  cell_id [in] cell ID
 *  @param  face_id [in] face ID
 */
/*===========================================================================*/
inline Face::Face(
    const kvs::UInt32 node_id0,
    const kvs::UInt32 node_id1,
    const kvs::UInt32 node_id2,
    const kvs::UInt32 node_id3,
    const kvs::UInt32 cell_id,
    const kvs::UInt32 face_id )
{
    kvs::UInt32 id[4] = { node_id0, node_id1, node_id2, node_id3 };
    std::sort( id, id + 4 );

    m_id[0] = id[0];
    m_id[1] = id[1];
    m_id[2] = id[2];
    m_id[3] = cell_id;
    m_id[4] = face_id;
}

/*===========================================================================*/
/**
 *  @brief  Returns a node ID.
 *  @param  index [in] index (0-2)
 *  @return node ID
 */
/*===========================================================================*/
inline kvs::UInt32 Face::nodeID( const size_t index ) const
{
    KVS_ASSERT( index < 3 );

    return m_id[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Returns a cell ID.
 *  @return cell ID
 */
/*===========================================================================*/
inline kvs::UInt32 Face::cellID() const
{
    return m_id[3];
}

/*===========================================================================*/
/**
 *  @brief  Returns a face ID.
 *  @return face ID
 */
/*===========================================================================*/
inline kvs::UInt32 Face::faceID() const
{
    return m_id[4];
}

/*===========================================================================*/
/**
 *  @brief  '==' operator for the Face class.
 *  @param  f0 [in] face #0
 *  @param  f1 [in] face #1
 *  @return true, if the face #0 is equal to the face #1
 */
/*===========================================================================*/
inline bool operator == ( const Face& f0, const Face& f1 )
{
    for ( size_t i = 0; i < 3; i++ )
    {
        bool flag = false;
        for ( size_t j = 0; j < 3; j++ )
        {
            if ( f0.nodeID(i) == f1.nodeID(j) ) { flag = true; continue; }
        }
        if ( !flag ) return false;
    }

    return true;
}

typedef std::multimap<kvs::UInt32,Face> FaceMap;

const kvs::UInt32 TetrahedralCellFaces[12] = {
    0, 1, 2, // face 0
    0, 2, 3, // face 1
    0, 3, 1, // face 2
    1, 3, 2  // face 3
};

const kvs::UInt32 HexahedralCellFaces[24] = {
    0, 1, 2, 3, // face 0
    7, 6, 5, 4, // face 1
    0, 4, 5, 1, // face 2
    1, 5, 6, 2, // face 3
    2, 6, 7, 3, // face 4
    0, 3, 7, 4  // face 5
};

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellAdjacencyGraph class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
CellAdjacencyGraph::CellAdjacencyGraph( const kvs::UnstructuredVolumeObject* volume )
{
    this->create( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destorys the CellAdjacencyGraph class.
 */
/*===========================================================================*/
CellAdjacencyGraph::~CellAdjacencyGraph()
{
}

/*===========================================================================*/
/**
 *  @brief  Creates an adjacency graph of the given volume data.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void CellAdjacencyGraph::create( const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UnstructuredVolumeObject::CellType cell_type = volume->cellType();
    switch ( cell_type )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
        this->create_for_tetrahedral_cell( volume );
        this->set_external_face_number();
        break;
    case kvs::UnstructuredVolumeObject::Hexahedra:
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
        this->create_for_hexahedral_cell( volume );
        this->set_external_face_number();
        break;
    default:
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the adjacency graph.
 *  @return adjacency graph
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt32>& CellAdjacencyGraph::graph() const
{
    return m_graph;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mask for the external faces.
 *  @return mask for the external faces
 */
/*===========================================================================*/
const kvs::BitArray& CellAdjacencyGraph::mask() const
{
    return m_mask;
}

/*===========================================================================*/
/**
 *  @brief  Creates an adjacency graph for the tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void CellAdjacencyGraph::create_for_tetrahedral_cell( const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* const connections = volume->connections().data();
    const size_t nnodes = volume->numberOfNodes();
    const size_t ncells = volume->numberOfCells();
//    const size_t nnodes_per_cell = static_cast<size_t>( volume->cellType() );
    const size_t nnodes_per_cell = volume->numberOfCellNodes();

    m_graph.allocate( ncells * 4 );
    m_graph.fill( 0 );
    m_mask.allocate( ncells * 4 );

    ::FaceMap face_map;
    for ( size_t cell_id = 0, index = 0; cell_id < ncells; cell_id++ )
    {
        // IDs of the first-order nodes.
        const kvs::UInt32 node[4] = {
            connections[ cell_id * nnodes_per_cell ],
            connections[ cell_id * nnodes_per_cell + 1 ],
            connections[ cell_id * nnodes_per_cell + 2 ],
            connections[ cell_id * nnodes_per_cell + 3 ]
        };

        for ( size_t face_id = 0; face_id < 4; face_id++, index++ )
        {
            const kvs::UInt32 n[3] = {
                node[ ::TetrahedralCellFaces[ face_id * 3 ] ],
                node[ ::TetrahedralCellFaces[ face_id * 3 + 1 ] ],
                node[ ::TetrahedralCellFaces[ face_id * 3 + 2 ] ]
            };

            const kvs::UInt32 key( ( n[0] + n[1] + n[2] ) % nnodes );
            const ::Face value( n[0], n[1], n[2], cell_id, face_id );

            // Search.
            m_mask.reset( index );
            ::FaceMap::iterator f = face_map.find( key );
            ::FaceMap::const_iterator last = face_map.end();
            if ( f != last )
            {
                ::FaceMap::const_iterator upper = face_map.upper_bound( key );
                while ( f != upper )
                {
                    if ( f->second == value )
                    {
                        // The edge has been already inserted in the bucket.
                        m_graph[ index ] = f->second.cellID();
                        m_graph[ f->second.cellID() * 4 + f->second.faceID() ] = cell_id;
                        m_mask.set( index );
                        m_mask.set( f->second.cellID() * 4 + f->second.faceID() );
                        face_map.erase( f );
                        break;
                    }
                    f++;
                }
            }
            face_map.insert( std::make_pair( key, value ) );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates an adjacency graph for the hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void CellAdjacencyGraph::create_for_hexahedral_cell( const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* const connections = volume->connections().data();
    const size_t nnodes = volume->numberOfNodes();
    const size_t ncells = volume->numberOfCells();
//    const size_t nnodes_per_cell = static_cast<size_t>( volume->cellType() );
    const size_t nnodes_per_cell = volume->numberOfCellNodes();

    m_graph.allocate( ncells * 6 );
    m_graph.fill( 0 );
    m_mask.allocate( ncells * 6 );

    ::FaceMap face_map;
    for ( size_t cell_id = 0, index = 0; cell_id < ncells; cell_id++ )
    {
        // IDs of the first-order nodes.
        const kvs::UInt32 node[8] = {
            connections[ cell_id * nnodes_per_cell ],
            connections[ cell_id * nnodes_per_cell + 1 ],
            connections[ cell_id * nnodes_per_cell + 2 ],
            connections[ cell_id * nnodes_per_cell + 3 ],
            connections[ cell_id * nnodes_per_cell + 4 ],
            connections[ cell_id * nnodes_per_cell + 5 ],
            connections[ cell_id * nnodes_per_cell + 6 ],
            connections[ cell_id * nnodes_per_cell + 7 ]
        };

        for ( size_t face_id = 0; face_id < 6; face_id++, index++ )
        {
            const kvs::UInt32 n[4] = {
                node[ ::HexahedralCellFaces[ face_id * 4 ] ],
                node[ ::HexahedralCellFaces[ face_id * 4 + 1 ] ],
                node[ ::HexahedralCellFaces[ face_id * 4 + 2 ] ],
                node[ ::HexahedralCellFaces[ face_id * 4 + 3 ] ]
            };

            const kvs::UInt32 key( ( n[0] + n[1] + n[2] + n[3] ) % nnodes );
            const ::Face value( n[0], n[1], n[2], n[3], cell_id, face_id );

            // Search.
            m_mask.reset( index );
            ::FaceMap::iterator f = face_map.find( key );
            ::FaceMap::const_iterator last = face_map.end();
            if ( f != last )
            {
                ::FaceMap::const_iterator upper = face_map.upper_bound( key );
                while ( f != upper )
                {
                    if ( f->second == value )
                    {
                        // The edge has been already inserted in the bucket.
                        m_graph[ index ] = f->second.cellID();
                        m_graph[ f->second.cellID() * 6 + f->second.faceID() ] = cell_id;
                        m_mask.set( index );
                        m_mask.set( f->second.cellID() * 6 + f->second.faceID() );
                        face_map.erase( f );
                        break;
                    }
                    f++;
                }
            }
            face_map.insert( std::make_pair( key, value ) );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets external face numbers.
 */
/*===========================================================================*/
void CellAdjacencyGraph::set_external_face_number()
{
    const size_t nfaces = m_graph.size();
    for ( size_t i = 0, counter = 0; i < nfaces; i++ )
    {
        if ( !m_mask.test(i) ) m_graph[i] = counter++;
    }
}

} // end of namespace kvs
