/****************************************************************************/
/**
 *  @file   KVSMLObjectUnstructuredVolume.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLObjectUnstructuredVolume.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#if 1

#else

#include "KVSMLObjectUnstructuredVolume.h"
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
inline const size_t GetNumberOfNodesPerElement( const std::string& cell_type )
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
bool KVSMLObjectUnstructuredVolume::CheckExtension( const std::string& filename )
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
 *  @return true, if the KVSMLObjectUnstructuredVolume class can read the given file
 */
/*===========================================================================*/
bool KVSMLObjectUnstructuredVolume::CheckFormat( const std::string& filename )
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
KVSMLObjectUnstructuredVolume::KVSMLObjectUnstructuredVolume():
    m_writing_type( kvs::KVSMLObjectUnstructuredVolume::Ascii ),
    m_cell_type( "" ),
    m_has_label( false ),
    m_has_unit( false ),
    m_has_min_value( false ),
    m_has_max_value( false ),
    m_label( "" ),
    m_unit( "" ),
    m_veclen( 0 ),
    m_nnodes( 0 ),
    m_ncells( 0 ),
    m_min_value( 0.0 ),
    m_max_value( 0.0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML object unstructured volume object class by reading the given file.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLObjectUnstructuredVolume::KVSMLObjectUnstructuredVolume( const std::string& filename ):
    m_writing_type( kvs::KVSMLObjectUnstructuredVolume::Ascii ),
    m_cell_type( "" ),
    m_has_label( false ),
    m_has_unit( false ),
    m_has_min_value( false ),
    m_has_max_value( false ),
    m_label( "" ),
    m_unit( "" ),
    m_veclen( 0 ),
    m_nnodes( 0 ),
    m_ncells( 0 ),
    m_min_value( 0.0 ),
    m_max_value( 0.0 )
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
void KVSMLObjectUnstructuredVolume::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Cell type : " << m_cell_type << std::endl;
    os << indent << "Veclen : " << m_veclen << std::endl;
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of cells : " << m_ncells << std::endl;
    if ( m_has_label ) { os << indent << "Value label : " << m_label << std::endl; }
    if ( m_has_unit ) { os << indent << "Value unit : " << m_unit << std::endl; }
    os << indent << "Value type : " << m_values.typeInfo()->typeName();
}

