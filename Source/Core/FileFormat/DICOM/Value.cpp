/****************************************************************************/
/**
 *  @file Value.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Value.cpp 1595 2013-06-12 03:27:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Value.h"
#include <kvs/IgnoreUnusedVariable>
#include <iostream>
#include <fstream>
#include <string>
#include "Item.h"
#include "DataType.h"
#include "StreamReader.h"


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  Construct a new class.
 */
/*===========================================================================*/
Value::Value()
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new class.
 *  @param  type   [in] data type
 *  @param  length [in] data length
 */
/*===========================================================================*/
Value::Value( const dcm::DataType type, const unsigned int length )
{
    // Check the data type.
    dcm::DataType data_type = type;
    if( type != dcm::DATA_STRING || type != dcm::DATA_OTHER )
    {
        if( dcm::DATA_TYPE_TO_SIZE[type] != length )
        {
            data_type = dcm::DATA_STRING;
        }
    }

    m_type   = data_type;
    m_length = length;
}

/*===========================================================================*/
/**
 *  @brief  Destruct this class.
 */
/*===========================================================================*/
Value::~Value()
{
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for char type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const char value )
{
    m_value_char = value;
    m_type       = dcm::DATA_CHAR;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for unsigned char type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const unsigned char value )
{
    m_value_uchar = value;
    m_type        = dcm::DATA_UCHAR;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for shor type.
 *  @param  value [i] value
 */
/*===========================================================================*/
Value& Value::operator = ( const short value )
{
    m_value_short = value;
    m_type        = dcm::DATA_SHORT;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for unsigned short type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const unsigned short value )
{
    m_value_ushort = value;
    m_type         = dcm::DATA_USHORT;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for int type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const int value )
{
    m_value_int = value;
    m_type      = dcm::DATA_INT;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for unsigned int type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const unsigned int value )
{
    m_value_uint = value;
    m_type       = dcm::DATA_UINT;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for float type.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const float value )
{
    m_value_float = value;
    m_type        = dcm::DATA_FLOAT;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for double type.
 *  @param  value [i] value
 */
/*===========================================================================*/
Value& Value::operator = ( const double value )
{
    m_value_double = value;
    m_type         = dcm::DATA_DOUBLE;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Copy operator for string.
 *  @param  value [in] value
 */
/*===========================================================================*/
Value& Value::operator = ( const std::string value )
{
    m_value_string = value;
    m_type         = dcm::DATA_STRING;
    return *this;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @param  a [in] value
 *  @param  b [in] value
 *  @return true, if a equals to b
 */
/*===========================================================================*/
bool operator == ( const Value& a, const Value& b )
{
    return a.m_type == b.m_type;
}

bool operator != ( const Value& a, const Value& b )
{
    return a.m_type != b.m_type;
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  m_type      
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const Value& v )
{
    switch( v.m_type )
    {
    case dcm::DATA_CHAR:  { char           value = v; os << value; break; }
    case dcm::DATA_UCHAR: { unsigned char  value = v; os << value; break; }
    case dcm::DATA_INT:   { int            value = v; os << value; break; }
    case dcm::DATA_UINT:  { unsigned int   value = v; os << value; break; }
    case dcm::DATA_SHORT: { short          value = v; os << value; break; }
    case dcm::DATA_USHORT:{ unsigned short value = v; os << value; break; }
    case dcm::DATA_FLOAT: { float          value = v; os << value; break; }
    case dcm::DATA_DOUBLE:{ double         value = v; os << value; break; }
    case dcm::DATA_STRING:
    case dcm::DATA_OTHER: { std::string    value = v; os << value; break; }
    default: break;
    }
    return os;
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  m_type      
 */
/*===========================================================================*/
std::ofstream& operator << ( std::ofstream& ofs, const Value& v )
{
    switch( v.m_type )
    {
    case dcm::DATA_CHAR:  { char           value = v; ofs << value; break; }
    case dcm::DATA_UCHAR: { unsigned char  value = v; ofs << value; break; }
    case dcm::DATA_INT:   { int            value = v; ofs << value; break; }
    case dcm::DATA_UINT:  { unsigned int   value = v; ofs << value; break; }
    case dcm::DATA_SHORT: { short          value = v; ofs << value; break; }
    case dcm::DATA_USHORT:{ unsigned short value = v; ofs << value; break; }
    case dcm::DATA_FLOAT: { float          value = v; ofs << value; break; }
    case dcm::DATA_DOUBLE:{ double         value = v; ofs << value; break; }
    case dcm::DATA_STRING:
    case dcm::DATA_OTHER: { std::string    value = v; ofs << value; break; }
    default: break;
    }
    return ofs;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator char () const
{
    return m_value_char;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator unsigned char () const
{
    return ( m_value_uchar );
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator short () const
{
    return m_value_short;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator unsigned short () const
{
    return m_value_ushort;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator int () const
{
    return m_value_int;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator unsigned int () const
{
    return m_value_uint;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator float () const
{
    return m_value_float;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator double () const
{
    return m_value_double;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return 
 */
/*===========================================================================*/
Value::operator std::string () const
{
    return m_value_string;
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  type        
 */
/*===========================================================================*/
void Value::setDataType( const dcm::DataType type )
{
    m_type = type;
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  length      
 */
/*===========================================================================*/
void Value::setLength( const unsigned int length )
{
    m_length = length;
}

/*===========================================================================*/
/**
 *  @brief  Read the DICOM data value.
 *  @param  ifs  [in] input file stream
 *  @param  swap [in] swap flag
 *  @retunr true, if the reading process is done succesfully
 */
/*===========================================================================*/
bool dcm::Value::read( std::ifstream& ifs, const bool swap )
{
    // In the case of undefined length.
    if( m_length == 0xffffffff )
    {
        dcm::Item item( m_length );
        bool state = item.read( ifs, swap );
        if( !state )
        {
            kvsMessageError("Cannot read the item.");
            return false;
        }

        m_value_string = item.values();
        return true;
    }

    switch( m_type )
    {
    case dcm::DATA_CHAR:   *this = dcm::StreamReader::Get<char>( ifs, swap );            break;
    case dcm::DATA_UCHAR:  *this = dcm::StreamReader::Get<unsigned char>( ifs, swap );   break;
    case dcm::DATA_INT:    *this = dcm::StreamReader::Get<int>( ifs, swap );             break;
    case dcm::DATA_UINT:   *this = dcm::StreamReader::Get<unsigned int>( ifs, swap );    break;
    case dcm::DATA_SHORT:  *this = dcm::StreamReader::Get<short>( ifs, swap );           break;
    case dcm::DATA_USHORT: *this = dcm::StreamReader::Get<unsigned short>( ifs, swap );  break;
    case dcm::DATA_FLOAT:  *this = dcm::StreamReader::Get<float>( ifs, swap );           break;
    case dcm::DATA_DOUBLE: *this = dcm::StreamReader::Get<double>( ifs, swap );          break;
    case dcm::DATA_STRING: *this = dcm::StreamReader::Get( ifs, m_length, swap );        break;
    case dcm::DATA_OTHER:  *this = dcm::StreamReader::Get( ifs, m_length, swap );        break;
    default: kvsMessageError("Unknown data type"); return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read the item data.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] swap flag
 *  @retunr true, if the reading process is done succesfully
 */
/*===========================================================================*/
bool dcm::Value::read_data( std::ifstream& ifs, const bool swap )
{
    // Read item tag.
    unsigned short data_tag[2];
    data_tag[0] = dcm::StreamReader::Get<unsigned short>( ifs, swap );
    data_tag[1] = dcm::StreamReader::Get<unsigned short>( ifs, swap );
    kvs::IgnoreUnusedVariable( data_tag[0] );
    kvs::IgnoreUnusedVariable( data_tag[1] );

    // Read VR (2 bytes).
    char vr[2+1];
    ifs.read( vr, 2 );
    if( ifs.bad() )
    {
        kvsMessageError("Cannot read the VR code.");
        return false;
    }
    vr[2] = '\0';

    // Read value length.
    unsigned int data_length = 0;
    if( std::string( vr ) == "SQ" )
    {
        ifs.clear();
        ifs.seekg( 2, std::ios_base::cur ); // reserved region (2 bytes)
        data_length = dcm::StreamReader::Get<unsigned int>( ifs, swap );
    }
    else
    {
        ifs.clear();
        ifs.seekg( -2, std::ios_base::cur ); // go back (2bytes)
        data_length = dcm::StreamReader::Get<unsigned int>( ifs, swap );
    }

    dcm::Item item( data_length );
    bool state = item.read( ifs, swap );
    if( !state )
    {
        kvsMessageError("Cannot read the item.");
        return false;
    }

    m_value_string = item.values();

    return true;
}

} // end of namespace dcm

} // end of namespace kvs
