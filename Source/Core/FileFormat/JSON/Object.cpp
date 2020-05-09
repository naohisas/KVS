/*****************************************************************************/
/**
 *  @file   Object.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Object.h"
#include "Array.h"
#include "Value.h"


namespace kvs
{

namespace json
{

void Object::add( const std::string& key, const kvs::Int16 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), static_cast<double>( value ) ) );
}

void Object::add( const std::string& key, const kvs::UInt16 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), static_cast<double>( value ) ) );
}

void Object::add( const std::string& key, const kvs::Int32 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), static_cast<double>( value ) ) );
}

void Object::add( const std::string& key, const kvs::UInt32 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), static_cast<double>( value ) ) );
}

void Object::add( const std::string& key, const kvs::Real32 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), static_cast<double>( value ) ) );
}

void Object::add( const std::string& key, const kvs::Real64 value )
{
    SuperClass::insert( std::make_pair( key.c_str(), value ) );
}

void Object::add( const std::string& key, const std::string& value )
{
    SuperClass::insert( std::make_pair( key.c_str(), value ) );
}

void Object::add( const std::string& key, const kvs::json::Value& value )
{
    SuperClass::insert( std::make_pair( key.c_str(), value ) );
}

void Object::add( const std::string& key, const kvs::json::Array& value )
{
    SuperClass::insert( std::make_pair( key.c_str(), value ) );
}

void Object::add( const std::string& key, const kvs::json::Object& value )
{
    SuperClass::insert( std::make_pair( key.c_str(), value ) );
}

} // end of namespace json

} // end of namespace kvs
