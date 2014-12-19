/****************************************************************************/
/**
 *  @file String.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: String.h 1349 2012-11-13 07:09:38Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__STRING_H_INCLUDE
#define KVS__STRING_H_INCLUDE

#include <string>
#include <sstream>
#include <kvs/Type>
#if KVS_ENABLE_DEPRECATED
#include <cstdarg>
#include <cstdio>
#include <iostream>
#endif

namespace kvs
{

/*==========================================================================*/
/**
 *  String class.
 */
/*==========================================================================*/
class String
{
public:

    template <typename T>
    static T To( const std::string& str )
    {
        T ret;
        std::istringstream ss( str );
        ss >> ret;
        return ret;
    }

    template <typename T>
    static std::string ToString( const T& src )
    {
        std::ostringstream ss;
        ss << src;
        return ss.str();
    }

    static std::string ToUpper( const std::string& str );

    static std::string ToLower( const std::string& str );

    static std::string Replace( const std::string& source, const std::string& pattern, const std::string& placement );

    //static std::string Format( const char* str, ... );

    static std::string FromFile( const std::string& filename );


#if KVS_ENABLE_DEPRECATED
private:

    char*  m_data; ///< string data
    size_t m_size; ///< string size (not include '\0')

public:

    String( void );

    String( const String& str );

    explicit String( const std::string& str );

    template<typename T>
    explicit String( T number );

    ~String( void );

public:

    char& operator []( size_t index );

    const char operator []( size_t index ) const;

    String& operator =( const std::string& str );

    String& operator =( const String& str );

    String& operator +=( const std::string& str );

    String& operator +=( const String& str );

    const bool operator ==( const String& str );

    const bool operator !=( const String& str );

    const bool operator <( const String& str );

    const bool operator >( const String& str );

    const bool operator <=( const String& str );

    const bool operator >=( const String& str );

    friend const String operator +( const String& str1, const String& str2 );

    friend std::ostream& operator <<( std::ostream& os, const String& str );

public:

    char* data( void );

    const char* data( void ) const;

    const size_t size( void ) const;

    char& at( size_t index );

    const char at( size_t index ) const;

    void upper( void );

    void upper( size_t index );

    void lower( void );

    void lower( size_t index );

    void format( const char* str, ... );

    void replace( const std::string& pattern, const std::string& placement );

    template<typename T>
    void setNumber( T number );

    template<typename T>
    const T toNumber( int base = 10 ) const;

    template<typename T>
    static const T toNumber( const std::string& str, int base = 10 );

    const std::string toStdString( void ) const;
#else
private:
    String();
#endif
};

template <>
inline std::string String::ToString( const kvs::Int8& src )
{
    std::ostringstream ss;
    ss << (int)src;
    return ss.str();
}

template <>
inline std::string String::ToString( const kvs::UInt8& src )
{
    std::ostringstream ss;
    ss << (int)src;
    return ss.str();
}

template <>
inline std::string String::ToString( const std::string& src )
{
    return src;
}

template <>
inline kvs::Int8 String::To( const std::string& str )
{
    int ret;
    std::istringstream ss( str );
    ss >> ret;
    return static_cast<kvs::Int8>( ret );
}

template <>
inline kvs::UInt8 String::To( const std::string& str )
{
    int ret;
    std::istringstream ss( str );
    ss >> ret;
    return static_cast<kvs::UInt8>( ret );
}

template <>
inline std::string String::To( const std::string& src )
{
    return src;
}

#if KVS_ENABLE_DEPRECATED
template<typename T>
String::String( T number )
{
    this->setNumber<T>( number );
}

template<typename T>
void String::setNumber( T number )
{
    std::stringstream s;
    s << number;

    *this = s.str();
}

template<typename T>
const T String::toNumber( const std::string& str, int base )
{
    return( String( str ).toNumber<T>( base ) );
}
#endif
} // end of namespace kvs

#endif // KVS__STRING_H_INCLUDE
