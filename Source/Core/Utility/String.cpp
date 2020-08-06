/****************************************************************************/
/**
 *  @file   String.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "String.h"
#include <cctype>
#include <cstdarg>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <kvs/Exception>


namespace
{

template <typename T>
std::string StringFromInt(
    const T val,
    const int width,
    const char fill )
{
    std::ostringstream ss;
    if ( fill ) { ss.fill( fill ); }
    ss << std::setw( width ) << val;
    return ss.str();
}

template <typename T>
std::string StringFromReal(
    const T val,
    const int precision,
    const bool fixed,
    const bool scientific )
{
    std::ostringstream ss;
    if ( scientific ) { ss.setf( std::ios_base::scientific ); }
    else { if ( fixed ) { ss.setf( std::ios_base::fixed ); } }
    ss << std::setprecision( precision ) << val;
    return ss.str();
}

} // end of namespace


namespace kvs
{

std::string String::From( const kvs::Int8 val, const int width, const char fill )
{
    return ::StringFromInt( static_cast<int>( val ), width, fill );
}

std::string String::From( const kvs::Int16 val, const int width, const char fill )
{
    return ::StringFromInt( val, width, fill );
}

std::string String::From( const kvs::Int32 val, const int width, const char fill )
{
    return ::StringFromInt( val, width, fill );
}

std::string String::From( const kvs::UInt8 val, const int width, const char fill )
{
    return ::StringFromInt( static_cast<unsigned int>( val ), width, fill );
}

std::string String::From( const kvs::UInt16 val, const int width, const char fill )
{
    return ::StringFromInt( val, width, fill );
}

std::string String::From( const kvs::UInt32 val, const int width, const char fill )
{
    return ::StringFromInt( val, width, fill );
}

std::string String::From( const kvs::Real32 val, const int precision, const bool fixed, const bool scientific )
{
    return ::StringFromReal( val, precision, fixed, scientific );
}

std::string String::From( const kvs::Real64 val, const int precision, const bool fixed, const bool scientific )
{
    return ::StringFromReal( val, precision, fixed, scientific );
}

std::string String::FromFile( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs )
        KVS_THROW( kvs::FileReadFaultException, "Cannot open '" + filename + "'." );

    std::ostringstream buffer;
    buffer << ifs.rdbuf();

    return buffer.str();
}

std::string String::ToUpper( const std::string& str )
{
    std::string ret = str;
    std::string::iterator first = ret.begin();
    std::string::iterator last = ret.end();
    while ( first != last )
    {
        *first = static_cast<char>( std::toupper( *first ) );
        ++first;
    }
    return ret;
}

std::string String::ToLower( const std::string& str )
{
    std::string ret = str;
    std::string::iterator first = ret.begin();
    std::string::iterator last = ret.end();
    while ( first != last )
    {
        *first = static_cast<char>( std::tolower( *first ) );
        ++first;
    }
    return ret;
}

std::string String::Replace(
    const std::string& source,
    const std::string& pattern,
    const std::string& placement )
{
    std::string result;
    std::string::size_type pos_before = 0;
    std::string::size_type pos = 0;
    std::string::size_type len = pattern.size();
    while ( ( pos = source.find( pattern, pos ) ) != std::string::npos )
    {
        result.append( source, pos_before, pos - pos_before );
        result.append( placement );
        pos += len;
        pos_before = pos;
    }
    result.append( source, pos_before, source.size() - pos_before );
    return result;
}

} // end of namespace kvs
