/*****************************************************************************/
/**
 *  @file   Object.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "picojson.h"
#include <kvs/Type>


namespace kvs
{

namespace json
{

class Array;
class Value;

/*===========================================================================*/
/**
 *  @brief  JSON Object class.
 */
/*===========================================================================*/
class Object : public picojson::object
{
public:
    typedef picojson::object SuperClass;

public:
    Object() {}
    Object( const SuperClass& object ): SuperClass( object ) {}
    Object( SuperClass&& object ) noexcept : SuperClass( object ) {}

    void add( const std::string& key, const kvs::Int16 value );
    void add( const std::string& key, const kvs::UInt16 value );
    void add( const std::string& key, const kvs::Int32 value );
    void add( const std::string& key, const kvs::UInt32 value );
    void add( const std::string& key, const kvs::Real32 value );
    void add( const std::string& key, const kvs::Real64 value );
    void add( const std::string& key, const std::string& value );
    void add( const std::string& key, const kvs::json::Value& value );
    void add( const std::string& key, const kvs::json::Array& value );
    void add( const std::string& key, const kvs::json::Object& value );
};

} // end of namespace json

} // end of namespace kvs