/*===========================================================================*/
/**
 *  @brief  Read a KVSML unstructured volume object from the given file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool KVSMLObjectUnstructuredVolume::read( const std::string& filename )
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
    kvs::kvsml::UnstructuredVolumeObjectTag volume_tag;
    if ( !volume_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", volume_tag.name().c_str() );
        return false;
    }

    if ( !volume_tag.hasCellType() )
    {
        kvsMessageError( "'cell_type' is not specified in <%s>.", volume_tag.name().c_str() );
        return false;
    }
    m_cell_type = volume_tag.cellType();

    // <Node>
    kvs::kvsml::NodeTag node_tag;
    if ( !node_tag.read( volume_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", node_tag.name().c_str() );
        return false;
    }

    if ( !node_tag.hasNNodes() )
    {
        kvsMessageError( "'nnodes' is not specified in <%s>.", node_tag.name().c_str() );
        return false;
    }
    m_nnodes = node_tag.nnodes();

    // <Value>
    kvs::kvsml::ValueTag value_tag;
    if ( !value_tag.read( node_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", value_tag.name().c_str() );
        return false;
    }

    m_has_label = value_tag.hasLabel();
    if ( m_has_label ) { m_label = value_tag.label(); }

    m_has_unit = value_tag.hasUnit();
    if ( m_has_unit ) { m_unit = value_tag.unit(); }

    if ( !value_tag.hasVeclen() )
    {
        kvsMessageError( "'veclen' is not specified in <%s>.", value_tag.name().c_str() );
        return false;
    }
    m_veclen = value_tag.veclen();

    m_has_min_value = value_tag.hasMinValue();
    if ( m_has_min_value ) { m_min_value = value_tag.minValue(); }

    m_has_max_value = value_tag.hasMaxValue();
    if ( m_has_max_value ) { m_max_value = value_tag.maxValue(); }

    // <DataArray>
    const size_t value_nelements = m_nnodes * m_veclen;
    kvs::kvsml::DataArrayTag values;
    if ( !values.read( value_tag.node(), value_nelements, &m_values ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         values.name().c_str(),
                         value_tag.name().c_str() );
        return false;
    }

    // <Coord>
    kvs::kvsml::CoordTag coord_tag;
    if ( !coord_tag.read( node_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", coord_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    const size_t dimension = 3;
    const size_t coord_nelements = m_nnodes * dimension;
    kvs::kvsml::DataArrayTag coords;
    if ( !coords.read( coord_tag.node(), coord_nelements, &m_coords ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         coords.name().c_str(),
                         coord_tag.name().c_str() );
        return false;
    }

    if ( m_cell_type == "point" )
    {
        // In case of point, <Cell> tag is not specified in the format.
        BaseClass::setSuccess( true );
        return true;
    }

    // <Cell>
    kvs::kvsml::CellTag cell_tag;
    if ( !cell_tag.read( volume_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", cell_tag.name().c_str() );
        return false;
    }

    if ( !cell_tag.hasNCells() )
    {
        kvsMessageError( "'ncells' is not specified in <%s>.", cell_tag.name().c_str() );
        return false;
    }
    m_ncells = cell_tag.ncells();

    // <Connection>
    kvs::kvsml::ConnectionTag connection_tag;
    if ( !connection_tag.read( cell_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", connection_tag.name().c_str() );
        return false;
    }

    // <DataArray>
    const size_t nnodes_per_element = ::GetNumberOfNodesPerElement( m_cell_type );
    const size_t connection_nelements = m_ncells * nnodes_per_element;
    kvs::kvsml::DataArrayTag connections;
    if ( !connections.read( connection_tag.node(), connection_nelements, &m_connections ) )
    {
        kvsMessageError( "Cannot read <%s> for <%s>.",
                         connections.name().c_str(),
                         connection_tag.name().c_str() );
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
bool KVSMLObjectUnstructuredVolume::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment(" Generated by kvs::KVSMLObjectStructuredVolume::write() ") );

    // <KVSML>
    kvs::kvsml::KVSMLTag kvsml_tag;
    kvsml_tag.write( &document );

    // <Object type="UnstructuredVolumeObject">
    kvs::kvsml::ObjectTag object_tag;
    object_tag.setType( "UnstructuredVolumeObject" );
    if ( !object_tag.write( kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", object_tag.name().c_str() );
        return false;
    }

    // <UnstructuredVolumeObject cell_type="xxx">
    kvs::kvsml::UnstructuredVolumeObjectTag volume_tag;
    volume_tag.setCellType( m_cell_type );
    if ( !volume_tag.write( object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", volume_tag.name().c_str() );
        return false;
    }

    // <Node nnodes="xxx">
    kvs::kvsml::NodeTag node_tag;
    node_tag.setNNodes( m_nnodes );
    if ( !node_tag.write( volume_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", node_tag.name().c_str() );
        return false;
    }

    // <Value label="xxx" veclen="xxx" min_value="xxx" max_value="xxx">
    kvs::kvsml::ValueTag value_tag;
    value_tag.setVeclen( m_veclen );

    if ( m_has_label ) { value_tag.setLabel( m_label ); }
    if ( m_has_unit ) { value_tag.setUnit( m_unit ); }
    if ( m_has_min_value ) { value_tag.setMinValue( m_min_value ); }
    if ( m_has_max_value ) { value_tag.setMaxValue( m_max_value ); }

    if ( !value_tag.write( node_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", value_tag.name().c_str() );
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
    if ( !values.write( value_tag.node(), m_values, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         values.name().c_str(),
                         value_tag.name().c_str() );
        return false;
    }

    // <Coord>
    kvs::kvsml::CoordTag coord_tag;
    if ( !coord_tag.write( node_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", coord_tag.name().c_str() );
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

    if ( !coords.write( coord_tag.node(), m_coords, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         coords.name().c_str(),
                         coord_tag.name().c_str() );
        return false;
    }

    if ( m_cell_type == "point" )
    {
        const bool success = document.write( filename );
        BaseClass::setSuccess( success );

        return success;
    }

    // <Cell ncells="xxx">
    kvs::kvsml::CellTag cell_tag;
    cell_tag.setNCells( m_ncells );
    if ( !cell_tag.write( volume_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", cell_tag.name().c_str() );
        return false;
    }

    // <Connection>
    kvs::kvsml::ConnectionTag connection_tag;
    if ( !connection_tag.write( cell_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", connection_tag.name().c_str() );
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

    if ( !connections.write( connection_tag.node(), m_connections, pathname ) )
    {
        kvsMessageError( "Cannot write <%s> for <%s>.",
                         connections.name().c_str(),
                         connection_tag.name().c_str() );
        return false;
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs

#endif
