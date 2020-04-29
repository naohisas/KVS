/*****************************************************************************/
/**
 *  @file   Value.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "picojson.h"
#include "Object.h"
#include <kvs/Type>


namespace kvs
{

namespace json
{

/*===========================================================================*/
/**
 *  @brief  JSON value class.
 */
/*===========================================================================*/
class Value : public picojson::value
{
public:
    typedef picojson::value SuperClass;

public:
    Value() {}
    Value( const SuperClass& value ): SuperClass( value ) {}
    Value( SuperClass&& value ) noexcept : SuperClass( value ) {}
    explicit Value( const kvs::Int16 value ) : SuperClass( kvs::Real64(value) ) {}
    explicit Value( const kvs::UInt16 value ) : SuperClass( kvs::Real64(value) ) {}
    explicit Value( const kvs::Int32 value ) : SuperClass( kvs::Real64(value) ) {}
    explicit Value( const kvs::UInt32 value ) : SuperClass( kvs::Real64(value) ) {}
    explicit Value( const kvs::Real32 value ) : SuperClass( kvs::Real64(value) ) {}
    explicit Value( const kvs::Real64 value ) : SuperClass( value ) {}
    explicit Value( const std::string& value ) : SuperClass( value ) {}
    explicit Value( std::string&& value ) noexcept : SuperClass( value ) {}
    explicit Value( const kvs::json::Object& object ) : SuperClass( object ) {}
};

} // end of namespace json

} // end of namespace kvs
