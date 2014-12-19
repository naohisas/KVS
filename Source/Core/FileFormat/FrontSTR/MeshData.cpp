/*****************************************************************************/
/**
 *  @file   MeshData.cpp
 *  @author Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MeshData.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "MeshData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <kvs/Tokenizer>
#include <kvs/Message>


namespace
{

const std::string ElelemtTypeToString[kvs::fstr::MeshData::NumberOfElementTypes] =
{
    "Unknown",
    "Line",
    "Tri",
    "Tri2",
    "Quad",
    "Quad2",
    "Tet",
    "Tet2",
    "Penta",
    "Penta2",
    "Hex",
    "Hex2",
    "QuadInt",
    "TriShell",
    "QuadShell"
};

const size_t NumberOfNodes[15] =
{
    0,  // unknown
    2,  // line (111)
    3,  // tri (231)
    6,  // tri2 (232)
    4,  // quad (241)
    8,  // quad2 (242)
    4,  // tet (341)
    10, // tet2 (342)
    6,  // penta (351)
    15, // penta2 (352)
    8,  // hex (361)
    20, // hex2 (362)
    8,  // quad interface (541)
    3,  // tri shell (731)
    4   // quad shell (741)
};

/*===========================================================================*/
/**
 *  @brief  Returns element type.
 *  @param  value [in] value (element number)
 *  @return element type
 */
/*===========================================================================*/
kvs::fstr::MeshData::ElementType GetElementType( const int value )
{
    kvs::fstr::MeshData::ElementType element_type = kvs::fstr::MeshData::ElementTypeUnknown;
    switch ( value )
    {
    case 111: element_type = kvs::fstr::MeshData::Line; break;
    case 231: element_type = kvs::fstr::MeshData::Tri; break;
    case 232: element_type = kvs::fstr::MeshData::Tri2; break;
    case 241: element_type = kvs::fstr::MeshData::Quad; break;
    case 242: element_type = kvs::fstr::MeshData::Quad2; break;
    case 341: element_type = kvs::fstr::MeshData::Tet; break;
    case 342: element_type = kvs::fstr::MeshData::Tet2; break;
    case 351: element_type = kvs::fstr::MeshData::Penta; break;
    case 352: element_type = kvs::fstr::MeshData::Penta2; break;
    case 361: element_type = kvs::fstr::MeshData::Hex; break;
    case 362: element_type = kvs::fstr::MeshData::Hex2; break;
    case 541: element_type = kvs::fstr::MeshData::QuadInt; break;
    case 731: element_type = kvs::fstr::MeshData::TriShell; break;
    case 741: element_type = kvs::fstr::MeshData::QuadShell; break;
    default: break;
    }

    return element_type;
}

}


