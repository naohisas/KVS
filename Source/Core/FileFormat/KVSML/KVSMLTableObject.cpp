/*****************************************************************************/
/**
 *  @file   KVSMLTableObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLTableObject.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "KVSMLTableObject.h"
#include "TableObjectTag.h"
#include "ColumnTag.h"
#include "DataArrayTag.h"
#include <kvs/File>
#include <kvs/XMLDocument>
#include <kvs/XMLDeclaration>
#include <kvs/XMLElement>
#include <kvs/XMLComment>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given filename has the supported extension
 */
/*===========================================================================*/
bool KVSMLTableObject::CheckExtension( const std::string& filename )
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
 *  @return true, if the KVSMLTableObject class can read the given file
 */
/*===========================================================================*/
bool KVSMLTableObject::CheckFormat( const std::string& filename )
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

    // <TableObject>
    const std::string table_tag("TableObject");
    const kvs::XMLNode::SuperClass* table_node = kvs::XMLNode::FindChildNode( object_node, table_tag );
    if ( !table_node ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML table object class.
 */
/*===========================================================================*/
KVSMLTableObject::KVSMLTableObject():
    m_writing_type( kvs::KVSMLTableObject::Ascii ),
    m_nrows(0),
    m_ncolumns(0)
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML table object class by reading the given file.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
KVSMLTableObject::KVSMLTableObject( const std::string& filename ):
    m_writing_type( kvs::KVSMLTableObject::Ascii ),
    m_nrows(0),
    m_ncolumns(0)
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Sets a min. value.
 *  @param  value [in] min. value
 */
/*===========================================================================*/
void KVSMLTableObject::setMinValue( const size_t column_index, const double value )
{
    m_has_min_values[column_index] = true;
    m_min_values[column_index] = value;
}

/*===========================================================================*/
/**
 *  @brief  Sets a max. value.
 *  @param  value [in] max. value
 */
/*===========================================================================*/
void KVSMLTableObject::setMaxValue( const size_t column_index, const double value )
{
    m_has_max_values[column_index] = true;
    m_max_values[column_index] = value;
}

/*===========================================================================*/
/**
 *  @brief  Sets a min. range.
 *  @param  value [in] min. range
 */
/*===========================================================================*/
void KVSMLTableObject::setMinRange( const size_t column_index, const double range )
{
    m_has_min_ranges[column_index] = true;
    m_min_ranges[column_index] = range;
}

/*===========================================================================*/
/**
 *  @brief  Sets a max. range.
 *  @param  value [in] max. range
 */
/*===========================================================================*/
void KVSMLTableObject::setMaxRange( const size_t column_index, const double range )
{
    m_has_max_ranges[column_index] = true;
    m_max_ranges[column_index] = range;
}

/*===========================================================================*/
/**
 *  @brief  Adds a column.
 *  @param  column [in] column
 *  @param  label [in] column label
 */
/*===========================================================================*/
void KVSMLTableObject::addColumn( const kvs::AnyValueArray& column, const std::string& label )
{
    m_columns.push_back( column );
    m_labels.push_back( label );

    m_ncolumns++;
    m_nrows = m_nrows > column.size() ? m_nrows : column.size();
}

/*===========================================================================*/
/**
 *  @brief  Prints the information of this file.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void KVSMLTableObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of rows : " << m_nrows << std::endl;
    os << indent << "Number of columns : " << m_ncolumns << std::endl;
    os << indent << "Labels : ";
    for ( size_t i = 0; i < m_ncolumns; i++ )
    {
        os << m_labels.at(i);
        if ( i < m_ncolumns - 1 ) os << ", ";
    }
    os << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a KVSML table object from the given file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool KVSMLTableObject::read( const std::string& filename )
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

    // <TableObject>
    kvs::kvsml::TableObjectTag table_object_tag;
    if ( !table_object_tag.read( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", table_object_tag.name().c_str() );
        return false;
    }

    m_nrows = table_object_tag.nrows();
    m_ncolumns = table_object_tag.ncolumns();

    // <Column>
    kvs::kvsml::ColumnTag column_tag;
    kvs::XMLNode::SuperClass* node = kvs::XMLNode::FindChildNode( table_object_tag.node(), column_tag.name() );
    size_t counter = 0;
    while ( node )
    {
        column_tag.read( kvs::XMLNode::ToElement( node ) );

        if ( counter++ < m_ncolumns )
        {
            m_labels.push_back( column_tag.label() );
            m_has_min_values.push_back( column_tag.hasMinValue() );
            m_has_max_values.push_back( column_tag.hasMaxValue() );
            m_has_min_ranges.push_back( column_tag.hasMinRange() );
            m_has_max_ranges.push_back( column_tag.hasMaxRange() );
            m_min_values.push_back( column_tag.minValue() );
            m_max_values.push_back( column_tag.maxValue() );
            m_min_ranges.push_back( column_tag.minRange() );
            m_max_ranges.push_back( column_tag.maxRange() );

            kvs::AnyValueArray data_array;
            kvs::kvsml::DataArrayTag data_array_tag;
            if ( !data_array_tag.read( node, m_nrows, &data_array ) )
            {
                kvsMessageError( "Cannot read <%s>.", data_array_tag.name().c_str() );
                return false;
            }

            m_columns.push_back( data_array );
        }

        node = table_object_tag.node()->IterateChildren( column_tag.name(), node );
    }

    BaseClass::setSuccess( true );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML table object to the given file.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool KVSMLTableObject::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( false );

    // XML document
    std::string comment(" Generated by kvs::KVSMLTableObject::write() ");
    kvs::XMLDocument document;
    document.InsertEndChild( kvs::XMLDeclaration("1.0") );
    document.InsertEndChild( kvs::XMLComment( comment.c_str() ) );

    // <KVSML>
    m_kvsml_tag.write( &document );

    // <Object type="TableObject">
    m_object_tag.setType( "TableObject" );
    if ( !m_object_tag.write( m_kvsml_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", m_object_tag.name().c_str() );
        return false;
    }

    // <TableObject nrows="xxx", ncolumns="xxx">
    kvs::kvsml::TableObjectTag table_object_tag;
    table_object_tag.setNRows( m_nrows );
    table_object_tag.setNColumns( m_ncolumns );
    if ( !table_object_tag.write( m_object_tag.node() ) )
    {
        kvsMessageError( "Cannot write <%s>.", table_object_tag.name().c_str() );
        return false;
    }

    for ( size_t i = 0; i < m_ncolumns; i++ )
    {
        // <Column label="xxx" min_value="xxx" max_value="xxx" min_range="xxx" max_range="xxx">
        kvs::kvsml::ColumnTag column_tag;
        const std::string label = m_labels.at(i);
        column_tag.setLabel( label );

        if ( m_has_min_values.at(i) ) { column_tag.setMinValue( m_min_values.at(i) ); }
        if ( m_has_max_values.at(i) ) { column_tag.setMaxValue( m_max_values.at(i) ); }
        if ( m_has_min_ranges.at(i) ) { column_tag.setMinRange( m_min_ranges.at(i) ); }
        if ( m_has_max_ranges.at(i) ) { column_tag.setMaxRange( m_max_ranges.at(i) ); }

        if ( !column_tag.write( table_object_tag.node() ) )
        {
            kvsMessageError( "Cannot write <%s>.", column_tag.name().c_str() );
            return false;
        }

        // <DataArray>
        kvs::kvsml::DataArrayTag data_array;
        if ( m_writing_type == ExternalAscii )
        {
            data_array.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, label ) );
            data_array.setFormat( "ascii" );
        }
        else if ( m_writing_type == ExternalBinary )
        {
            data_array.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, label ) );
            data_array.setFormat( "binary" );
        }

        const std::string pathname = kvs::File( filename ).pathName();
        if ( !data_array.write( column_tag.node(), m_columns.at(i), pathname ) )
        {
            kvsMessageError( "Cannot write <%s>.", data_array.name().c_str() );
            return false;
        }
    }

    const bool success = document.write( filename );
    BaseClass::setSuccess( success );

    return success;
}

} // end of namespace kvs
