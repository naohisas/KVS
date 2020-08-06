/****************************************************************************/
/**
 *  @file   String.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <kvs/Type>
#include <kvs/Deprecated>
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
        T ret; std::istringstream ss( str ); ss >> ret;
        return ret;
    }

    template <typename T>
    static std::string From( const T& value )
    {
        std::ostringstream ss; ss << value;
        return ss.str();
    }

    template <typename... Args>
    static std::string Format( const char* format, Args... args )
    {
        const auto size = std::snprintf( nullptr, 0, format, args... );
        KVS_ASSERT( size >= 0 );

        char* buffer = new char [ size + 1 ];
        std::snprintf( buffer, size + 1, format, args... );

        std::string str( buffer );
        delete [] buffer;
        return str;
    }

    static std::string From( const kvs::Int8 value, const int width, const char fill = 0 );
    static std::string From( const kvs::Int16 value, const int width, const char fill = 0 );
    static std::string From( const kvs::Int32 value, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt8 value, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt16 value, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt32 value, const int width, const char fill = 0 );
    static std::string From( const kvs::Real32 value, const int precision, const bool fixed = false, const bool scientific = false );
    static std::string From( const kvs::Real64 value, const int precision, const bool fixed = false, const bool scientific = false );
    static std::string FromFile( const std::string& filename );
    static std::string ToUpper( const std::string& str );
    static std::string ToLower( const std::string& str );
    static std::string Replace( const std::string& source, const std::string& pattern, const std::string& placement );

    template <typename T>
    KVS_DEPRECATED( static std::string ToString( const T& value ) ) { return From<T>( value ); }

    template <typename T>
    KVS_DEPRECATED( static std::string ToString(
        const T& value,
        const int precision,
        const bool fixed = false,
        const bool scientific = false ) )
    {
        return From( value, precision, fixed, scientific );
    }

    template <typename T>
    KVS_DEPRECATED( static std::string ToString( const T& value, const std::string& format ) )
    {
        char buffer[100];
        if ( std::sprintf( buffer, format.c_str(), value ) >= 0 )
        {
            return std::string( buffer );
        }
        return std::string("");
    }

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
inline std::string String::To( const std::string& str )
{
    return str;
}

template <>
inline std::string String::From( const kvs::Int8& value )
{
    std::ostringstream ss;
    ss << static_cast<int>(value);
    return ss.str();
}

template <>
inline std::string String::From( const kvs::UInt8& value )
{
    std::ostringstream ss;
    ss << static_cast<int>(value);
    return ss.str();
}

template <>
inline std::string String::From( const std::string& value )
{
    return value;
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
