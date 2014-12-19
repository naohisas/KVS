/****************************************************************************/
/**
 *  @file Value.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Value.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DCM__VALUE_H_INCLUDE
#define KVS__DCM__VALUE_H_INCLUDE

#include <string>
#include <iostream>
#include <fstream>
#include "DataType.h"


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  DICOM data value class.
 */
/*===========================================================================*/
class Value
{
private:

    union
    {
        char m_value_char; ///< char type value
        unsigned char m_value_uchar; ///< unsigned char type value
        short m_value_short; ///< short type value
        unsigned short m_value_ushort; ///< unsigned short type value
        int m_value_int; ///< int type value
        unsigned int m_value_uint; ///< unsigned int type value
        float m_value_float; ///< float type value
        double m_value_double; ///< double type value
    };
    std::string m_value_string; ///< string type value
    dcm::DataType m_type; ///< data type
    unsigned int m_length; ///< data length

public:

    Value();
    Value( const dcm::DataType type, const unsigned int length );
    virtual ~Value();

public:

    Value& operator = ( const char value );
    Value& operator = ( const unsigned char value );
    Value& operator = ( const short value );
    Value& operator = ( const unsigned short value );
    Value& operator = ( const int value );
    Value& operator = ( const unsigned int value );
    Value& operator = ( const float value );
    Value& operator = ( const double value );
    Value& operator = ( const std::string value );
    friend bool operator == ( const Value& a, const Value& b );
    friend bool operator != ( const Value& a, const Value& b );
    friend std::ostream& operator << ( std::ostream& os, const Value& v );
    friend std::ofstream& operator << ( std::ofstream& ofs, const Value& v );

public:

    operator char () const;
    operator unsigned char () const;
    operator short () const;
    operator unsigned short () const;
    operator int () const;
    operator unsigned int () const;
    operator float () const;
    operator double () const;
    operator std::string () const;

public:

    void setDataType( const dcm::DataType type );
    void setLength( const unsigned int length );

public:

    bool read( std::ifstream& ifs, const bool swap = false );

private:

    bool read_data( std::ifstream& ifs, const bool swap = false );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__VALUE_H_INCLUDE
