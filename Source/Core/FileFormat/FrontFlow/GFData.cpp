/*****************************************************************************/
/**
 *  @file   GFData.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GFData.cpp 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GFData.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Tokenizer>


namespace kvs
{

bool GFData::CheckExtension( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Construct a new GFData class.
 */
/*===========================================================================*/
GFData::GFData()
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new GFData class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
GFData::GFData( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Construct a new GFData class.
 *  @param  mesh_file [in] filename of mesh data
 *  @param  flow_file [in] filename of flow data
 *  @param  boundary_file [in] filename of doundary data
 */
/*===========================================================================*/
GFData::GFData( const std::string& mesh_file, const std::string& flow_file, const std::string& boundary_file )
{
    this->read( mesh_file, flow_file, boundary_file );
}

/*===========================================================================*/
/**
 *  @brief  Return flow data.
 *  @return flow data
 */
/*===========================================================================*/
const kvs::gf::FlowData& GFData::flowData() const
{
    return m_flow_data;
}

/*===========================================================================*/
/**
 *  @brief  Return mesh data.
 *  @return mesh data
 */
/*===========================================================================*/
const kvs::gf::MeshData& GFData::meshData() const
{
    return m_mesh_data;
}

/*===========================================================================*/
/**
 *  @brief  Return doundary data.
 *  @return boundary data
 */
/*===========================================================================*/
const kvs::gf::BoundaryData& GFData::boundaryData() const
{
    return m_boundary_data;
}

void GFData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Mesh data : " << std::endl;
    m_mesh_data.print( os, indent.nextIndent() );
    os << indent << "Flow data : " << std::endl;
    m_flow_data.print( os, indent.nextIndent() );
    os << indent << "Boundary data : " << std::endl;
    m_boundary_data.print( os, indent.nextIndent() );
}

/*===========================================================================*/
/**
 *  @brief  Read GF data file (not implemented).
 *  @param  filename [in] filename (ex. "aaa.mesh;bbb.flow;ccc.boun")
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool GFData::read( const std::string& filename )
{
    kvs::Tokenizer t( filename, ";" );
    const std::string mesh_file = t.isLast() ? "" : t.token();
    const std::string flow_file = t.isLast() ? "" : t.token();
    const std::string boundary_file = t.isLast() ? "" : t.token();

    return this->read( mesh_file, flow_file, boundary_file );
}

/*===========================================================================*/
/**
 *  @brief  Read GF data files composed of flow, mesh and boundary data file.
 *  @param  mesh_file [in] filename of mesh data
 *  @param  flow_file [in] filename of flow data
 *  @param  boundary_file [in] filename of boundary data
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool GFData::read( const std::string& mesh_file, const std::string& flow_file, const std::string& boundary_file )
{
    std::string filename = mesh_file + ";" + flow_file;
    if ( boundary_file != "" ) filename += ";" + boundary_file;
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    if ( !m_mesh_data.read( mesh_file ) )
    {
        kvsMessageError("Cannot read mesh file %s.", mesh_file.c_str() );
        BaseClass::setSuccess( false );
    }

    if ( !m_flow_data.read( flow_file ) )
    {
        kvsMessageError("Cannot read flow file %s.", flow_file.c_str() );
        BaseClass::setSuccess( false );
    }

    if ( boundary_file != "" )
    {
        if ( !m_boundary_data.read( boundary_file ) )
        {
            kvsMessageError("Cannot read bounday file %s.", boundary_file.c_str() );
            BaseClass::setSuccess( false );
        }
    }

    return BaseClass::isSuccess();
}

/*===========================================================================*/
/**
 *  @brief  Write GF data files (not implemented).
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool GFData::write( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return true;
}

} // end of namespace kvs
