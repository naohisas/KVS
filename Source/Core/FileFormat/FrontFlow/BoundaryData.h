/*****************************************************************************/
/**
 *  @file   BoundaryData.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BoundaryData.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF__BOUNDARY_H_INCLUDE
#define KVS__GF__BOUNDARY_H_INCLUDE

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
 *  @brief  GF Boundary condition data class.
 */
/*===========================================================================*/
class BoundaryData
{
private:

    size_t m_dimensions; ///< dimensions (2 or 3)
    size_t m_nnodes_inlet; ///< number of inlet boundary nodes
    size_t m_nnodes_wall; ///< number of wall boundary nodes
    size_t m_nnodes_symetric; ///< number of symetric boundary nodes
    size_t m_nnodes_cyclic; ///< number of cyclic boundary nodes
    size_t m_nnodes_body; ///< number of body surface boundary nodes
    size_t m_nnodes_free; ///< number of free boundary nodes
    size_t m_nnodes_moving_wall; ///< number of moving wall boundary nodes
    size_t m_nnodes_inter_connect; ///< number of inter connect boundary nodes
    size_t m_nnodes_temp; ///< number of temperature boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_inlet; ///< inlet boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_wall; ///< wall boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_symetric; ///< symetric boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_cyclic; ///< cyclic boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_body; ///< body surface boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_free; ///< free boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_moving_wall; ///< moving wall boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_temp; ///< temperature boundary nodes
    kvs::ValueArray<kvs::UInt32> m_connections_heat; ///< heat flux boundary nodes
    kvs::ValueArray<kvs::Real32> m_velocities_inlet; ///< inlet boundary velocities
    kvs::ValueArray<kvs::Real32> m_velocities_wall; ///< wall boundary velocities
    kvs::ValueArray<kvs::Int32>  m_inter_connects; ///< inter connection datasets
    kvs::ValueArray<kvs::Real32> m_temperatures; ///< boundary temperatures
    kvs::ValueArray<kvs::Real32> m_heat_fluxes; ///< boundary heat fluxes

public:

    BoundaryData();
    BoundaryData( const std::string filename );

    size_t dimensions() const;
    size_t nnodesInlet() const;
    size_t nnodesWall() const;
    size_t nnodesSymetric() const;
    size_t nnodesCyclic() const;
    size_t nnodesBody() const;
    size_t nnodesFree() const;
    size_t nnodesMovingWall() const;
    size_t nnodesInterConnect() const;
    size_t nnodesTemp() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsInlet() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsWall() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsSymetric() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsCyclic() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsBody() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsFree() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsMovingWall() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsTemp() const;
    const kvs::ValueArray<kvs::UInt32>& connectionsHeat() const;
    const kvs::ValueArray<kvs::Real32>& velocitiesInlet() const;
    const kvs::ValueArray<kvs::Real32>& velocitiesWall() const;
    const kvs::ValueArray<kvs::Int32>&  interConnects() const;
    const kvs::ValueArray<kvs::Real32>& temperatures() const;
    const kvs::ValueArray<kvs::Real32>& heatFluxes() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string filename );
};

} // end of namespace gf

} // end of namespace kvs

#endif // KVS__GF__BOUNDARY_H_INCLUDE
