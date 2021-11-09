/****************************************************************************/
/**
 *  @file   AnyValueArray.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <typeinfo>
#include <utility>
#include <kvs/Type>
#include <kvs/SharedPointer>
#include <kvs/ValueArray>
#include "StaticAssert.h"


namespace kvs
{

namespace temporal
{

class TypeInfo
{
private:
    kvs::Type::TypeID m_id = kvs::Type::UnknownType;

public:
    TypeInfo() = default;
    TypeInfo( kvs::Type::TypeID id ): m_id( id ) {}

public:
    const std::type_info& type() const;
    const char* typeName() const;
}; // TypeInfo

template <typename T, T Value>
struct integral_constant
{
    static const T value = Value;
    using value_type = T;
};

//typedef integral_constant<bool, true> true_type;
//typedef integral_constant<bool, false> false_type;
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

} // temporal

namespace detail
{

class ValueGetter
{
public:
    template <typename ReturnT>
    static ReturnT Get( const void* ptr, kvs::Type::TypeID id )
    {
        KVS_ASSERT( id != kvs::Type::UnknownType );
        switch ( id )
        {
        case kvs::Type::TypeInt8:   return GetValue<ReturnT, kvs::Int8>  ( ptr );
        case kvs::Type::TypeInt16:  return GetValue<ReturnT, kvs::Int16> ( ptr );
        case kvs::Type::TypeInt32:  return GetValue<ReturnT, kvs::Int32> ( ptr );
        case kvs::Type::TypeInt64:  return GetValue<ReturnT, kvs::Int64> ( ptr );
        case kvs::Type::TypeUInt8:  return GetValue<ReturnT, kvs::UInt8> ( ptr );
        case kvs::Type::TypeUInt16: return GetValue<ReturnT, kvs::UInt16>( ptr );
        case kvs::Type::TypeUInt32: return GetValue<ReturnT, kvs::UInt32>( ptr );
        case kvs::Type::TypeUInt64: return GetValue<ReturnT, kvs::UInt64>( ptr );
        case kvs::Type::TypeReal32: return GetValue<ReturnT, kvs::Real32>( ptr );
        case kvs::Type::TypeReal64: return GetValue<ReturnT, kvs::Real64>( ptr );
        default: break;
        }
        return ReturnT();
    }

private:
    template <typename ReturnT, typename DataT>
    static ReturnT GetValue( const void* ptr )
    {
        return static_cast<ReturnT>( *static_cast<const DataT*>( ptr ) );
    }

private:
    ValueGetter();
}; // ValueGetter

class AnyValueArrayElement
{
private:
    const void* m_ptr = nullptr;
    kvs::Type::TypeID m_type_id = kvs::Type::UnknownType;

public:
    AnyValueArrayElement( const void* ptr, kvs::Type::TypeID type_id )
    {
        m_ptr = ptr;
        m_type_id = type_id;
    }

    template <typename T>
    const T to() const
    {
        return ValueGetter::Get<T>( m_ptr, m_type_id );
    }
}; // AnyValueArrayElement

template <typename T>
class AnyValueArrayIterator
{
public:
    using this_type = AnyValueArrayIterator;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

private:
    char* m_ptr = nullptr;
    size_t m_size_of_value = 0;
    kvs::Type::TypeID m_id = kvs::Type::UnknownType;

public:
    AnyValueArrayIterator( void* ptr, size_t size_of_value, kvs::Type::TypeID id )
    {
        m_ptr = static_cast<char*>( ptr );
        m_size_of_value = size_of_value;
        m_id = id;
    }

public:
    value_type operator *() const
    {
        return ValueGetter::Get<T>( m_ptr, m_id );
    }

    value_type operator []( difference_type n ) const
    {
        return ValueGetter::Get<T>( m_ptr + m_size_of_value * n, m_id );
    }

public:
    this_type& operator ++()
    {
        m_ptr += m_size_of_value;
        return *this;
    }

    this_type operator ++( int )
    {
        this_type temp( *this );
        ++( *this );
        return temp;
    }

    this_type& operator --()
    {
        m_ptr -= m_size_of_value;
        return *this;
    }

    this_type operator --( int )
    {
        this_type temp( *this );
        --( *this );
        return temp;
    }

public:
    this_type& operator +=( difference_type right )
    {
        m_ptr += m_size_of_value * right;
        return *this;
    }

    this_type operator +( difference_type right ) const
    {
        return this_type( *this ) += right;
    }

    friend this_type operator +( difference_type left, const this_type& right )
    {
        return right + left;
    }

    this_type& operator -=( difference_type right )
    {
        m_ptr -= m_size_of_value * right;
        return *this;
    }

    this_type operator -( difference_type right ) const
    {
        return this_type( *this ) -= right;
    }

    difference_type operator -( const this_type& right ) const
    {
        return ( m_ptr - right.m_ptr ) / m_size_of_value;
    }

public:
    bool operator ==( const this_type& other ) const
    {
        return m_ptr == other.m_ptr;
    }

    bool operator !=( const this_type& other ) const
    {
        return !( *this == other );
    }

    bool operator <( const this_type& right ) const
    {
        return m_ptr < right.m_ptr;
    }

    bool operator >( const this_type& right ) const
    {
        return right < *this;
    }

    bool operator <=( const this_type& right ) const
    {
        return !( right < *this );
    }

    bool operator >=( const this_type& right ) const
    {
        return !( *this < right );
    }
}; // AnyValueArrayIterator

} // detail


/*==========================================================================*/
/**
 *  Any type array class.
 */
