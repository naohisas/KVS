/*****************************************************************************/
/**
 *  @file   USGSMesh.h
 *  @author Naohisa Sakamoto
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
#ifndef KVS__GIS__USGS_MESH_H_INCLUDE
#define KVS__GIS__USGS_MESH_H_INCLUDE

#include "Mesh.h"
#include "Area.h"
#include <kvs/Vector2>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  USGS (U.S. Geological Survey) mesh class.
 */
/*===========================================================================*/
class USGSMesh : public kvs::gis::Mesh
{
public:

    typedef kvs::gis::Mesh BaseClass;

private:

    int m_nbands; ///< number of bands
    int m_nbits; ///< number of bits
    int m_band_row_bytes; ///< 
    int m_total_row_bytes; ///< 
    int m_band_gap_bytes; ///< 
    short m_ocean_value; ///< 

public:

    USGSMesh();
    USGSMesh( const std::string& header_filename, const std::string& data_filename, const kvs::gis::Area& area );
    virtual ~USGSMesh();

public:

    bool read( const std::string& header_filename, const std::string& data_filename, const kvs::gis::Area& area );

private:

    bool read_header( FILE* fp );
    bool read_data( FILE* fp, const kvs::gis::Area& area );
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__USGS_MESH_H_INCLUDE
