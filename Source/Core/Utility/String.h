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
#include <memory>
#include <locale>
#include <cstdio>
#include <iostream>
#include <kvs/Type>
#include <kvs/Deprecated>


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
    static std::string From( const T& val )
    {
        std::ostringstream ss; ss << val;
        return ss.str();
    }

    template <typename T>
    static std::string From( const T& val, const char thousands_sep )
    {
        struct Seprator : std::numpunct<char>
        {
            char m_sep;
            Seprator( const char sep ): m_sep( sep ) {}
            char do_thousands_sep() const { return m_sep; }
            std::string do_grouping() const { return "\3"; }
        };
        std::unique_ptr<Seprator> seprator( new Seprator( thousands_sep ) );
        std::ostringstream ss;
        ss.imbue( std::locale( std::cout.getloc(), seprator.release() ) );
        ss << std::fixed << val;
        return ss.str();
    }

    template <typename... Args>
    static std::string Format( const std::string& format, Args... args )
    {
        const auto size = std::snprintf( nullptr, 0, format.c_str(), args... ) + 1;
        std::unique_ptr<char[]> buffer( new char [ size ] );
        std::snprintf( buffer.get(), size, format.c_str(), args... );
        return std::string( buffer.get(), buffer.get() + size );
    }

    static std::string From( const kvs::Int8 val, const int width, const char fill = 0 );
    static std::string From( const kvs::Int16 val, const int width, const char fill = 0 );
    static std::string From( const kvs::Int32 val, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt8 val, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt16 val, const int width, const char fill = 0 );
    static std::string From( const kvs::UInt32 val, const int width, const char fill = 0 );
    static std::string From( const kvs::Real32 val, const int precision, const bool fixed = false, const bool scientific = false );
    static std::string From( const kvs::Real64 val, const int precision, const bool fixed = false, const bool scientific = false );
    static std::string FromFile( const std::string& filename );
    static std::string ToUpper( const std::string& str );
    static std::string ToLower( const std::string& str );
    static std::string Replace( const std::string& source, const std::string& pattern, const std::string& placement );

private:
    String();

public:
    template <typename T>
    KVS_DEPRECATED( static std::string ToString( const T& val ) ) { return From<T>( val ); }

    template <typename T>
    KVS_DEPRECATED( static std::string ToString(
        const T& val,
        const int precision,
        const bool fixed = false,
        const bool scientific = false ) )
    {
        return From( val, precision, fixed, scientific );
    }

    template <typename T>
    KVS_DEPRECATED( static std::string ToString( const T& val, const std::string& format ) )
    {
        char buffer[100];
        if ( std::sprintf( buffer, format.c_str(), val ) >= 0 )
        {
            return std::string( buffer );
        }
        return std::string("");
    }
};

template <>
inline kvs::Int8 String::To( const std::string& str )
{
    int ret; std::istringstream ss( str ); ss >> ret;
    return static_cast<kvs::Int8>( ret );
}

template <>
inline kvs::UInt8 String::To( const std::string& str )
{
    int ret; std::istringstream ss( str ); ss >> ret;
    return static_cast<kvs::UInt8>( ret );
}

template <>
inline std::string String::To( const std::string& str )
{
    return str;
}

template <>
inline std::string String::From( const kvs::Int8& val )
{
    std::ostringstream ss; ss << static_cast<int>(val);
    return ss.str();
}

template <>
inline std::string String::From( const kvs::UInt8& val )
{
    std::ostringstream ss; ss << static_cast<int>(val);
    return ss.str();
}

template <>
inline std::string String::From( const std::string& val )
{
    return val;
}

template <>
inline std::string String::From( const kvs::Int8& val, const char )
{
    std::ostringstream ss; ss << static_cast<int>(val);
    return ss.str();
}

template <>
inline std::string String::From( const kvs::UInt8& val, const char )
{
    std::ostringstream ss; ss << static_cast<int>(val);
    return ss.str();
}

} // end of namespace kvs
