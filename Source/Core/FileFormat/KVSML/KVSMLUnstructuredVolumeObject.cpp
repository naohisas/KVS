/****************************************************************************/
/**
 *  @file   KVSMLUnstructuredVolumeObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLUnstructuredVolumeObject.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "KVSMLUnstructuredVolumeObject.h"
#include "UnstructuredVolumeObjectTag.h"
#include "NodeTag.h"
#include "CellTag.h"
#include "ValueTag.h"
#include "CoordTag.h"
#include "ConnectionTag.h"
#include "DataArrayTag.h"
#include <kvs/File>
#include <kvs/XMLDocument>
#include <kvs/XMLDeclaration>
#include <kvs/XMLElement>
#include <kvs/XMLComment>
#include <kvs/AnyValueArray>
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns the number of nodes per element.
 *  @param  cell_type [in] cell type as string
 *  @return number of nodes per element
 */
/*===========================================================================*/
inline size_t GetNumberOfNodesPerElement( const std::string& cell_type )
{
    size_t nnodes = 0;
    if ( cell_type == "tetrahedra" ) nnodes = 4;
    else if ( cell_type == "quadratic tetrahedra" ) nnodes = 10;
    else if ( cell_type == "hexahedra" ) nnodes = 8;
    else if ( cell_type == "quadratic hexahedra" ) nnodes = 20;
    else if ( cell_type == "pyramid" ) nnodes = 5;
    else if ( cell_type == "point" ) nnodes = 1;
    else if ( cell_type == "prism" ) nnodes = 6;

    return nnodes;
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given filename has the supported extension
 */
/*===========================================================================*/
bool KVSMLUnstructuredVolumeObject::CheckExtension( const std::string& filename )
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
 *  @return true, if the KVSMLUnstructuredVolumeObject class can read the given file
 */
/*===========================================================================*/
bool KVSMLUnstructuredVolumeObject::CheckFormat( const std::string& filename )
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

    // <UnstructuredVolumeObject>
    const std::string volume_tag("UnstructuredVolumeObject");
    const kvs::XMLNode::SuperClass* volume_node = kvs::XMLNode::FindChildNode( object_node, volume_tag );
    if ( !volume_node ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML object unstructured volume object class.
 */
/*===========================================================================*/
KVSMLUnstructuredVolumeObject::KVSMLUnstructuredVolumeObject():
    m_writing_type( kvs::KVSMLUnstructuredVolumeObject::Ascii )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML object unstructured volume object class by reading the given file.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLUnstructuredVolumeObject::KVSMLUnstructuredVolumeObject( const std::string& filename ):
    m_writing_type( kvs::KVSMLUnstructuredVolumeObject::Ascii )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Prints the information of this file.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void KVSMLUnstructuredVolumeObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Cell type : " << this->cellType() << std::endl;
    os << indent << "Veclen : " << this->veclen() << std::endl;
    os << indent << "Number of nodes : " << this->nnodes() << std::endl;
    os << indent << "Number of cells : " << this->ncells() << std::endl;
    if ( this->hasLabel() ) { os << indent << "Value label : " << this->label() << std::endl; }
    if ( this->hasUnit() ) { os << indent << "Value unit : " << this->unit() << std::endl; }
    os << indent << "Value type : " << m_values.typeInfo()->typeName();
    if ( this->hasMinValue() ) { os << indent << "Min value : " << this->minValue() << std::endl; }
    if ( this->hasMaxValue() ) { os << indent << "Max value : " << this->maxValue() << std::endl; }
    if ( this->hasObjectCoord() )
    {
        os << indent << "Min object coord : " << this->minObjectCoord() << std::endl;
        os << indent << "Max object coord : " << this->maxObjectCoord() << std::endl;
    }
    if ( this->hasExternalCoord() )
    {
        os << indent << "Min external coord : " << this->minExternalCoord() << std::endl;
        os << indent << "Max external coord : " << this->maxExternalCoord() << std::endl;
    }
}

/*===========================================================================*/
/**
 *  @brief  Read a KVSML unstructured volume object from the given file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool KVSMLUnstructuredVolumeObject::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document
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

    // <UnstructuredVolumeObject>
    if ( !m_volume_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_volume_tag.name().c_str() );
        return false;
    }

//    if ( !m_volume_tag.hasCellType() )
//    {
//        kvsMessageError( "'cell_type' is not specified in <%s>.", m_volume_tag.name().c_str() );
//        return false;
//    }

    // <Node>
    if ( !m_node_tag.read( m_volume_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_node_tag.name().c_str() );
        return false;
    }

    if ( !m_node_tag.hasNNodes() )
    {
        kvsMessageError( "'nnodes' is not specified in <%s>.", m_node_tag.name().c_str() );
        return false;
    }

    // <Value>
    if ( !m_value_tag.read( m_node_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_value_tag.name().c_str() );
        return false;
    }

    if ( !m_value_tag.hasVeclen() )
    {
        kvsMessageError( "'veclen' is not specified in <%s>.", m_value_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    const size_t value_nelements = m_node_tag.nnodes() * m_value_tag.veclen();
    kvs::kvsml::DataArrayTag values;
    if ( !values.read( m_value_tag.node(), value_nelements, &m_values ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         values.name().c_str(),
                         m_value_tag.name().c_str() );
        return false;
    }

    // <Coord>
    if ( !m_coord_tag.read( m_node_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_coord_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    const size_t dimension = 3;
    const size_t coord_nelements = m_node_tag.nnodes() * dimension;
    kvs::kvsml::DataArrayTag coords;
    if ( !coords.read( m_coord_tag.node(), coord_nelements, &m_coords ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         coords.name().c_str(),
                         m_coord_tag.name().c_str() );
        return false;
    }

    if ( m_volume_tag.cellType() == "point" )
    {
        // In case of point, <Cell> tag is not specified in the format.
        BaseClass::setSuccess( true );
        return true;
    }

    // <Cell>
    if ( !m_cell_tag.read( m_volume_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_cell_tag.name().c_str() );
        return false;
    }

    if ( !m_cell_tag.hasNCells() )
    {
        kvsMessageError( "'ncells' is not specified in <%s>.", m_cell_tag.name().c_str() );
        return false;
    }

    // <Connection>
    if ( !m_connection_tag.read( m_cell_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_connection_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    const size_t nnodes_per_element = ::GetNumberOfNodesPerElement( m_volume_tag.cellType() );
    const size_t connection_nelements = m_cell_tag.ncells() * nnodes_per_element;
    kvs::kvsml::DataArrayTag connections;
    if ( !connections.read( m_connection_tag.node(), connection_nelements, &m_connections ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         connections.name().c_str(),
                         m_connection_tag.name().c_str() );
        return false;
    }

    BaseClass::setSuccess( true );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML unstructured volume object to the given file.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool KVSMLUnstructuredVolumeObject::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document
    std::string comment(" Generated by kvs::KVSMLUnstructuredVolumeObject::write() ");
    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment( comment.c_str() ) );

    // <KVSML>
    m_kvsml_tag.write( &document );

    // <Object type="UnstructuredVolumeObject"
    //         object_coord="xxx xxx xxx xxx xxx xxx"
    //         external_coord="xxx xxx xxx xxx xxx xxx">
    m_object_tag.setType( "UnstructuredVolumeObject" );
    if ( !m_object_tag.write( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <UnstructuredVolumeObject cell_type="xxx">
    if ( !m_volume_tag.write( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_volume_tag.name().c_str() );
        return false;
    }

    // <Node nnodes="xxx">
    if ( !m_node_tag.write( m_volume_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_node_tag.name().c_str() );
        return false;
    }

    // <Value label="xxx" veclen="xxx" min_value="xxx" max_value="xxx">
    if ( !m_value_tag.write( m_node_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_value_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    kvs::kvsml::DataArrayTag values;
    if ( m_writing_type == ExternalAscii )
    {
        values.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "value" ) );
        values.setFormat( "ascii" );
    }
    else if ( m_writing_type == ExternalBinary )
    {
        values.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "value" ) );
        values.setFormat( "binary" );
    }

    const std::string pathname = kvs::File( filename ).pathName();
    if ( !values.write( m_value_tag.node(), m_values, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         values.name().c_str(),
                         m_value_tag.name().c_str() );
        return false;
    }

    // <Coord>
    if ( !m_coord_tag.write( m_node_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_coord_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    kvs::kvsml::DataArrayTag coords;
    if ( m_writing_type == ExternalAscii )
    {
        coords.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "coord" ) );
        coords.setFormat( "ascii" );
    }
    else if ( m_writing_type == ExternalBinary )
    {
        coords.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "coord" ) );
        coords.setFormat( "binary" );
    }

    if ( !coords.write( m_coord_tag.node(), m_coords, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         coords.name().c_str(),
                         m_coord_tag.name().c_str() );
        return false;
    }

    if ( m_volume_tag.cellType() == "point" )
    {
        const bool success = document.write( filename );
        BaseClass::setSuccess( success );
        return success;
    }

    // <Cell ncells="xxx">
    if ( !m_cell_tag.write( m_volume_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_cell_tag.name().c_str() );
        return false;
    }

    // <Connection>
    if ( !m_connection_tag.write( m_cell_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_connection_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    kvs::kvsml::DataArrayTag connections;
    if ( m_writing_type == ExternalAscii )
    {
        connections.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "connect" ) );
        connections.setFormat( "ascii" );
    }
    else if ( m_writing_type == ExternalBinary )
    {
        connections.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "connect" ) );
        connections.setFormat( "binary" );
    }

    if ( !connections.write( m_connection_tag.node(), m_connections, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         connections.name().c_str(),
                         m_connection_tag.name().c_str() );
        return false;
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs
