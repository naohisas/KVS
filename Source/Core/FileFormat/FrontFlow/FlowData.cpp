/*****************************************************************************/
/**
 *  @file   FlowData.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FlowData.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "FlowData.h"
#include "File.h"
#include <kvs/Assert>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  Construct a new FlowData class.
 */
/*===========================================================================*/
FlowData::FlowData():
    m_dimensions( 0 ),
    m_nnodes( 0 ),
    m_nelements( 0 ),
    m_nsteps( 0 ),
    m_times( NULL ),
    m_steps( NULL ),
    m_velocities( NULL ),
    m_pressures( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new FlowData class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
FlowData::FlowData( const std::string filename ):
    m_dimensions( 0 ),
    m_nnodes( 0 ),
    m_nelements( 0 ),
    m_nsteps( 0 ),
    m_times( NULL ),
    m_steps( NULL ),
    m_velocities( NULL ),
    m_pressures( NULL )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Destroy the FlowData class.
 */
/*===========================================================================*/
FlowData::~FlowData()
{
    if ( m_times ) delete [] m_times;
    if ( m_steps ) delete [] m_steps;
    if ( m_velocities ) delete [] m_velocities;
    if ( m_pressures ) delete [] m_pressures;
}

/*===========================================================================*/
/**
 *  @brief  Return dimensions.
 *  @return dimensions
 */
/*===========================================================================*/
size_t FlowData::dimensions() const
{
    return m_dimensions;
}

/*===========================================================================*/
/**
 *  @brief  Return number of nodes.
 *  @return number of nodes
 */
/*===========================================================================*/
size_t FlowData::nnodes() const
{
    return m_nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Return number of elements.
 *  @return number of elements
 */
/*===========================================================================*/
size_t FlowData::nelements() const
{
    return m_nelements;
}

/*===========================================================================*/
/**
 *  @brief  Return number of time steps
 *  @return number of time steps
 */
/*===========================================================================*/
size_t FlowData::nsteps() const
{
    return m_nsteps;
}

/*===========================================================================*/
/**
 *  @brief  Return time value specified by the index.
 *  @param  index [in] index of time value array
 *  @return time value
 */
/*===========================================================================*/
kvs::Real32 FlowData::time( const size_t index ) const
{
    KVS_ASSERT( index < m_nsteps );
    return m_times[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Return step value specified by the index.
 *  @param  index [in] index of step value array
 *  @return step value
 */
/*===========================================================================*/
kvs::Int32 FlowData::step( const size_t index ) const
{
    KVS_ASSERT( index < m_nsteps );
    return m_steps[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Return velocity value array specified by the index.
 *  @param  index [in] index of velocity value array list
 *  @return velocity value array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32>& FlowData::velocities( const size_t index ) const
{
    KVS_ASSERT( index < m_nsteps );
    return m_velocities[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Return pressure value array specified by the index.
 *  @param  index [in] index of pressure value array list
 *  @return pressure value array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32>& FlowData::pressures( const size_t index ) const
{
    KVS_ASSERT( index < m_nsteps );
    return m_pressures[ index ];
}

void FlowData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Dimensions : " << m_dimensions << std::endl;
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of elements : " << m_nelements << std::endl;
    os << indent << "Number of time steps : " << m_nsteps << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read flow data file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FlowData::read( const std::string filename )
{
    kvs::gf::File file;
    if ( !file.read( filename ) )
    {
        kvsMessageError("Cannot read mesh data file.");
        return false;
    }

    if ( m_times ) delete [] m_times;
    if ( m_steps ) delete [] m_steps;
    if ( m_velocities ) delete [] m_velocities;
    if ( m_pressures ) delete [] m_pressures;

    m_nsteps = file.dataSetList().size();
    m_times = new kvs::Real32 [ m_nsteps ];
    m_steps = new kvs::Int32 [ m_nsteps ];
    m_velocities = new kvs::ValueArray<kvs::Real32> [ m_nsteps ];
    m_pressures = new kvs::ValueArray<kvs::Real32> [ m_nsteps ];

    for ( size_t i = 0; i < m_nsteps; i++ )
    {
        const kvs::gf::DataSet& data_set = file.dataSet(i);
        for ( size_t j = 0; j < data_set.dataList().size(); j++ )
        {
            const kvs::gf::Data& data = data_set.data(j);
            const std::string& keyword = data.keyword();
            const std::string& type = data.arrayTypeHeader();

            // Read time.
            if ( keyword == "*TIME_PS" )
            {
                if ( type == "#FLT_ARY" )
                {
                    m_times[i] = data.fltArray().front();
                }
            }

            // Read step.
            if ( keyword == "*STEP_PS" )
            {
                if ( type == "#INT_ARY" )
                {
                    m_steps[i] = data.intArray().front();
                }
            }

            // Read velocity values.
            if ( keyword == "*VELO_2D" || keyword == "*VELO_3D" )
            {
                if ( type == "#FLT_ARY" )
                {
                    m_dimensions = data.num();
                    m_nnodes = data.num2();
                    m_velocities[i] = data.fltArray();
                }
            }

            // Read pressure values.
            if ( keyword == "*PRES_2E" || keyword == "*PRES_3E" )
            {
                if ( type == "#FLT_ARY" )
                {
                    m_nelements = data.num2();
                    m_pressures[i] = data.fltArray();
                }
            }
        }
    }

    return true;
}

} // end of namespace gf

} // end of namespace kvs
