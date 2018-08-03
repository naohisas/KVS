/****************************************************************************/
/**
 *  @file   KVSMLLineObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLLineObject.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "KVSMLLineObject.h"
#include "LineObjectTag.h"
#include "LineTag.h"
#include "VertexTag.h"
#include "CoordTag.h"
#include "ColorTag.h"
#include "ConnectionTag.h"
#include "SizeTag.h"
#include "DataArrayTag.h"
#include "DataValueTag.h"
#include "DataReader.h"
#include "DataWriter.h"
#include <kvs/File>
#include <kvs/XMLDocument>
#include <kvs/XMLDeclaration>
#include <kvs/XMLElement>
#include <kvs/XMLComment>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given filename has the supported extension
 */
/*===========================================================================*/
bool KVSMLLineObject::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "kvsml" || file.extension() == "KVSML" ||
         file.extension() == "xml"   || file.extension() == "XML" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Check the file format.
 *  @param  filename [in] filename
 *  @return true, if the KVSMLLineObject class can read the given file
 */
/*===========================================================================*/
bool KVSMLLineObject::CheckFormat( const std::string& filename )
{
    kvs::XMLDocument document;
    if ( !document.read( filename ) ) return false;

    // <KVSML>
    const std::string kvsml_tag("KVSML");
    const kvs::XMLNode::SuperClass* kvsml_node = kvs::XMLDocument::FindNode( &document, kvsml_tag );
    if ( !kvsml_node ) return false;

    // <Object>
    const std::string object_tag("Object");
    const kvs::XMLNode::SuperClass* object_node = kvs::XMLNode::FindChildNode( kvsml_node, object_tag );
    if ( !object_node ) return false;

    // <LineObject>
    const std::string line_tag("LineObject");
    const kvs::XMLNode::SuperClass* line_node = kvs::XMLNode::FindChildNode( object_node, line_tag );
    if ( !line_node ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML line object class.
 */
/*===========================================================================*/
KVSMLLineObject::KVSMLLineObject():
    m_writing_type( kvs::KVSMLLineObject::Ascii )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML line object class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLLineObject::KVSMLLineObject( const std::string& filename ):
    m_writing_type( kvs::KVSMLLineObject::Ascii )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Prints file information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent for each line
 */
/*===========================================================================*/
void KVSMLLineObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Line type : " << m_line_type << std::endl;
    os << indent << "Color type : " << m_color_type << std::endl;
    os << indent << "Number of vertices : " << m_coords.size() / 3 << std::endl;
    if ( m_object_tag.hasObjectCoord() )
    {
        os << indent << "Min object coord : " << m_object_tag.minObjectCoord() << std::endl;
        os << indent << "Max object coord : " << m_object_tag.maxObjectCoord() << std::endl;
    }
    if ( m_object_tag.hasExternalCoord() )
    {
        os << indent << "Min external coord : " << m_object_tag.minExternalCoord() << std::endl;
        os << indent << "Max external coord : " << m_object_tag.maxExternalCoord() << std::endl;
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads a KVSML line object file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool KVSMLLineObject::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document.
    kvs::XMLDocument document;
    if ( !document.read( filename ) )
    {
        kvsMessageError( "%s", document.ErrorDesc().c_str() );
        return false;
    }

    // <KVSML>
    m_kvsml_tag.read( &document );

    // <Object>
    if ( !m_object_tag.read( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <LineObject>
    kvs::kvsml::LineObjectTag line_object_tag;
    if ( !line_object_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", line_object_tag.name().c_str() );
        return false;
    }

    m_line_type = "strip"; // default
    if ( line_object_tag.hasLineType() )
    {
        // strip, uniline, polyline or segment
        m_line_type = line_object_tag.lineType();
    }

    m_color_type = "line"; // default
    if ( line_object_tag.hasColorType() )
    {
        // line or vertex
        m_color_type = line_object_tag.colorType();
    }

    // <Vertex>
    kvs::kvsml::VertexTag vertex_tag;
    if ( !vertex_tag.read( line_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", vertex_tag.name().c_str() );
        return false;
    }
    else
    {
        // Parent node.
        const kvs::XMLNode::SuperClass* parent = vertex_tag.node();

        // <Coord>
        const size_t ncoords = vertex_tag.nvertices();
        if ( !kvs::kvsml::ReadCoordData( parent, ncoords, &m_coords ) ) 
        {
            return false;
        }

        if ( m_coords.size() == 0 )
        {
            kvsMessageError( "Cannot read the coord data." );
            return false;
        }

        // <Color>
        if ( m_color_type == "vertex" )
        {
            const size_t ncolors = vertex_tag.nvertices();
            if ( !kvs::kvsml::ReadColorData( parent, ncolors, &m_colors ) )
            {
                return false;
            }
        }
        if ( m_colors.size() == 0 )
        {
            // default value (black).
            m_colors.allocate(3);
            m_colors[0] = 0;
            m_colors[1] = 0;
            m_colors[2] = 0;
        }

        // <Size>
        // WARNING: This tag should be described under <Line> since the size that is
        // described in <Size> represents the line size not the vertex size. Therefore,
        // we recommend that the size is described under <Line> not under <Vertex>.
        // Current version of KVSML tentative supports both of the descriptions for the
        // size in order to maintain backward compatibility.
        const size_t nsizes = vertex_tag.nvertices();
        if ( !kvs::kvsml::ReadSizeData( parent, nsizes, &m_sizes ) )
        {
            return false;
        }

        if ( m_sizes.size() == 0 )
        {
            // default value (1).
            m_sizes.allocate(1);
            m_sizes[0] = 1;
        }
    }

    // <Line>
    kvs::kvsml::LineTag line_tag;
    if ( line_tag.isExisted( line_object_tag.node() ) )
    {
        if ( !line_tag.read( line_object_tag.node() ) )
        {
            kvsMessageError( "Cannot read <%s>.", line_tag.name().c_str() );
            return false;
        }
        else
        {
            // Parent node.
            const kvs::XMLNode::SuperClass* parent = line_tag.node();
            const size_t nvertices = vertex_tag.nvertices();
            const size_t nlines = line_tag.nlines();

            // <Size>
            const size_t nsizes = nlines;
            if ( !kvs::kvsml::ReadSizeData( parent, nsizes, &m_sizes ) )
            {
                return false;
            }

            if ( m_sizes.size() == 0 )
            {
                // default value (1).
                m_sizes.allocate(1);
                m_sizes[0] = 1;
            }

            // <Color>
            if ( m_color_type == "line" )
            {
                const size_t ncolors =
                    ( m_line_type == "strip"    ) ? nvertices - 1 :
                    ( m_line_type == "uniline"  ) ? nlines - 1 :
                    ( m_line_type == "polyline" ) ? nlines :
                    ( m_line_type == "segment"  ) ? nlines : 0;
                if ( !kvs::kvsml::ReadColorData( parent, ncolors, &m_colors ) )
                {
                    return false;
                }
            }

            if ( m_colors.size() == 0 )
            {
                // default value (black).
                m_colors.allocate(3);
                m_colors[0] = 0;
                m_colors[1] = 0;
                m_colors[2] = 0;
            }

            // <Connection>
            if ( m_line_type != "strip" )
            {
                const size_t nconnections =
                    ( m_line_type == "uniline"  ) ? nlines :
                    ( m_line_type == "polyline" ) ? nlines * 2 :
                    ( m_line_type == "segment"  ) ? nlines * 2 : 0;
                if ( !kvs::kvsml::ReadConnectionData( parent, nconnections, &m_connections ) )
                {
                    return false;
                }
            }

        }
    }
    else
    {
        if ( m_line_type != "strip" || m_color_type == "line" )
        {
            kvsMessageError( "Cannot find <%s>.", line_tag.name().c_str() );
            return false;
        }
    }

    BaseClass::setSuccess( true );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML line object.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool KVSMLLineObject::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document
    std::string comment(" Generated by kvs::KVSMLLineObject::write() ");
    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment( comment.c_str() ) );

    // <KVSML>
    m_kvsml_tag.write( &document );

    // <Object type="LineObject">
    m_object_tag.setType( "LineObject" );
    if ( !m_object_tag.write( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <LineObject>
    kvs::kvsml::LineObjectTag line_object_tag;
    line_object_tag.setLineType( m_line_type );
    line_object_tag.setColorType( m_color_type );
    if ( !line_object_tag.write( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", line_object_tag.name().c_str() );
        return false;
    }

    const size_t dimension = 3;
    const size_t nvertices = m_coords.size() / dimension;

    // <Vertex nvertices="xxx">
    kvs::kvsml::VertexTag vertex_tag;
    vertex_tag.setNVertices( nvertices );
    if ( !vertex_tag.write( line_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", vertex_tag.name().c_str() );
        return false;
    }
    else
    {
        // Parent node and writing data type.
        kvs::XMLNode::SuperClass* parent = vertex_tag.node();
        const kvs::kvsml::WritingDataType type = static_cast<kvs::kvsml::WritingDataType>(m_writing_type);

        // <Coord>
        if ( !kvs::kvsml::WriteCoordData( parent, type, filename, m_coords ) )
        {
            return false;
        }

        // <Color>
        if ( m_color_type == "vertex" )
        {
            if ( !kvs::kvsml::WriteColorData( parent, type, filename, m_colors ) )
            {
                return false;
            }
        }
    }

    // <Line nlines="xxx">
    if ( m_color_type == "line" || m_sizes.size() > 0 || m_connections.size() > 0 )
    {
        const size_t nconnections = m_connections.size();
        const size_t nlines =
            ( m_line_type == "strip"   ) ? nvertices - 1 :
            ( m_line_type == "uniline" ) ? nconnections : nconnections / 2;

        kvs::kvsml::LineTag line_tag;
        line_tag.setNLines( nlines );
        if ( !line_tag.write( line_object_tag.node() ) )
        {
            kvsMessageError( "Cannot write <%s>.", line_tag.name().c_str() );
            return false;
        }

        // Parent node and writing data type.
        kvs::XMLNode::SuperClass* parent = line_tag.node();
        const kvs::kvsml::WritingDataType type = static_cast<kvs::kvsml::WritingDataType>(m_writing_type);

        // <Connection>
        if ( !kvs::kvsml::WriteConnectionData( parent, type, filename, m_connections ) )
        {
            return false;
        }

        // <Size>
        if ( !kvs::kvsml::WriteSizeData( parent, type, filename, m_sizes ) )
        {
            return false;
        }

        // <Color>
        if ( m_color_type == "line" )
        {
            if ( !kvs::kvsml::WriteColorData( parent, type, filename, m_colors ) )
            {
                return false;
            }
        }
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs
