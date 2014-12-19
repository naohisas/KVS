/*****************************************************************************/
/**
 *  @file   GFData.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GFData.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF_DATA_H_INCLUDE
#define KVS__GF_DATA_H_INCLUDE

#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include "FlowData.h"
#include "MeshData.h"
#include "BoundaryData.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  GF file format class.
 */
/*===========================================================================*/
class GFData : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

private:

    kvs::gf::MeshData m_mesh_data; ///< GF mesh data
    kvs::gf::FlowData m_flow_data; ///< GF flow data
    kvs::gf::BoundaryData m_boundary_data; ///< GF doundary condition data

public:

    static bool CheckExtension( const std::string& filename );

public:

    GFData();
    GFData( const std::string& filename );
    GFData( const std::string& mesh_file, const std::string& flow_file, const std::string& boundary_file = "" );

    const kvs::gf::FlowData& flowData() const;
    const kvs::gf::MeshData& meshData() const;
    const kvs::gf::BoundaryData& boundaryData() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool read( const std::string& mesh_file, const std::string& flow_file, const std::string& boundary_file = "" );

private:

    bool write( const std::string& filename );
};

} // end of namespace kvs

#endif // KVS__GF_DATA_H_INCLUDE
