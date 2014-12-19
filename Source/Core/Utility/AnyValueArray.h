/****************************************************************************/
/**
 *  @file AnyValueArray.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AnyValueArray.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__ANY_VALUE_ARRAY_H_INCLUDE
#define KVS__ANY_VALUE_ARRAY_H_INCLUDE

#include <typeinfo>
#include <utility>
#if KVS_ENABLE_DEPRECATED
#include <string>
#include <sstream>
#endif
#include <kvs/Type>
#include <kvs/SharedPointer>
#include <kvs/ValueArray>
#if KVS_ENABLE_DEPRECATED
#include <kvs/Endian>
#include <kvs/String>
#include <kvs/AnyValue>
#endif
#include "StaticAssert.h"


namespace kvs
{

namespace temporal
{

class TypeInfo
{
private:
    kvs::Type::TypeID m_id;

public:
    TypeInfo();
    TypeInfo( kvs::Type::TypeID id );

public:
    const std::type_info& type() const;
    const char* typeName() const;
}; // TypeInfo

template <typename T, T Value>
struct integral_constant
{
    static const T value = Value;
    typedef T value_type;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

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
#if KVS_DEPRECATED
        case kvs::Type::TypeString: return GetValueFromString<ReturnT>( ptr );
#endif
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

#if KVS_ENABLE_DEPRECATED
    template <typename DataT>
    static std::string GetString( const void* ptr )
    {
        return kvs::String::ToString( *static_cast<const DataT*>( ptr ) );
    }

    template <typename ReturnT>
    static ReturnT GetValueFromString( const void* ptr )
    {
        const std::string& source = *static_cast<const std::string*>( ptr );
        return kvs::String::To<ReturnT>( source );
    }
#endif
private:
    ValueGetter();
}; // ValueGetter

#if KVS_ENABLE_DEPRECATED
template <>
inline std::string ValueGetter::Get<std::string>( const void* ptr, kvs::Type::TypeID id )
{
    KVS_ASSERT( id != kvs::Type::UnknownType );
    switch ( id )
    {
    case kvs::Type::TypeInt8:   return GetString<kvs::Int8>  ( ptr );
    case kvs::Type::TypeInt16:  return GetString<kvs::Int16> ( ptr );
    case kvs::Type::TypeInt32:  return GetString<kvs::Int32> ( ptr );
    case kvs::Type::TypeInt64:  return GetString<kvs::Int64> ( ptr );
    case kvs::Type::TypeUInt8:  return GetString<kvs::UInt8> ( ptr );
    case kvs::Type::TypeUInt16: return GetString<kvs::UInt16>( ptr );
    case kvs::Type::TypeUInt32: return GetString<kvs::UInt32>( ptr );
    case kvs::Type::TypeUInt64: return GetString<kvs::UInt64>( ptr );
    case kvs::Type::TypeReal32: return GetString<kvs::Real32>( ptr );
    case kvs::Type::TypeReal64: return GetString<kvs::Real64>( ptr );
    case kvs::Type::TypeString: return *static_cast<const std::string*>( ptr );
    default: break;
    }
    return std::string();
}
#endif

class AnyValueArrayElement
{
private:
    const void* m_ptr;
    kvs::Type::TypeID m_type_id;

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
    typedef AnyValueArrayIterator           this_type;
    typedef const T                         value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef std::random_access_iterator_tag iterator_category;

private:
    char* m_ptr;
    size_t m_size_of_value;
    kvs::Type::TypeID m_id;

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
    typedef kvs::temporal::TypeInfo TypeInfo;
    typedef kvs::detail::AnyValueArrayElement value_type;

private:
    kvs::SharedPointer<void> m_values;       ///< value array
    size_t    m_size_of_value; ///< byte size of a value
    size_t    m_size;       ///< number of values
    kvs::Type::TypeID m_type_id;

    TypeInfo m_type_info;

public:
    AnyValueArray();

    template<typename T>
    AnyValueArray( const kvs::ValueArray<T>& values ):
        m_type_info( kvs::Type::GetID<T>() )
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        m_values        = values.sharedPointer();
        m_size          = values.size();
        m_size_of_value = sizeof( T );
        m_type_id       = kvs::Type::GetID<T>();
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

    size_t byteSize() const;

    const kvs::SharedPointer<void>& sharedPointer() const;

    const void* data() const
    {
        return m_values.get();
    }

    void* data()
    {
        return m_values.get();
    }

    void swap( AnyValueArray& other );

    AnyValueArray clone() const;

    bool empty() const
    {
        return this->size() == 0;
    }

    const value_type operator []( size_t index ) const
    {
        const void* ptr = static_cast<const char*>( this->data() ) + m_size_of_value * index;
        return value_type( ptr, this->typeID() );
    }

    kvs::Type::TypeID typeID() const
    {
        return m_type_id;
    }

    template<typename T>
    T at( const size_t index ) const
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        return ( *this )[ index ].to<T>();
    }

    // for compatibility.

    const TypeInfo* typeInfo() const
    {
        return &m_type_info;
    }


public:
#if KVS_ENABLE_DEPRECATED

    template<typename T>
    AnyValueArray( const T* values, const size_t size )
    {
        *this = AnyValueArray( kvs::ValueArray<T>( values, size ) );
    }

    template<typename T>
    AnyValueArray( const std::vector<T>& values )
    {
        *this = AnyValueArray( kvs::ValueArray<T>( values ) );
    }

    template<typename T>
    void* allocate( const size_t size )
    {
        this->deallocate();
        *this = AnyValueArray( kvs::ValueArray<T>( size ) );
        return this->data();
    }

    bool isEmpty() const
    {
        return this->empty();
    }

    void swapByte()
    {
        switch ( this->typeID() )
        {
        case kvs::Type::TypeInt8:   kvs::Endian::Swap( static_cast<kvs::Int8*  >( this->data() ), this->size() ); break;
        case kvs::Type::TypeUInt8:  kvs::Endian::Swap( static_cast<kvs::UInt8* >( this->data() ), this->size() ); break;
        case kvs::Type::TypeInt16:  kvs::Endian::Swap( static_cast<kvs::Int16* >( this->data() ), this->size() ); break;
        case kvs::Type::TypeUInt16: kvs::Endian::Swap( static_cast<kvs::UInt16*>( this->data() ), this->size() ); break;
        case kvs::Type::TypeInt32:  kvs::Endian::Swap( static_cast<kvs::Int32* >( this->data() ), this->size() ); break;
        case kvs::Type::TypeUInt32: kvs::Endian::Swap( static_cast<kvs::UInt32*>( this->data() ), this->size() ); break;
        case kvs::Type::TypeInt64:  kvs::Endian::Swap( static_cast<kvs::Int64* >( this->data() ), this->size() ); break;
        case kvs::Type::TypeUInt64: kvs::Endian::Swap( static_cast<kvs::UInt64*>( this->data() ), this->size() ); break;
        case kvs::Type::TypeReal32: kvs::Endian::Swap( static_cast<kvs::Real32*>( this->data() ), this->size() ); break;
        case kvs::Type::TypeReal64: kvs::Endian::Swap( static_cast<kvs::Real64*>( this->data() ), this->size() ); break;
        default: break;
        }
    }

    template<typename T>
    void shallowCopy( const kvs::ValueArray<T>& values )
    {
        *this = AnyValueArray( values );
    }

    void shallowCopy( const AnyValueArray& other )
    {
        *this = other;
    }

    template<typename T>
    void deepCopy( const T* values, const size_t size )
    {
        *this = AnyValueArray( kvs::ValueArray<T>( values, size ) );
    }

    const void* pointer() const
    {
        return this->data();
    }

    void* pointer()
    {
        return this->data();
    }

    template<typename T>
    const T* pointer() const
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        KVS_ASSERT( this->check_type<T>() );
        return static_cast<const T*>( this->data() );
    }

    template<typename T>
    T* pointer()
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        KVS_ASSERT( this->check_type<T>() );
        return static_cast<T*>( this->data() );
    }

    //template<typename T>
    //T& at( const size_t index )
    //{
    //    KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
    //    KVS_ASSERT( index < this->size() );
    //    KVS_ASSERT( this->check_type<T>() );
    //    return static_cast<T*>( this->data() )[ index ];
    //}

    //template<typename T>
    //const T& at( const size_t index ) const
    //{
    //    KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
    //    KVS_ASSERT( index < this->size() );
    //    KVS_ASSERT( this->check_type<T>() );
    //    return static_cast<const T*>( this->data() )[ index ];
    //}

    template<typename T>
    T to( const size_t index ) const
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        return ( *this )[ index ].to<T>();
    }

    void deallocate();

#else
    template<typename T>
    void allocate( const size_t size )
    {
        *this = AnyValueArray( kvs::ValueArray<T>( size ) );
    }

#endif

    void release();

    bool unique() const;

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
#if KVS_ENABLE_DEPRECATED
template <> struct AnyValueArray::is_supported<std::string> : kvs::temporal::true_type {};
#endif
} // end of namespace kvs


namespace std
{

template <>
inline void swap( kvs::AnyValueArray& lhs, kvs::AnyValueArray& rhs )
{
    lhs.swap( rhs );
}

} // std

#endif // KVS__ANY_VALUE_ARRAY_H_INCLUDE
