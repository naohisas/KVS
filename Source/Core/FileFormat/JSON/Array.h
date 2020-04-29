/*****************************************************************************/
/**
 *  @file   Array.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "picojson.h"
#include <vector>
#include <kvs/ValueArray>


namespace kvs
{

namespace json
{

/*===========================================================================*/
/**
 *  @brief  JSON Array class.
 */
/*===========================================================================*/
class Array : public picojson::array
{
public:
    typedef picojson::array SuperClass;

public:
    Array() {}
    Array( const SuperClass& array ): SuperClass( array ) {}
    Array( SuperClass&& array ) noexcept : SuperClass( array ) {}

    template <typename T>
    explicit Array( const std::vector<T>& array ) { this->set<T>( array ); }

    template <typename T>
    explicit Array( const std::initializer_list<T>& array ) { this->set<T>( array ); }

    template <typename T>
    explicit Array( const kvs::ValueArray<T>& array ) { this->set<T>( array ); }

    template <typename T>
    void set( const std::vector<T>& array ) { for ( auto& e : array ) { this->add(e); } }

    template <typename T>
    void set( const std::initializer_list<T>& array ) { for ( auto& e : array ) { this->add(e); } }

    template <typename T>
    void set( const kvs::ValueArray<T>& array ) { for ( auto& e : array ) { this->add(e); } }

    void add( const kvs::Int16 value );
    void add( const kvs::UInt16 value );
    void add( const kvs::Int32 value );
    void add( const kvs::UInt32 value );
    void add( const kvs::Real32 value );
    void add( const kvs::Real64 value );
    void add( const std::string& value );
};

} // end of namespace json

} // end of namespace kvs