/*==========================================================================*/
class AnyValueArray
{
public:
    using TypeInfo = kvs::temporal::TypeInfo;
    using value_type = kvs::detail::AnyValueArrayElement;

private:
    kvs::SharedPointer<void> m_values{}; ///< value array
    size_t m_size_of_value = 0; ///< byte size of a value
    size_t m_size = 0; ///< number of values
    kvs::Type::TypeID m_type_id = kvs::Type::UnknownType;
    TypeInfo m_type_info{};

public:
    AnyValueArray() = default;

    template<typename T>
    AnyValueArray( const kvs::ValueArray<T>& values ):
        m_type_info( kvs::Type::GetID<T>() )
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        m_values = values.sharedPointer();
        m_size = values.size();
        m_size_of_value = sizeof( T );
        m_type_id = kvs::Type::GetID<T>();
    }

public:
    AnyValueArray& operator =( const AnyValueArray& rhs )
    {
        AnyValueArray temp( rhs );
        temp.swap( *this );
        return *this;
    }

    template <typename T>
    kvs::ValueArray<T> asValueArray() const
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        KVS_ASSERT( this->check_type<T>() );
        return kvs::ValueArray<T>( kvs::static_pointer_cast<T>( m_values ), this->size() );
    }

public:
    size_t size() const
    {
        return m_size;
    }

    size_t byteSize() const
    {
        return this->size() * m_size_of_value;
    }

    const kvs::SharedPointer<void>& sharedPointer() const
    {
        return m_values;
    }

    const void* data() const
    {
        return m_values.get();
    }

    void* data()
    {
        return m_values.get();
    }

    bool empty() const
    {
        return this->size() == 0;
    }

    kvs::Type::TypeID typeID() const
    {
        return m_type_id;
    }

    void swap( AnyValueArray& other );

    AnyValueArray clone() const;

    void release();

    bool unique() const;

    const value_type operator []( size_t index ) const
    {
        const void* ptr = static_cast<const char*>( this->data() ) + m_size_of_value * index;
        return value_type( ptr, this->typeID() );
    }

    template<typename T>
    T at( const size_t index ) const
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        return ( *this )[ index ].to<T>();
    }

    // for compatibility.
    // {
    const TypeInfo* typeInfo() const { return &m_type_info; }

    template<typename T>
    void allocate( const size_t size )
    {
        *this = AnyValueArray( kvs::ValueArray<T>( size ) );
    }
    // }

private:
    template <typename T>
    bool check_type() const
    {
        return m_type_id == kvs::Type::GetID<T>();
    }

    template <typename T>
    struct is_supported : kvs::temporal::false_type {};
};

template <> struct AnyValueArray::is_supported<kvs::Int8  > : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::Int16 > : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::Int32 > : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::Int64 > : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::UInt8 > : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::UInt16> : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::UInt32> : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::UInt64> : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::Real32> : kvs::temporal::true_type {};
template <> struct AnyValueArray::is_supported<kvs::Real64> : kvs::temporal::true_type {};

} // end of namespace kvs

namespace std
{

template <>
inline void swap( kvs::AnyValueArray& lhs, kvs::AnyValueArray& rhs )
{
    lhs.swap( rhs );
}

} // std
