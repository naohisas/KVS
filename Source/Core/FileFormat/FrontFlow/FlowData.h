/*****************************************************************************/
/**
 *  @file   FlowData.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FlowData.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF__FLOW_DATA_H_INCLUDE
#define KVS__GF__FLOW_DATA_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Indent>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  GF Flow data class.
 */
/*===========================================================================*/
class FlowData
{
private:

    size_t m_dimensions; ///< dimensions (2 or 3)
    size_t m_nnodes; ///< number of nodes
    size_t m_nelements; ///< number of elements
    size_t m_nsteps; ///< number of time steps
    kvs::Real32* m_times; ///< time values
    kvs::Int32* m_steps; ///< step values
    kvs::ValueArray<kvs::Real32>* m_velocities; ///< velocity values
    kvs::ValueArray<kvs::Real32>* m_pressures; ///< pressure values

public:

    FlowData();
    FlowData( const std::string filename );
    virtual ~FlowData();

    size_t dimensions() const;
    size_t nnodes() const;
    size_t nelements() const;
    size_t nsteps() const;
    kvs::Real32 time( const size_t index ) const;
    kvs::Int32 step( const size_t index ) const;
    const kvs::ValueArray<kvs::Real32>& velocities( const size_t index ) const;
    const kvs::ValueArray<kvs::Real32>& pressures( const size_t index ) const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string filename );
};

} // end of namespace gf

} // end of namespace kvs

#endif // KVS__GF__FLOW_DATA_H_INCLUDE
