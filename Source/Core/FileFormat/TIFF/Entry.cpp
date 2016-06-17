/****************************************************************************/
/**
 *  @file   Entry.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Entry.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Entry.h"
#include "ValueType.h"
#include "TagDictionary.h"
#include <cstring>


namespace kvs
{

namespace tiff
{

Entry::Entry( const kvs::UInt16 tag ):
    m_tag( tag ),
    m_type( 0 ),
    m_count( 0 )
{
}

Entry::Entry( std::ifstream& ifs )
{
    this->read( ifs );
}

bool operator == ( const Entry& lhs, const Entry& rhs )
{
    return lhs.tag() == rhs.tag();
}

std::ostream& operator << ( std::ostream& os, const Entry& entry )
{
    os << "Tag:   " << entry.tagDescription() << std::endl;
    os << "Type:  " << entry.typeName() << std::endl;
    os << "Count: " << entry.count() << std::endl;
    if( entry.type() == kvs::tiff::Ascii )
    {
        os << "Value: ";
        kvs::ValueArray<kvs::Int8> values = entry.values().asValueArray<kvs::Int8>();
        for ( size_t i = 0; i < entry.count(); i++ )
        {
            os << values[i];
        }
    }
    else
    {
        os << "Value: ";
        kvs::ValueArray<kvs::UInt32> values = entry.values().asValueArray<kvs::UInt32>();
        for ( size_t i = 0; i < entry.count(); i++ )
        {
            os << values[i];
        }
    }

    return os;
}

std::string Entry::tagDescription() const
{
    static const kvs::tiff::TagDictionary TagDatabase;
    return TagDatabase.find( m_tag ).name();
}

std::string Entry::typeName() const
{
    return kvs::tiff::ValueTypeName[ m_type ];
}

void Entry::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Tag : " << this->tagDescription() << std::endl;
    os << indent << "Type : " << this->typeName() << std::endl;
    os << indent << "Count : " << this->count() << std::endl;
    if( this->type() == kvs::tiff::Ascii )
    {
        os << indent << "Value : ";
        kvs::ValueArray<kvs::Int8> values = this->values().asValueArray<kvs::Int8>();
        for ( size_t i = 0; i < this->count(); i++ )
        {
            os << values[i];
        }
    }
    else
    {
        os << indent << "Value : ";
        kvs::ValueArray<kvs::UInt32> values = this->values().asValueArray<kvs::UInt32>();
        for ( size_t i = 0; i < this->count(); i++ )
        {
            os << values[i];
        }
    }
    os << std::endl;
}

bool Entry::read( std::ifstream& ifs )
{
    // Read a entry.
    unsigned char buffer[12];
    ifs.read( reinterpret_cast<char*>(buffer), 12 );
    if ( ifs.gcount() != 12 ) return false;

    // Separate tag, type and count from the buffer.
    if ( !memcpy( &m_tag,    buffer + 0, 2 ) ) return false; // offset 0, byte 2
    if ( !memcpy( &m_type,   buffer + 2, 2 ) ) return false; // offset 2, byte 2
    if ( !memcpy( &m_count,  buffer + 4, 4 ) ) return false; // offset 4, byte 4

    // Allocate memory for the value array.
    this->allocate_values( m_count, m_type );

    // Read values.
    const size_t byte_size = kvs::tiff::ValueTypeSize[m_type] * m_count;
    if ( byte_size > 4 )
    {
        const std::ifstream::pos_type end_of_entry = ifs.tellg();
        {
            // Separate a value as offset.
            kvs::UInt32 offset;
            if ( !memcpy( &offset, buffer + 8, 4 ) ) return false; // offset 8, byte 4

            // Read values of the entry to m_values.
            ifs.seekg( offset, std::ios::beg );
            ifs.read( reinterpret_cast<char*>( m_values.data() ), byte_size );
            if ( size_t( ifs.gcount() ) != byte_size ) return false;
        }
        ifs.seekg( end_of_entry, std::ios::beg );
    }
    else
    {
        // Read values of the entry from the buffer to m_values.
        if ( !memcpy( m_values.data(), buffer + 8, 4 ) ) return false; // offset 8, byte 4
    }

    return true;
}

void* Entry::allocate_values( const size_t nvalues, const size_t value_type )
{
    switch ( value_type )
    {
    case kvs::tiff::Byte: m_values.allocate<kvs::UInt8>( nvalues ); break;
    case kvs::tiff::Ascii: m_values.allocate<char>( nvalues ); break;
    case kvs::tiff::Short: m_values.allocate<kvs::UInt16>( nvalues ); break;
    case kvs::tiff::Long: m_values.allocate<kvs::UInt32>( nvalues ); break;
    case kvs::tiff::Rational: m_values.allocate<kvs::Real64>( nvalues ); break;
    case kvs::tiff::SByte: m_values.allocate<kvs::Int8>( nvalues ); break;
    case kvs::tiff::Undefined: m_values.allocate<char>( nvalues ); break;
    case kvs::tiff::SShort: m_values.allocate<kvs::Int16>( nvalues ); break;
    case kvs::tiff::SLong: m_values.allocate<kvs::Int32>( nvalues ); break;
    case kvs::tiff::SRational: m_values.allocate<kvs::Real64>( nvalues ); break;
    case kvs::tiff::Float: m_values.allocate<kvs::Real32>( nvalues ); break;
    case kvs::tiff::Double: m_values.allocate<kvs::Real64>( nvalues ); break;
    default:
    {
        kvsMessageError("Unknown entry value type (%d).", int( value_type ) );
        break;
    }
    }
    return m_values.data();
}

} // end of namespace tiff

} // end of namespace kvs
