/*****************************************************************************/
/**
 *  @file   GSIMesh.h
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
/*----------------------------------------------------------------------------
 *
 * Format information:
 * Degital Elevation Model (DEM) data from Geospatial Information Authority
 * of Japan (http://www.gsi.go.jp/index.html).
 *     250m mesh: http://www.gsi.go.jp/geoinfo/dmap/dem250m-index.html
 *      50m mesh: http://www.gsi.go.jp/geoinfo/dmap/dem50m-index.html
 */
/*****************************************************************************/
#ifndef KVS__GIS__GSI_MESH_H_INCLUDE
#define KVS__GIS__GSI_MESH_H_INCLUDE

#include "Mesh.h"
#include "Area.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <kvs/Vector2>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  GSI (Geospatial Information Authority of Japan) mesh class.
 */
/*===========================================================================*/
class GSIMesh : public kvs::gis::Mesh
{
public:

    typedef kvs::gis::Mesh BaseClass;

    enum Type
    {
        Mesh1KM,
        Mesh250M,
        Mesh50M
    };

private:

    Type m_type; ///< mesh type
    size_t m_code; ///< mesh code
    size_t m_scale; ///< scale of the maps
    size_t m_survey_year; ///< survey year of the maps
    size_t m_modified_year; ///< modified year of the maps
    size_t m_digitalisation_year; ///< digitalisation year of the maps
    size_t m_foliar_num; ///< number of foliars
    char m_first_name[20]; ///< name of the 1st map
    bool m_first_flg; ///< flag for the 1st map
    char m_second_name[20]; ///< name of the 2nd map
    bool m_second_flg; ///< flag for the 2nd map
    char m_third_name[20]; ///< name of the 3rd map
    bool m_third_flg; ///< flag for the 3rd map
    char m_forth_name[20]; ///< name of the 4th map
    bool m_forth_flg; ///< flag for the 4th map
    size_t m_record_num; ///< number of records
    char m_comment[80]; ///< comment
    bool m_record_flg[320]; ///< a set of record flag

public:

    GSIMesh();
    GSIMesh( const std::string& filename, const kvs::gis::Area& area );
    virtual ~GSIMesh();

public:

    bool read( const std::string& filename, const kvs::gis::Area& area );

private:

    bool read_header( FILE* fp );
    bool read_data( FILE* fp, const kvs::gis::Area& area );
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__GSI_MESH_H_INCLUDE
