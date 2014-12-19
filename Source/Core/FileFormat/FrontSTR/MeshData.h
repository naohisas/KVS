/*****************************************************************************/
/**
 *  @file   MeshData.h
 *  @author Guo Jiazhen
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
#ifndef KVS__FSTR__MESH_DATA_H_INCLUDE
#define KVS__FSTR__MESH_DATA_H_INCLUDE

#include <string>
#include <iostream>
#include <fstream>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>


namespace kvs
{

namespace fstr
{

/*===========================================================================*/
/**
 *  @brief  Mesh data class.
 */
/*===========================================================================*/
class MeshData
{
public:

    typedef kvs::ValueArray<kvs::Real32> Coords;
    typedef kvs::ValueArray<kvs::UInt32> Connections;

    enum ElementType
    {
        ElementTypeUnknown = 0, ///< Unknown element type
        Line, ///< Line element (111)
        Tri, ///< Triangle element (231)
        Tri2, ///< Quadratic triangle element (232)
        Quad, ///< Quadrangle element (241)
        Quad2, ///< Quadratic quadrangle element (242)
        Tet, ///< Tetrahedral element (341)
        Tet2, ///< Quadratic tetrahedral element (342)
        Penta, ///< Pentahedral element (351)
        Penta2, ///< Quadratic pentahedral element (352)
        Hex, ///< Hexahedral element (361)
        Hex2, ///< Quadratic hexahedral element (362)
        QuadInt, ///< Quadrangle interface element (541)
        TriShell, ///< Triangle shell element (731)
        QuadShell, ///< Quadrangle shell element (741)
        NumberOfElementTypes ///< Number of element types
    };

private:

    ElementType m_element_type; ///< element type
    size_t m_nnodes; ///< number of nodes
    size_t m_ncells; ///< number of cells
    Coords m_coords; ///< coordinate values
    Connections m_connections; ///< connection values

public:

    MeshData();

    ElementType elementType() const;
    size_t numberOfNodes() const;
    size_t numberOfCells() const;
    const Coords& coords() const;
    const Connections& connections() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool readData( const std::string& filename );
    bool readDividedData( const std::string& filename );

private:

    bool read_node( std::string& line, std::ifstream& ifs );
    bool read_element( std::string& line, std::ifstream& ifs );
    void adjust_connection( kvs::UInt32* connection );
};

} // end of namespace fstr

} // end of namespace kvs

#endif // KVS__FSTR__MESH_DATA_H_INCLUDE
