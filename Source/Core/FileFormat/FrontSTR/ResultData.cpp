/*****************************************************************************/
/**
 *  @file   ResultData.cpp
 *  @author Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResultData.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ResultData.h"
#include <cstdio>
#include <sstream>


namespace kvs
{

namespace fstr
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResultData class.
 */
/*===========================================================================*/
ResultData::ResultData():
    m_nnodes(0),
    m_ncells(0),
    m_ncomponents_per_node(0),
    m_ncomponents_per_cell(0)
{
}

/*===========================================================================*/
/**
 *  @brief  Returns number of nodes.
 *  @return number of nodes
 */
/*===========================================================================*/
size_t ResultData::numberOfNodes() const
{
    return m_nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of cells.
 *  @return number of cells
 */
/*===========================================================================*/
size_t ResultData::numberOfCells() const
{
    return m_ncells;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of components per node.
 *  @return number of components per node
 */
/*===========================================================================*/
size_t ResultData::numberOfComponentsPerNode() const
{
    return m_ncomponents_per_node;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of components per cell.
 *  @return number of components per cell
 */
/*===========================================================================*/
size_t ResultData::numberOfComponentsPerCell() const
{
    return m_ncomponents_per_cell;
}

/*===========================================================================*/
/**
 *  @brief  Returns vector length for each components.
 *  @return vector length for each components
 */
/*===========================================================================*/
const std::vector<size_t>& ResultData::veclens() const
{
    return m_veclens;
}

/*===========================================================================*/
/**
 *  @brief  Returns labels for each components.
 *  @return labels for each components
 */
/*===========================================================================*/
const std::vector<std::string>& ResultData::labels() const
{
    return m_labels;
}

/*===========================================================================*/
/**
 *  @brief  Returns values for each components.
 *  @return values for each components.
 */
/*===========================================================================*/
const std::vector<ResultData::Values>& ResultData::values() const
{
    return m_values;
}

void ResultData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of cells : " << m_ncells << std::endl;
    os << indent << "Number of components per node : " << m_ncomponents_per_node << std::endl;
    os << indent << "Number of components per cell : " << m_ncomponents_per_cell << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Reads non divided result data.
 *  @param  filename [in] filename
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ResultData::readData( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    // Skipping comment.
    std::string line("");
    while ( std::getline( ifs, line ) )
    {
        if ( line[0] == '*' ) continue;
        else break;
    }

    m_nnodes = 0;
    m_ncells = 0;
    m_ncomponents_per_node = 0;
    m_ncomponents_per_cell = 0;

    if ( !this->read_nnodes_and_ncells( line, ifs ) )
    {
        kvsMessageError("Cannot read nnodes and ncells.");
        return false;
    }

    if ( !this->read_veclens( line, ifs ) )
    {
        kvsMessageError("Cannot read veclens.");
        return false;
    }

    if ( !this->read_labels( line, ifs ) )
    {
        kvsMessageError("Cannot read labels.");
        return false;
    }

    // Reading values for nodes.
    kvs::ValueArray<kvs::Real32>* values_temp = new kvs::ValueArray<kvs::Real32> [ m_ncomponents_per_node ];
    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        values_temp[i].allocate( m_nnodes * m_veclens[i] );
        values_temp[i].fill( 0 );
    }

    for ( size_t i = 0; i < m_nnodes; i++ )
    {
        size_t node_index = 0; ifs >> node_index; node_index -= 1;
        if ( node_index >= m_nnodes ) break;
        for ( size_t j = 0; j < m_ncomponents_per_node; j++ )
        {
            const size_t veclen = m_veclens[j];
            const size_t index = node_index * veclen;
            for ( size_t k = 0; k < veclen; k++ )
            {
                kvs::Real32 value = 0.0f; ifs >> value;
                values_temp[j][ index + k ] = value;
            }
        }
    }

    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        m_values.push_back( values_temp[i] );
        values_temp[i].release();
    }

    delete [] values_temp;

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads divided result data.
 *  @param  filename [in] filename
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ResultData::readDividedData( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    // Skipping comment.
    std::string line("");
    while ( std::getline( ifs, line ) )
    {
        if ( line[0] == '*' ) continue;
        else break;
    }

    m_nnodes = 0;
    m_ncells = 0;
    m_ncomponents_per_node = 0;
    m_ncomponents_per_cell = 0;

    if ( !this->read_nnodes_and_ncells( line, ifs ) )
    {
        kvsMessageError("Cannot read nnodes and ncells.");
        return false;
    }

    if ( !this->read_veclens( line, ifs ) )
    {
        kvsMessageError("Cannot read veclens.");
        return false;
    }

    if ( !this->read_labels( line, ifs ) )
    {
        kvsMessageError("Cannot read labels.");
        return false;
    }

    // Reading values for nodes.
    kvs::ValueArray<kvs::Real32>* values_temp = new kvs::ValueArray<kvs::Real32> [ m_ncomponents_per_node ];
    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        values_temp[i].allocate( m_nnodes * m_veclens[i] );
        values_temp[i].fill( 0 );
    }

    for ( size_t i = 0; i < m_nnodes; i++ )
    {
        // In divided result data, the index is global, this line is not used for now,
        // but will be used later.
        size_t node_index = 0; ifs >> node_index;
        for ( size_t j = 0; j < m_ncomponents_per_node; j++ )
        {
            const size_t veclen = m_veclens[j];
            const size_t index = i * veclen;
            for ( size_t k = 0; k < veclen; k++ )
            {
                kvs::Real32 value = 0.0f; ifs >> value;
                values_temp[j][ index + k ] = value;
            }
        }
    }

    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        m_values.push_back( values_temp[i] );
        values_temp[i].release();
    }

    delete [] values_temp;

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads number of nodes and cells.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ResultData::read_nnodes_and_ncells( std::string& line, std::ifstream& ifs )
{
    // Reading number of nodes and cells.
    if ( std::sscanf( line.c_str(), "%lu %lu", &m_nnodes, &m_ncells ) == EOF ) return false;

    // Reading number of components for nodes and cells.
    if ( !std::getline( ifs, line ) ) return false;
    if ( std::sscanf( line.c_str(), "%lu %lu", &m_ncomponents_per_node, &m_ncomponents_per_cell ) == EOF ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads vector length for each components.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ResultData::read_veclens( std::string& line, std::ifstream& ifs )
{
    // Reading vector length of components.
    if ( !std::getline( ifs, line ) ) return false;

    std::stringstream buffer( line );
    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        size_t veclen = 0; buffer >> veclen;
        m_veclens.push_back( veclen );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads labels for each components.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ResultData::read_labels( std::string& line, std::ifstream& ifs )
{
    // Reading labels of components.
    for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
    {
        if ( !std::getline( ifs, line ) ) return false;
        m_labels.push_back( line );
    }

    return true;
}

} // end of namespace fstr

} // end of namespace kvs
