/*****************************************************************************/
/**
 *  @file   DataArrayTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataArrayTag.cpp 1810 2014-09-03 01:03:41Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "DataArrayTag.h"
#include "DataArray.h"
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/XMLDocument>
#include <kvs/File>
#include <kvs/ValueArray>
#include <kvs/AnyValueArray>
#include <kvs/IgnoreUnusedVariable>
#include <iostream>
#include <fstream>
#include <sstream>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new data array tag class.
 */
/*===========================================================================*/
DataArrayTag::DataArrayTag():
    kvs::kvsml::TagBase( "DataArray" ),
    m_has_type( false ),
    m_has_file( false ),
    m_has_format( false ),
    m_has_endian( false ),
    m_type( "" ),
    m_file( "" ),
    m_format( "" ),
    m_endian( "" )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads a data array tag.
 *  @param  parent [in] pointer to the parent node
 *  @param  nelements [in] number of elements of the data array
 *  @param  data [out] data array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DataArrayTag::read(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nelements,
    kvs::AnyValueArray* data )
{
    BaseClass::read( parent );
    this->read_attribute();
    return this->read_data( nelements, data );
}

/*===========================================================================*/
/**
 *  @brief  Writes the data array.
 *  @param  parent [in] pointer to the paranet node for writing
 *  @param  data [in] data array
 *  @param  pathname [in] pathname
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool DataArrayTag::write(
    kvs::XMLNode::SuperClass* parent,
    const kvs::AnyValueArray& data,
    const std::string pathname )
{
    if ( data.size() == 0 ) return true;

    const std::string tag_name = BaseClass::name();
    kvs::XMLElement element( tag_name );
    element.setAttribute( "type", kvs::kvsml::DataArray::GetDataType( data ) );

    // Internal data: <DataArray type="xxx">xxx</DataArray>
    if ( !m_has_file )
    {
        // Write the data array to string-stream.
        std::ostringstream oss( std::ostringstream::out );
        const std::type_info& data_type = data.typeInfo()->type();
        const size_t data_size = data.size();
        if ( data_type == typeid(kvs::Int8) )
        {
            const kvs::Int8* values = static_cast<const kvs::Int8*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << kvs::Int16(values[i]) << " ";
        }
        else if ( data_type == typeid(kvs::UInt8) )
        {
            const kvs::UInt8* values = static_cast<const kvs::UInt8*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << kvs::UInt16(values[i]) << " ";
        }
        else if ( data_type == typeid(kvs::Int16) )
        {
            const kvs::Int16* values = static_cast<const kvs::Int16*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }
        else if ( data_type == typeid(kvs::UInt16) )
        {
            const kvs::UInt16* values = static_cast<const kvs::UInt16*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }
        else if ( data_type == typeid(kvs::Int32) )
        {
            const kvs::Int32* values = static_cast<const kvs::Int32*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }
        else if ( data_type == typeid(kvs::UInt32) )
        {
            const kvs::UInt32* values = static_cast<const kvs::UInt32*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }
        else if ( data_type == typeid(kvs::Real32) )
        {
            const kvs::Real32* values = static_cast<const kvs::Real32*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }
        else if ( data_type == typeid(kvs::Real64) )
        {
            const kvs::Real64* values = static_cast<const kvs::Real64*>( data.data() );
            for ( size_t i = 0; i < data_size; i++ ) oss << values[i] << " ";
        }

        // Insert the data array as string-stream to the parent node.
        TiXmlText text;
        text.SetValue( oss.str() );

        kvs::XMLNode::SuperClass* node = parent->InsertEndChild( element );
        if( !node )
        {
            kvsMessageError( "Cannot insert <%s>.", tag_name.c_str() );
            return false;
        }

        return node->InsertEndChild( text ) != NULL;
    }

    // External data: <DataArray type="xxx" format="xxx" file="xxx" endian="xxx"/>
    else
    {
        if ( !m_has_format )
        {
            kvsMessageError( "'format' is not spcified in <%s>.", tag_name.c_str() );
            return false;
        }

        element.setAttribute( "format", m_format );
        element.setAttribute( "file", m_file );

        if ( m_format == "binary" )
        {
            if ( kvs::Endian::IsBig() ) { m_endian = "big"; }
            if ( kvs::Endian::IsLittle() ) { m_endian = "little"; }
            element.setAttribute( "endian", m_endian );
        }

        parent->InsertEndChild( element );

        // Write the data to the external data file.
        const std::string filename = pathname + kvs::File::Separator() + m_file;
        return kvs::kvsml::DataArray::WriteExternalData( data, filename, m_format );
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads attributes.
 */
/*===========================================================================*/
void DataArrayTag::read_attribute()
{
    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( m_node );

    // type="xxx"
    const std::string type = kvs::XMLElement::AttributeValue( element, "type" );
    if ( type != "" )
    {
        m_has_type = true;
        m_type = type;
    }

    // file="xxx"
    const std::string file = kvs::XMLElement::AttributeValue( element, "file" );
    if ( file != "" )
    {
        m_has_file = true;
        m_file = file;
    }

    // format="xxx"
    const std::string format = kvs::XMLElement::AttributeValue( element, "format" );
    if ( format != "" )
    {
        m_has_format = true;
        m_format = format;
    }

    // endian="xxx"
    const std::string endian = kvs::XMLElement::AttributeValue( element, "endian" );
    if ( endian != "" )
    {
        m_has_endian = true;
        m_endian = endian;
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads a data array.
 *  @param  nelements [in] number of elements
 *  @param  data [out] pointer to the data array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DataArrayTag::read_data( const size_t nelements, kvs::AnyValueArray* data )
{
    const std::string tag_name = BaseClass::name();

    // Internal data.
    if ( m_file == "" )
    {
        const TiXmlText* array_text = kvs::XMLNode::ToText( m_node );
        if ( !array_text )
        {
            kvsMessageError( "No value in <%s>.", tag_name.c_str() );
            return false;
        }

        // <DataArray type="xxx">xxx</DataArray>
        const std::string delim(" ,\t\n");
        kvs::Tokenizer t( array_text->Value(), delim );

        if( m_type == "char" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::Int8>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "unsigned char" || m_type == "uchar" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::UInt8>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "short" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::Int16>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "unsigned short" || m_type == "ushort" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::UInt16>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "int" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::Int32>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "unsigned int" || m_type == "uint" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::UInt32>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "float" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::Real32>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if ( m_type == "double" )
        {
            if ( !kvs::kvsml::DataArray::ReadInternalData<kvs::Real64>( data, nelements, t ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else
        {
            kvsMessageError( "'type' is not specified or unknown data type in <%s>.", tag_name.c_str() );
            return false;
        }
    }

    // External data.
    else
    {
        // <DataArray file="xxx" type="xxx" format="xxx" endian="xxx"/>
        if( m_format == "" )
        {
            kvsMessageError( "'format' is not specified in <%s>.", tag_name.c_str() );
            return false;
        }

        // Check byte swapping.
        bool byte_swap = false;
        if ( m_format == "binary" )
        {
            if ( m_has_endian )
            {
                if ( kvs::Endian::IsBig() && m_endian == "little" ) { byte_swap = true; }
                if ( kvs::Endian::IsLittle() && m_endian == "big" ) { byte_swap = true; }
            }
        }

        // Filename as an absolute path.
        const kvs::XMLDocument* document
            = reinterpret_cast<kvs::XMLDocument*>( m_node->GetDocument() );
        const std::string path = kvs::File( document->filename() ).pathName( true );
        const std::string filename = path + kvs::File::Separator() + m_file;

        if( m_type == "char" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int8>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::Int8*)( data->data() ), nelements ); }
        }
        else if( m_type == "unsigned char" || m_type == "uchar" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt8>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::UInt8*)( data->data() ), nelements ); }
        }
        else if ( m_type == "short" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int16>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::Int16*)( data->data() ), nelements ); }
        }
        else if ( m_type == "unsigned short" || m_type == "ushort" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt16>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::UInt16*)( data->data() ), nelements ); }
        }
        else if ( m_type == "int" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::Int32*)( data->data() ), nelements ); }
        }
        else if ( m_type == "unsigned int" || m_type == "uint" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::UInt32*)( data->data() ), nelements ); }
        }
        else if ( m_type == "float" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Real32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::Real32*)( data->data() ), nelements ); }
        }
        else if ( m_type == "double" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Real64>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }

            if ( byte_swap ) { kvs::Endian::Swap( (kvs::Real64*)( data->data() ), nelements ); }
        }
        else
        {
            kvsMessageError( "'type' is not specified or unknown data type in <%s>.", tag_name.c_str() );
            return false;
        }
    }

    return true;
}

} // end of namespace kvsml

} // end of namespace kvs
