/*****************************************************************************/
/**
 *  @file   Array.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Array.h"


namespace kvs
{

namespace json
{

void Array::add( const kvs::Int16 value )
{
    this->push_back( picojson::value( static_cast<double>(value) ) );
}

void Array::add( const kvs::UInt16 value )
{
    this->push_back( picojson::value( static_cast<double>(value) ) );
}

void Array::add( const kvs::Int32 value )
{
    this->push_back( picojson::value( static_cast<double>(value) ) );
}

void Array::add( const kvs::UInt32 value )
{
    this->push_back( picojson::value( static_cast<double>(value) ) );
}

void Array::add( const kvs::Real32 value )
{
    this->push_back( picojson::value( static_cast<double>(value) ) );
}

void Array::add( const kvs::Real64 value )
{
    this->push_back( picojson::value( value ) );
}

void Array::add( const std::string& value )
{
    this->push_back( picojson::value( value ) );
}

} // end of namespace json

} // end of namespace kvs
