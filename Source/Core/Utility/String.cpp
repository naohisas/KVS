/****************************************************************************/
/**
 *  @file String.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: String.cpp 1349 2012-11-13 07:09:38Z s.yamada0808@gmail.com $
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
#if KVS_ENABLE_DEPRECATED
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <kvs/DebugNew>
#include <kvs/Message>
#include <kvs/Assert>
#include <kvs/IgnoreUnusedVariable>
#endif

#if KVS_ENABLE_DEPRECATED
namespace
{

inline size_t Length( const char* str )
{
    register const char* p = str;
    while ( *p ) p++;

    return( p - str );
}

inline char* Copy( char* dst, register const char* src)
{
    register char* p = dst;
    while ( ( *p++ = *src++ ) != '\0' ) ;

    return( p );
}

inline int Compare( register const char* str1, register const char* str2 )
{
    for ( ; ( *str1 == *str2 ) && ( *str1 != '\0' ); str1++, str2++ ) ;

    return( *str1 - *str2 );
}

}
#endif
namespace kvs
{

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
//
//std::string String::Format( const char* str, ... )
//{
//    const int max_buffer_size = 65536;
//    int buffer_size = 256;
//    std::vector<char> buffer;
//    int size = 0;
//
//    va_list args;
//    for (;;)
//    {
//        buffer.resize( buffer_size );
//
//        va_start( args, str );
//        size = vsnprintf( &buffer[0], buffer.size() - 1, str, args );
//        va_end( args );
//
//        if ( size >= 0 && size < (int)buffer.size() )
//            break;
//
//        buffer_size *= 2;
//        if ( buffer_size > max_buffer_size )
//            KVS_THROW( kvs::ArgumentException, "Too long string." );
//    }
//    return std::string( &buffer[0], size );
//}

std::string String::FromFile( const std::string& filename )
{
    std::ifstream ifs( filename.c_str() );
    if ( !ifs )
        KVS_THROW( kvs::FileReadFaultException, "Cannot open '" + filename + "'." );

    std::ostringstream buffer;
    buffer << ifs.rdbuf();

    return buffer.str();
}


#if KVS_ENABLE_DEPRECATED
String::String( void ):
    m_data( 0 ),
    m_size( 0 )
{
}

String::String( const String& str ):
    m_data( 0 ),
    m_size( 0 )
{
    *this = str;
}

String::String( const std::string& str ):
    m_data( 0 ),
    m_size( 0 )
{
    *this = str;
}

String::~String( void )
{
    if ( m_data )
    {
        delete [] m_data;
    }

    m_size = 0;
}

char& String::operator []( size_t index )
{
    KVS_ASSERT( index <= m_size );

    return( m_data[ index ] );
}

const char String::operator []( size_t index ) const
{
    KVS_ASSERT( index <= m_size );

    return( m_data[ index ] );
}

String& String::operator =( const std::string& str )
{
    if ( str.size() > 0 )
    {
        size_t size = str.size();

//        if ( m_data ) delete [] m_data;

        m_data = new char [ size + 1 ];
        if ( !m_data )
        {
            kvsMessageError( "Cannot allocate memory." );
            return( *this );
        }
        memset( m_data, '\0', size + 1 );

        ::Copy( m_data, str.c_str() );

        m_size = size;
    }

    return( *this );
}

String& String::operator =( const String& str )
{
    if ( str.size() > 0 )
    {
        size_t size = str.size();

        m_data = new char [ size + 1 ];
        if ( !m_data )
        {
            kvsMessageError( "Cannot allocate memory." );
            return( *this );
        }
        memset( m_data, '\0', size + 1 );

        ::Copy( m_data, str.data() );

        m_size = size;
    }

    return( *this );
}

String& String::operator +=( const std::string& str )
{
    if ( str.size() == 0 ) { return( *this ); }

    if ( m_size > 0 )
    {
        size_t size = m_size + str.size();

        char* data = new char [ size + 1 ];
        if ( !data )
        {
            kvsMessageError( "Cannot allocate memory." );
            return( *this );
        }
        memset( data, '\0', size + 1 );

        ::Copy( data,          m_data );
        ::Copy( data + m_size, str.c_str() );

        delete [] m_data;

        m_data = data;
        m_size = size;
    }
    else
    {
        *this = str;
    }

    return( *this );
}

String& String::operator +=( const String& str )
{
    if ( str.size() == 0 ) { return( *this ); }

    if ( m_size > 0 )
    {
        size_t size = m_size + str.size();

        char* data = new char [ size + 1 ];
        if ( !data )
        {
            kvsMessageError( "Cannot allocate memory." );
            return( *this );
        }
        memset( data, '\0', size + 1 );

        ::Copy( data,          m_data );
        ::Copy( data + m_size, str.data() );

        delete [] m_data;

        m_data = data;
        m_size = size;
    }
    else
    {
        *this = str;
    }

    return( *this );
}

const bool String::operator ==( const String& str )
{
    return( ::Compare( m_data, str.data() ) == 0 );
}

const bool String::operator !=( const String& str )
{
    return( !( *this == str ) );
}

const bool String::operator <( const String& str )
{
    return( ::Compare( m_data, str.data() ) < 0 );
}

const bool String::operator >( const String& str )
{
    return( ::Compare( m_data, str.data() ) > 0 );
}

const bool String::operator <=( const String& str )
{
    return( ::Compare( m_data, str.data() ) <= 0 );
}

const bool String::operator >=( const String& str )
{
    return( ::Compare( m_data, str.data() ) >= 0 );
}

const String operator +( const String& str1, const String& str2 )
{
    String ret( str1 ); ret += str2;
    return( ret );
}

std::ostream& operator <<( std::ostream& os, const String& str )
{
    os << str.m_data;

    return( os );
}

char& String::at( size_t index )
{
    KVS_ASSERT( index <= m_size );

    return( m_data[ index ] );
}

const char String::at( size_t index ) const
{
    KVS_ASSERT( index <= m_size );

    return( m_data[ index ] );
}

char* String::data( void )
{
    return( m_data );
}

const char* String::data( void ) const
{
    return( m_data );
}

const size_t String::size( void ) const
{
    return( m_size );
}

void String::upper( void )
{
    char* p = m_data;
    while( *p )
    {
        *p = static_cast<char>( std::toupper( *p ) );
        p++;
    }
}

void String::upper( size_t index )
{
    KVS_ASSERT( index < m_size );

    m_data[ index ] = static_cast<char>( std::toupper( m_data[ index ] ) );
}

void String::lower( void )
{
    char* p = m_data;
    while( *p )
    {
        *p = static_cast<char>( std::tolower( *p ) );
        p++;
    }
}

void String::lower( size_t index )
{
    KVS_ASSERT( index < m_size );

    m_data[ index ] = static_cast<char>( std::tolower( m_data[ index ] ) );
}

void String::format( const char* str, ... )
{
    int    size        = 0;
    size_t buffer_size = 256;
    char*  buffer      = NULL;

    va_list args;
    va_start( args, str );
    {
        for (;;)
        {
            buffer = new char [ buffer_size ];
            if ( !buffer )
            {
                kvsMessageError( "Cannot allocate memory." );
                break;
            }
            memset( buffer, '\0', buffer_size );

            size = vsprintf( buffer, str, args );
            if ( size >= 0 )
            {
                buffer[size] = '\0';
                break;
            }

            buffer_size *= 2;
            delete [] buffer;
        }
    }
    va_end( args );

    if ( buffer )
    {
        if ( m_data ) delete [] m_data;

        m_data = new char [ size + 1 ];
        if ( !m_data )
        {
            kvsMessageError( "Cannot allocate memory." );
            return;
        }
        memset( m_data, '\0', size + 1 );

        ::Copy( m_data, buffer );

        m_size = static_cast<size_t>( size );

        delete [] buffer;
    }
}

void String::replace( const std::string& pattern, const std::string& placement )
{
    const std::string source( m_data );
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

    if ( m_data ) delete [] m_data;

    *this = result;
}

template<>
const short String::toNumber<short>( int base ) const
{
    return( static_cast<short>( strtol( m_data, NULL, base ) ) );
}

template<>
const unsigned short String::toNumber<unsigned short>( int base ) const
{
    return( static_cast<unsigned short>( strtoul( m_data, NULL, base ) ) );
}

template<>
const int String::toNumber<int>( int base ) const
{
    return( static_cast<int>( strtol( m_data, NULL, base ) ) );
}

template<>
const unsigned int String::toNumber<unsigned int>( int base ) const
{
    return( static_cast<unsigned int>( strtoul( m_data, NULL, base ) ) );
}

template<>
const long String::toNumber<long>( int base ) const
{
    return( strtol( m_data, NULL, base ) );
}

template<>
const unsigned long String::toNumber<unsigned long>( int base ) const
{
    return( strtoul( m_data, NULL, base ) );
}

template<>
const float String::toNumber<float>( int base ) const
{
    kvs::IgnoreUnusedVariable( base );
    return( static_cast<float>( strtod( m_data, NULL ) ) );
}

template<>
const double String::toNumber<double>( int base ) const
{
    kvs::IgnoreUnusedVariable( base );
    return( strtod( m_data, NULL ) );
}

const std::string String::toStdString( void ) const
{
    return( std::string( m_data ) );
}

template<> String::String( short number );
template<> String::String( unsigned short number );
template<> String::String( int number );
template<> String::String( unsigned int number );
template<> String::String( long number );
template<> String::String( unsigned long number );
template<> String::String( float number );
template<> String::String( double number );

template<> void String::setNumber( short number );
template<> void String::setNumber( unsigned short number );
template<> void String::setNumber( int number );
template<> void String::setNumber( unsigned int number );
template<> void String::setNumber( long number );
template<> void String::setNumber( unsigned long number );
template<> void String::setNumber( float number );
template<> void String::setNumber( double number );

template<> const short String::toNumber( const std::string& str, int base );
template<> const unsigned short String::toNumber( const std::string& str, int base );
template<> const int String::toNumber( const std::string& str, int base );
template<> const unsigned int String::toNumber( const std::string& str, int base );
template<> const long String::toNumber( const std::string& str, int base );
template<> const unsigned long String::toNumber( const std::string& str, int base );
template<> const float String::toNumber( const std::string& str, int base );
template<> const double String::toNumber( const std::string& str, int base );

#endif

} // end of namespace kvs
