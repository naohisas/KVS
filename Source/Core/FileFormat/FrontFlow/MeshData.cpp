/*****************************************************************************/
/**
 *  @file   MeshData.cpp
 *  @author Naohisa Sakamoto
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
#include "File.h"


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  Construct a new MeshData class.
 */
/*===========================================================================*/
MeshData::MeshData():
    m_dimensions( 0 ),
    m_nnodes_per_element( 0 ),
    m_nnodes( 0 ),
    m_nelements( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new MeshData class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
MeshData::MeshData( const std::string filename ):
    m_dimensions( 0 ),
    m_nnodes_per_element( 0 ),
    m_nnodes( 0 ),
    m_nelements( 0 )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Return dimensions.
 *  @return dimensions
 */
/*===========================================================================*/
size_t MeshData::dimensions() const
{
    return m_dimensions;
}

/*===========================================================================*/
/**
 *  @brief  Return number of nodes per element.
 *  @return number of nodes per element
 */
/*===========================================================================*/
size_t MeshData::nnodesPerElement() const
{
    return m_nnodes_per_element;
}

/*===========================================================================*/
/**
 *  @brief  Return number of nodes.
 *  @return number of nodes
 */
/*===========================================================================*/
size_t MeshData::nnodes() const
{
    return m_nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Return number of elements.
 *  @return number of elements
 */
/*===========================================================================*/
size_t MeshData::nelements() const
{
    return m_nelements;
}

/*===========================================================================*/
/**
 *  @brief  Return coordinate values.
 *  @return coordinate values
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32>& MeshData::coords() const
{
    return m_coords;
}

/*===========================================================================*/
/**
 *  @brief  Return connection values.
 *  @return connection values
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt32>& MeshData::connections() const
{
    return m_connections;
}

void MeshData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Dimensions : " << m_dimensions << std::endl;
    os << indent << "Number of nodes per element : " << m_nnodes_per_element << std::endl;
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of elements : " << m_nelements << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read mesh data file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool MeshData::read( const std::string filename )
{
    kvs::gf::File file;
    if ( !file.read( filename ) )
    {
        kvsMessageError("Cannot read mesh data file.");
        return false;
    }

    if ( file.dataSetList().size() != 1 )
    {
        kvsMessageError("Multiple data set is not supported in kvs::gf::MeshData.");
        return false;
    }

    const kvs::gf::DataSet& data_set = file.dataSet(0);
    for ( size_t i = 0; i < data_set.dataList().size(); i++ )
    {
        const kvs::gf::Data& data = data_set.data(i);
        const std::string& keyword = data.keyword();
        const std::string& type = data.arrayTypeHeader();

        // Read coordinate values.
        if ( keyword == "*GRID_2D" || keyword == "*GRID_3D" )
        {
            if ( type == "#FLT_ARY" )
            {
                m_dimensions = data.num();
                m_nnodes = data.num2();
                m_coords = data.fltArray();
            }
        }

        // Read connection values.
        if ( keyword == "*NODE_2D" || keyword == "*NODE_3D" )
        {
            if ( type == "#INT_ARY" )
            {
                m_nnodes_per_element = data.num();
                m_nelements = data.num2();
                const kvs::Int32* src = data.intArray().data();
                const size_t size = data.intArray().size();
                m_connections.allocate( size );
                kvs::UInt32* dst = m_connections.data();
                for ( size_t i = 0; i < size; i++ )
                    dst[i] = static_cast<kvs::UInt32>( src[i] - 1 );
            }
        }
    }

    return true;
}

} // end of namespace gf

} // end of namespace kvs
