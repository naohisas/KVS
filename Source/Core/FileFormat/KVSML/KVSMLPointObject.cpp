/****************************************************************************/
/**
 *  @file   KVSMLPointObject.cpp
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
/****************************************************************************/
#include "KVSMLPointObject.h"
#include "PointObjectTag.h"
#include "VertexTag.h"
#include "CoordTag.h"
#include "ColorTag.h"
#include "NormalTag.h"
#include "SizeTag.h"
#include "DataArrayTag.h"
#include "DataValueTag.h"
#include "DataReader.h"
#include "DataWriter.h"
#include <kvs/XMLDocument>
#include <kvs/XMLDeclaration>
#include <kvs/XMLElement>
#include <kvs/XMLComment>
#include <kvs/ValueArray>
#include <kvs/File>
#include <kvs/Type>
#include <kvs/File>
#include <kvs/IgnoreUnusedVariable>
#include <iostream>
#include <fstream>
#include <sstream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given filename has the supported extension
 */
/*===========================================================================*/
bool KVSMLPointObject::CheckExtension( const std::string& filename )
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
 *  @return true, if the KVSMLPointObject class can read the given file
 */
/*===========================================================================*/
bool KVSMLPointObject::CheckFormat( const std::string& filename )
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

    // <PointObject>
    const std::string point_tag("PointObject");
    const kvs::XMLNode::SuperClass* point_node = kvs::XMLNode::FindChildNode( object_node, point_tag );
    if ( !point_node ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML point object class.
 */
/*===========================================================================*/
KVSMLPointObject::KVSMLPointObject():
    m_writing_type( kvs::KVSMLPointObject::Ascii )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML point object class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLPointObject::KVSMLPointObject( const std::string& filename ):
    m_writing_type( kvs::KVSMLPointObject::Ascii )
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
void KVSMLPointObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of vertices: " << m_coords.size() / 3;
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
 *  @brief  Read a KVSMl point object file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is successfully
 */
/*===========================================================================*/
bool KVSMLPointObject::read( const std::string& filename )
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

    // <PointObject>
    kvs::kvsml::PointObjectTag point_tag;
    if ( !point_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", point_tag.name().c_str() );
        return false;
    }

    // <Vertex>
    kvs::kvsml::VertexTag vertex_tag;
    if ( !vertex_tag.read( point_tag.node() ) )
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
        const size_t ncolors = vertex_tag.nvertices();
        if ( !kvs::kvsml::ReadColorData( parent, ncolors, &m_colors ) )
        {
            return false;
        }

        if ( m_colors.size() == 0 )
        {
            // default value (black).
            m_colors.allocate(3);
            m_colors[0] = 0;
            m_colors[1] = 0;
            m_colors[2] = 0;
        }

        // <Normal>
        const size_t nnormals = vertex_tag.nvertices();
        if ( !kvs::kvsml::ReadNormalData( parent, nnormals, &m_normals ) )
        {
            return false;
        }

        // <Size>
        const size_t nsizes = vertex_tag.nvertices();
        if ( !kvs::kvsml::ReadSizeData( parent, nsizes, &m_sizes ) )
        {
            return false;
        }
    }

    BaseClass::setSuccess( true );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML point object.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool KVSMLPointObject::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document
    std::string comment(" Generated by kvs::KVSMLPointObject::write() ");
    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment( comment.c_str() ) );

    // <KVSML>
    m_kvsml_tag.write( &document );

    // <Object type="PointObject"
    //         object_coord="xxx xxx xxx xxx xxx xxx"
    //         external_coord="xxx xxx xxx xxx xxx xxx">
    m_object_tag.setType( "PointObject" );
    if ( !m_object_tag.write( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <PointObject>
    kvs::kvsml::PointObjectTag point_tag;
    if ( !point_tag.write( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", point_tag.name().c_str() );
        return false;
    }

    // <Vertex nvertices="xxx">
    const size_t dimension = 3;
    kvs::kvsml::VertexTag vertex_tag;
    vertex_tag.setNVertices( m_coords.size() / dimension );
    if ( !vertex_tag.write( point_tag.node() ) )
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
        if ( !kvs::kvsml::WriteColorData( parent, type, filename, m_colors ) )
        {
            return false;
        }

        // <Normal>
        if ( !kvs::kvsml::WriteNormalData( parent, type, filename, m_normals ) )
        {
            return false;
        }

        // <Size>
        if ( !kvs::kvsml::WriteSizeData( parent, type, filename, m_sizes ) )
        {
            return false;
        }
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs
