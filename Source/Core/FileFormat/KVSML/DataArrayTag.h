/*****************************************************************************/
/**
 *  @file   DataArrayTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataArrayTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/ValueArray>
#include <kvs/AnyValueArray>
#include <kvs/File>
#include <kvs/Endian>
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/XMLDocument>
#include "DataArray.h"
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <DataArray>
 */
/*===========================================================================*/
class DataArrayTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_type; ///< flag to check whether 'type' is specified or not
    bool m_has_file; ///< flag to check whether 'file' is specified or not
    bool m_has_format; ///< flag to check whether 'format' is specified or not
    bool m_has_endian; ///< flag to check whether 'endian' is specified or not
    std::string m_type; ///< data type string
    std::string m_file; ///< external file name
    std::string m_format; ///< external file format
    std::string m_endian; ///< endianness of the binary data

public:
    DataArrayTag();

    bool hasType() const { return m_has_type; }
    bool hasFile() const { return m_has_file; }
    bool hasFormat() const { return m_has_format; }
    bool hasEndian() const { return m_has_endian; }
    const std::string& type() const { return m_type; }
    const std::string& file() const { return m_file; }
    const std::string& format() const { return m_format; }
    const std::string& endian() const { return m_endian; }

    void setFile( const std::string& file ) { m_has_file = true; m_file = file; }
    void setFormat( const std::string& format ) { m_has_format = true; m_format = format; }
    void setEndian( const std::string& endian ) { m_has_endian = true; m_endian = endian; }

    bool read( const kvs::XMLNode::SuperClass* parent, const size_t nelements, kvs::AnyValueArray* data );
    template <typename T>
    bool read( const kvs::XMLNode::SuperClass* parent, const size_t nelements, kvs::ValueArray<T>* data );
    bool write( kvs::XMLNode::SuperClass* parent, const kvs::AnyValueArray& data, const std::string pathname );
    template <typename T>
    bool write( kvs::XMLNode::SuperClass* parent, const kvs::ValueArray<T>& data, const std::string pathname );

private:
    void read_attribute();
    bool read_data( const size_t nelements, kvs::AnyValueArray* data );
    template <typename T>
    bool read_data( const size_t nelements, kvs::ValueArray<T>* data );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

/*===========================================================================*/
/**
 *  @brief  Reads a data array tag.
 *  @param  parent [in] pointer to the parent node
 *  @param  nelements [in] number of elements of the data array
 *  @param  data [out] data array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
template <typename T>
inline bool DataArrayTag::read(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nelements,
    kvs::ValueArray<T>* data )
{
    BaseClass::read( parent );
    this->read_attribute();
    return this->read_data<T>( nelements, data );
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
template <typename T>
inline bool DataArrayTag::write(
    kvs::XMLNode::SuperClass* parent,
    const kvs::ValueArray<T>& data,
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
        const size_t data_size = data.size();
        if ( typeid(T) == typeid(kvs::Int8) || typeid(T) == typeid(kvs::UInt8) )
        {
            for ( size_t i = 0; i < data_size; i++ ) oss << int(data[i]) << " ";
        }
        else
        {
            for ( size_t i = 0; i < data_size; i++ ) oss << data[i] << " ";
        }

        // Insert the data array as string-stream to the parent node.
        TiXmlText text;
        text.SetValue( oss.str() );

        kvs::XMLNode::SuperClass* node = parent->InsertEndChild( element );
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

        // Set text.
        const std::string filename = pathname + kvs::File::Separator() + m_file;
        return kvs::kvsml::DataArray::WriteExternalData( data, filename, m_format );
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
template <typename T>
bool DataArrayTag::read_data( const size_t nelements, kvs::ValueArray<T>* data )
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

        // <DataArray>xxx</DataArray>
        const std::string delim(" \n");
        kvs::Tokenizer tokenizer( array_text->Value(), delim );

        if ( !kvs::kvsml::DataArray::ReadInternalData<T>( data, nelements, tokenizer ) )
        {
            kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
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
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::Int8>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "unsigned char" || m_type == "uchar" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::UInt8>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "short" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::Int16>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "unsigned short" || m_type == "ushort" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::UInt16>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "int" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::Int32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "unsigned int" || m_type == "uint" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::UInt32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "float" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::Real32>( data, nelements, filename, m_format ) )
            {
                kvsMessageError( "Cannot read the data array in <%s>.", tag_name.c_str() );
                return false;
            }
        }
        else if( m_type == "double" )
        {
            if ( !kvs::kvsml::DataArray::ReadExternalData<T,kvs::Real64>( data, nelements, filename, m_format ) )
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

        if ( byte_swap ) { kvs::Endian::Swap( data->data(), nelements ); }
    }

    return true;
}

} // end of namespace kvsml

} // end of namespace kvs
