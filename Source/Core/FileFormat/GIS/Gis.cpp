/*****************************************************************************/
/**
 *  @file   Gis.cpp
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
#include "Gis.h"
#include "USGSMesh.h"
#include "GSIMesh.h"
#include <kvs/Tokenizer>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Directory>
#include <kvs/File>


namespace kvs
{

bool Gis::CheckExtension( const std::string& filename )
{
    if ( filename.find(";") != std::string::npos )
    {
        std::string hdr; // header file
        std::string bil; // data file
        kvs::Tokenizer t( filename, ";" );
        while ( !t.isLast() )
        {
            const kvs::File file( t.token() );
            if ( file.extension() == "hdr" || file.extension() == "HDR" ) { hdr = file.filePath(); }
            if ( file.extension() == "res" || file.extension() == "RES" ) { bil = file.filePath(); }
        }
        if ( !hdr.empty() && !bil.empty() ) return true;
    }
    else
    {
        const kvs::File file( filename );
        if ( file.extension() == "tem" || file.extension() == "TEM" ||
             file.extension() == "sem" || file.extension() == "SEM" ||
             file.extension() == "mem" || file.extension() == "MEM" )
        {
            return true;
        }
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GIS class.
 */
/*===========================================================================*/
Gis::Gis():
    m_ocean_color( 0, 0, 128 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GIS class.
 *  @param  filename [in] filename
 *  @param  render_area [in] render area of the mesh
 */
/*===========================================================================*/
Gis::Gis( const std::string& filename, const kvs::gis::Area& render_area ):
    m_ocean_color( 0, 0, 128 )
{
    this->setRenderArea( render_area );
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the GIS class.
 */
/*===========================================================================*/
Gis::~Gis()
{
    for ( size_t i = 0; i < m_meshes.size(); i++ )
    {
        if ( m_meshes[i] ) delete m_meshes[i];
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the render area of the mesh.
 *  @return area of the meshe
 */
/*===========================================================================*/
const kvs::gis::Area& Gis::renderArea() const
{
    return m_render_area;
}

/*===========================================================================*/
/**
 *  @brief  Returns the area of all meshes.
 *  @return area
 */
/*===========================================================================*/
const kvs::gis::Area& Gis::area() const
{
    return m_area;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mesh list.
 *  @return mesh list
 */
/*===========================================================================*/
const std::vector<kvs::gis::Mesh*>& Gis::meshes() const
{
    return m_meshes;
}

/*===========================================================================*/
/**
 *  @brief  Return the mesh.
 *  @param  index [in] index of the mesh
 *  @return pointer to the mesh
 */
/*===========================================================================*/
const kvs::gis::Mesh* Gis::mesh( const size_t index ) const
{
    return m_meshes[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Returns the ocean color.
 *  @return ocean color
 */
/*===========================================================================*/
kvs::RGBColor Gis::oceanColor() const
{
    return m_ocean_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets render area of the mesh.
 *  @param  render_area [in] render area
 */
/*===========================================================================*/
void Gis::setRenderArea( const kvs::gis::Area& render_area )
{
    m_render_area = render_area;
}

/*===========================================================================*/
/**
 *  @brief  Sets an ocean color.
 *  @param  color [in] ocean color
 */
/*===========================================================================*/
void Gis::setOceanColor( const kvs::RGBColor& color )
{
    m_ocean_color = color;
}

void Gis::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Area of all meshes : " << std::endl;
    m_area.print( os, indent.nextIndent() );
    os << indent << "Render area : " << std::endl;
    m_render_area.print( os, indent.nextIndent() );
    os << indent << "Ocean color : " << m_ocean_color << std::endl;
    os << indent << "Number of meshes : " << m_meshes.size() << std::endl;
    for ( size_t i = 0; i < m_meshes.size(); i++ )
    {
        os << indent << "Mesh [" << i << "] : " << std::endl;
        m_meshes[i]->print( os, indent.nextIndent() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads GIS mesh data file.
 *  @param  filenames [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Gis::read( const std::string& filenames )
{
    BaseClass::setFilename( filenames );
    BaseClass::setSuccess( true );

    if ( kvs::Directory( filenames ).isDirectory() )
    {
        kvs::Directory directory( filenames );

        const size_t nfiles = directory.fileList().size();
        for ( size_t i = 0; i < nfiles; i++ )
        {
            const std::string file = directory.fileList().at(i).filePath( true );
            if ( !this->read_mesh( file ) )
            {
                kvsMessageError("Cannot read the mesh data.");
                BaseClass::setSuccess( false );
                return false;
            }
        }
    }
    else
    {
        const std::string file = filenames;
        if ( !this->read_mesh( file ) )
        {
            kvsMessageError("Cannot read the mesh data.");
            BaseClass::setSuccess( false );
            return false;
        }
    }

    float min_latitude =  9999.0f;
    float max_latitude = -9999.0f;
    float min_longitude =  9999.0f;
    float max_longitude = -9999.0f;
    float min_height =  9999.0f;
    float max_height = -9999.0f;
    for ( size_t i = 0; i < m_meshes.size(); i++ )
    {
        if ( m_meshes[i]->hasData() )
        {
            min_latitude = kvs::Math::Min( min_latitude, m_meshes[i]->area().minLatitude() );
            max_latitude = kvs::Math::Max( max_latitude,  m_meshes[i]->area().maxLatitude() );
            min_longitude = kvs::Math::Min( min_longitude, m_meshes[i]->area().minLongitude() );
            max_longitude = kvs::Math::Max( max_longitude, m_meshes[i]->area().maxLongitude() );
            min_height = kvs::Math::Min( min_height, m_meshes[i]->area().minHeight() );
            max_height = kvs::Math::Max( max_height, m_meshes[i]->area().maxHeight() );
        }
    }

    m_area.setMinLatitude( min_latitude );
    m_area.setMaxLatitude( max_latitude );
    m_area.setMinLongitude( min_longitude );
    m_area.setMaxLongitude( max_longitude );
    m_area.setMinHeight( min_height );
    m_area.setMaxHeight( max_height );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads mesh data file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Gis::read_mesh( const std::string& filename )
{
    kvs::gis::Mesh* mesh = NULL;
    if ( filename.find(";") != std::string::npos )
    {
        std::string hdr; // header file
        std::string bil; // data file
        kvs::Tokenizer t( filename, ";" );
        while ( !t.isLast() )
        {
            const std::string f = t.token();
            if ( f.find( ".hdr" ) != std::string::npos ||
                 f.find( ".HDR" ) != std::string::npos ) { hdr = f; }
            else if ( f.find( ".res" ) != std::string::npos ||
                      f.find( ".RES" ) != std::string::npos ) { bil = f; }
        }

        mesh = new kvs::gis::USGSMesh( hdr, bil, m_render_area );
    }
    else
    {
        mesh = new kvs::gis::GSIMesh( filename, m_render_area );
    }

    m_meshes.push_back( mesh );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes GIS mesh data file.
 *  @param  filename [in] filename
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Gis::write( const std::string& filename )
{
    // This method has not been implemented yet.
    kvs::IgnoreUnusedVariable( filename );
    return false;
}

} // end of namespace kvs