namespace kvs
{

namespace fstr
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MeshData class.
 */
/*===========================================================================*/
MeshData::MeshData():
    m_element_type(MeshData::ElementTypeUnknown),
    m_nnodes(0),
    m_ncells(0)
{
}

/*===========================================================================*/
/**
 *  @brief  Returns element type.
 *  @return element type
 */
/*===========================================================================*/
MeshData::ElementType MeshData::elementType() const
{
    return m_element_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of nodes
 *  @return number of nodes
 */
/*===========================================================================*/
size_t MeshData::numberOfNodes() const
{
    return m_nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of cells.
 *  @return number of cells
 */
/*===========================================================================*/
size_t MeshData::numberOfCells() const
{
    return m_ncells;
}

/*===========================================================================*/
/**
 *  @brief  Returns coordinate values.
 *  @return coordinate values
 */
/*===========================================================================*/
const MeshData::Coords& MeshData::coords() const
{
    return m_coords;
}

/*===========================================================================*/
/**
 *  @brief  Returns connection values.
 *  @return connection values
 */
/*===========================================================================*/
const MeshData::Connections& MeshData::connections() const
{
    return m_connections;
}

void MeshData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Element type : " << ::ElelemtTypeToString[m_element_type] << std::endl;
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of cells : " << m_ncells << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Reads non divided mesh data.
 *  @param  filename [in] filename
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool MeshData::readData( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    std::string line("");
    std::string header("");
    while ( !ifs.eof() )
    {
        if ( line.empty() ) std::getline( ifs, line );

        // Skipping comment.
        if ( line[0] == '#' ) continue;
        if ( line.size() > 2 ) if ( line[0] == '!' && line[1] == '!' ) continue;

        // Check header.
        kvs::Tokenizer t( line, ",\r\n");
        header = t.token();

        // Reading coordinate data.
        if ( header == "!NODE" )
        {
            if ( !this->read_node( line, ifs ) )
            {
                kvsMessageError("Cannot read !NODE.");
                return false;
            }
        }

        // Reading connection data.
        else if ( header == "!ELEMENT" )
        {
            if ( !this->read_element( line, ifs ) )
            {
                kvsMessageError("Cannot read !ELEMENT.");
                return false;
            }
        }

        else if ( header == "!END" ) { break; } // terminate
        else { line.erase(); } // go to next line
    }

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads divided mesh data.
 *  @param  filename [in] filename
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool MeshData::readDividedData( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    m_nnodes = 0;
    m_ncells = 0;

    // Skipping header (11 lines).
    std::string line = "";
    std::getline( ifs, line ); // comment? (e.g. !HECMW-DMD-ASCII version=3)
    std::getline( ifs, line ); // number?  (e.g. 0)
    std::getline( ifs, line ); // number?  (e.g. 0)
    std::getline( ifs, line ); // number?  (e.g. 1)
    std::getline( ifs, line ); // number?  (e.g. 1)
    std::getline( ifs, line ); // number?  (e.g. 3)
    std::getline( ifs, line ); // file?    (e.g. ../v6engine.msh)
    std::getline( ifs, line ); // number?  (e.g. 0)
    std::getline( ifs, line ); // number?  (e.g. 1)
    std::getline( ifs, line ); // comment? (e.g. HECMW_Msh File generated by Ruby Script)
    std::getline( ifs, line ); // number?  (e.g. 0.0000000000000000E+00)

    // Reanding number of nodes.
    size_t nnodes_unknown = 0;
    size_t nnodes_inside = 0;
    ifs >> m_nnodes;
    ifs >> nnodes_unknown;
    ifs >> nnodes_inside;
    std::getline( ifs, line );

    // Reading boundary information. (?)
    // NOTE: This part may be useful to connect those partially defined data
    // into a whole, now it's skipped, but it should be implemented afterwards.
    const size_t nnodes = m_nnodes;
    for ( size_t i = 0; i < nnodes; i++ ) std::getline( ifs, line );

    // Reading index table of global coordinate.
    kvs::ValueArray<kvs::UInt32> coords_table( nnodes );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        kvs::UInt32 temp = 0; ifs >> temp;
        coords_table[i] = temp - 1;
    }
    std::getline( ifs, line );

    // Reading global coordinates data.
    m_coords.allocate( nnodes * 3 );
    kvs::Real32* coords = m_coords.data();
    for ( size_t i = 0; i < nnodes; i++ )
    {
        std::getline( ifs, line );

        std::istringstream line_stream( line );
        std::string svalue;

        std::getline( line_stream, svalue, ' ' );
        *(coords++) = static_cast<kvs::Real32>( atof( svalue.c_str() ) );

        std::getline( line_stream, svalue, ' ' );
        *(coords++) = static_cast<kvs::Real32>( atof( svalue.c_str() ) );

        std::getline( line_stream, svalue, ' ' );
        *(coords++) = static_cast<kvs::Real32>( atof( svalue.c_str() ) );
    }

    // Skipping header (4 lines).
    std::getline( ifs, line ); // number?  (e.g. 3)
    std::getline( ifs, line ); // number?  (e.g. 1)
    std::getline( ifs, line ); // number?  (e.g. 0 17184)
    std::getline( ifs, line ); // number?  (e.g. 3)

    // Reading number of cells.
    size_t ncells_unknown1 = 0;
    size_t ncells_unknown2 = 0;
    ifs >> m_ncells;
    ifs >> ncells_unknown1;
    ifs >> ncells_unknown2;
    std::getline( ifs, line );

    // Reading local index table. (?)
    for ( size_t i = 0; i < ncells_unknown2; i++ ) { float temp = 0.0f; ifs >> temp; }
    std::getline( ifs, line );

    // Skipping. (?)
    const size_t ncells = m_ncells;
    for ( size_t i = 0; i < ncells; i++ ) std::getline( ifs, line );

    // Reading global index table. (?)
    kvs::ValueArray<kvs::UInt32> connections_table( ncells );
    for ( size_t i = 0; i < ncells; i++ )
    {
        kvs::UInt32 temp = 0; ifs >> temp;
        connections_table[i] = temp - 1;
    }

    // Skipping. (?)
    for ( size_t i = 0; i < ncells; i++ ) { int temp = 0; ifs >> temp; } // Element type?
    std::getline( ifs, line );

    // Skipping. (?)
    std::getline( ifs, line );

    // Reading. (?)
    size_t start = 0;
    size_t end = 0;
    ifs >> start;
    ifs >> end;

    // Reading element type.
    int element_type = 0; ifs >> element_type;
    m_element_type = ::GetElementType( element_type );
    std::getline( ifs, line );

    // Reading. (?)
    for ( size_t i = 0; i < end - start + 1; i++ ) { size_t temp = 0; ifs >> temp; }
    std::getline( ifs, line );

    // Reading local connections.
    const size_t nnodes_per_cell = ::NumberOfNodes[m_element_type];
    m_connections.allocate( nnodes_per_cell * ncells );
    kvs::UInt32* connections = m_connections.data();
    for ( size_t i = 0; i < ncells; i++ )
    {
        kvs::UInt32* c = connections;
        for ( size_t j = 0; j < nnodes_per_cell; j++ )
        {
            kvs::UInt32 value = 0; ifs >> value;
            *(connections++) = value - 1;
        }

        this->adjust_connection( c );
    }

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads node data (coordinate values).
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool MeshData::read_node( std::string& line, std::ifstream& ifs )
{
    size_t counter = 0;
    std::vector<kvs::Real32> coords;
    while ( std::getline( ifs, line ) )
    {
        if ( line.size() > 2 ) if ( line[0] == '!' && line[1] != '!' ) break;

        std::istringstream line_stream( line );
        std::string svalue;

        // node ID (skip)
        if ( !std::getline( line_stream, svalue, ',' ) ) return false;

        // x, y, z
        while ( std::getline( line_stream, svalue, ',' ) )
        {
            const kvs::Real32 value = static_cast<kvs::Real32>( atof( svalue.c_str() ) );
            coords.push_back( value );
        }

        counter++;
    }

    m_nnodes = counter;
    m_coords = kvs::ValueArray<kvs::Real32>( coords );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads element data (element type and connection values).
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool MeshData::read_element( std::string& line, std::ifstream& ifs )
{
    m_element_type = ElementTypeUnknown;
    {
        std::istringstream line_stream( line );
        std::string svalue;

        // !ELEMENT
        if ( !std::getline( line_stream, svalue, ',' ) ) return false;

        // Reading element type.
        while ( std::getline( line_stream, svalue, ',' ) )
        {
            // Trim whitespace.
            svalue.erase( 0, svalue.find_first_not_of(" ") );

            kvs::Tokenizer t( svalue, "=");
            const std::string name = t.token();
            if ( name == "TYPE" )
            {
                const int element_type = atoi( t.token().c_str() );
                m_element_type = ::GetElementType( element_type );
            }
        }
    }

    size_t counter = 0;
    std::vector<kvs::UInt32> connections;
    const size_t nnodes_per_cell = ::NumberOfNodes[m_element_type];
    while ( std::getline( ifs, line ) )
    {
        if ( line.size() > 2 ) if ( line[0] == '!' && line[1] != '!' ) break;

        std::istringstream line_stream( line );
        std::string svalue;

        // element ID
        if ( !std::getline( line_stream, svalue, ',' ) ) return false;

        // Reading connections.
        for ( size_t i = 0; i < nnodes_per_cell; i++ )
        {
            std::getline( line_stream, svalue, ',' );

            const kvs::UInt32 value = static_cast<kvs::UInt32>( atoi( svalue.c_str() ) - 1 );
            connections.push_back( value );
        }

        // Adjust connections for KVS.
        kvs::UInt32* c = &(connections[ nnodes_per_cell * counter ]);
        this->adjust_connection( c );

        counter++;
    }

    m_ncells = counter;
    m_connections = kvs::ValueArray<kvs::UInt32>( connections );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Adjust connection for KVS.
 *  @param  connection [in] connection in FrontSTR
 */
/*===========================================================================*/
void MeshData::adjust_connection( kvs::UInt32* connection )
{
    switch ( m_element_type )
    {
    case Line:
    case Tri:
    case Tri2:
    case Quad:
    case Quad2:
    case Penta:
    case Penta2:
    case QuadInt:
    case TriShell:
    case QuadShell:
    {
        // Not supported in KVS.
        break;
    }
    case Tet:
    {
        // FSTR: 0 1 2 3
        // KVS:  0 3 2 1
        std::swap( connection[1], connection[3] );
        break;
    }
    case Tet2:
    {
        // FSTR: 0 1 2 3 4 5 6 7 8 9
        // KVS:  0 1 2 3 7 5 4 6 9 8
        std::swap( connection[4], connection[6] );
        std::swap( connection[6], connection[7] );
        std::swap( connection[8], connection[9] );
        break;
    }
    case Hex:
    {
        // FSTR: 0 1 2 3 4 5 6 7
        // KVS:  4 5 6 7 0 1 2 3
        std::swap( connection[0], connection[4] );
        std::swap( connection[1], connection[5] );
        std::swap( connection[2], connection[6] );
        std::swap( connection[3], connection[7] );
        break;
    }
    case Hex2:
    {
        // FSTR: 0 1 2 3 4 5 6 7  8  9 10 11 12 13 14 15 16 17 18 19
        // KVS:  4 5 6 7 0 1 2 3 12 13 14 15  8  9 10 11 16 17 18 19
        std::swap( connection[0],  connection[4] );
        std::swap( connection[1],  connection[5] );
        std::swap( connection[2],  connection[6] );
        std::swap( connection[3],  connection[7] );
        std::swap( connection[8],  connection[12] );
        std::swap( connection[9],  connection[13] );
        std::swap( connection[10], connection[14] );
        std::swap( connection[11], connection[15] );
        break;
    }
    default: break;
    }
}

} // end of namespace fstr

} // end of namespace kvs
