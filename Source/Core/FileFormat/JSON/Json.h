/*****************************************************************************/
/**
 *  @file   Json.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "Value.h"
#include "Object.h"
#include "Array.h"
#include <string>
#include <kvs/FileFormatBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  JSON file reader class.
 */
/*===========================================================================*/
class Json : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;

public:
    typedef kvs::json::Value::SuperClass Value;
    typedef kvs::json::Object::SuperClass Object;
    typedef kvs::json::Array::SuperClass Array;

private:
    Value m_value; ///< JSON data

public:
    Json() {}
    Json( const Value& value ) : m_value( value ) {}
    Json( Value&& value ) noexcept : m_value( value ) {}
    Json( const std::string& filename ) { this->read( filename ); }

    Object& rootObject() { return m_value.get<Object>(); }
    const Object& rootObject() const { return m_value.get<Object>(); }

    Value& value() { return m_value; }
    const Value& value() const { return m_value; }

    bool read( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs
