/****************************************************************************/
/**
 *  @file   AnyValueArray.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "AnyValueArray.h"
#include <kvs/Platform>
#include <kvs/Compiler>


namespace
{

template <typename T>
kvs::AnyValueArray MakeClone( const void* data, size_t size )
{
    kvs::ValueArray<T> ret( static_cast<const T*>( data ), size );
    return kvs::AnyValueArray( ret );
}

} // end of namespace


namespace kvs
{

namespace temporal
{

const std::type_info& TypeInfo::type() const
{
    switch ( m_id )
    {
    case kvs::Type::TypeInt8:   return typeid( kvs::Int8   );
    case kvs::Type::TypeInt16:  return typeid( kvs::Int16  );
    case kvs::Type::TypeInt32:  return typeid( kvs::Int32  );
    case kvs::Type::TypeInt64:  return typeid( kvs::Int64  );
    case kvs::Type::TypeUInt8:  return typeid( kvs::UInt8  );
    case kvs::Type::TypeUInt16: return typeid( kvs::UInt16 );
    case kvs::Type::TypeUInt32: return typeid( kvs::UInt32 );
    case kvs::Type::TypeUInt64: return typeid( kvs::UInt64 );
    case kvs::Type::TypeReal32: return typeid( kvs::Real32 );
    case kvs::Type::TypeReal64: return typeid( kvs::Real64 );
    default: break;
    }
    KVS_ASSERT( false );
    return typeid( void );
}

const char* TypeInfo::typeName() const
{
    switch ( m_id )
    {
    case kvs::Type::TypeInt8:  return "char";
    case kvs::Type::TypeInt16: return "short";
    case kvs::Type::TypeInt32: return "int";
    case kvs::Type::TypeInt64:
#if defined ( KVS_COMPILER_VC )
    return "signed __int64";
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
    return "long";
#else
    return "long long";
#endif
#endif
    case kvs::Type::TypeUInt8:  return "unsigned char";
    case kvs::Type::TypeUInt16: return "unsigned short";
    case kvs::Type::TypeUInt32: return "unsigned int";
    case kvs::Type::TypeUInt64:
#if defined ( KVS_COMPILER_VC )
    return "unsigned __int64";
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
    return "unsigned long";
#else
    return "unsigned long long";
#endif
#endif
    case kvs::Type::TypeReal32: return "float";
    case kvs::Type::TypeReal64: return "double";
    default: break;
    }
    KVS_ASSERT( false );
    return nullptr;
}

} // temporal

void AnyValueArray::swap( AnyValueArray& other )
{
    std::swap( m_values, other.m_values );
    std::swap( m_size, other.m_size );
    std::swap( m_size_of_value, other.m_size_of_value );
    std::swap( m_type_id, other.m_type_id );
    std::swap( m_type_info, other.m_type_info );
}

AnyValueArray AnyValueArray::clone() const
{
    const void* data = this->data();
    size_t size = this->size();
    switch ( this->typeID() )
    {
    case kvs::Type::TypeInt8:   return ::MakeClone<kvs::Int8  >( data, size );
    case kvs::Type::TypeInt16:  return ::MakeClone<kvs::Int16 >( data, size );
    case kvs::Type::TypeInt32:  return ::MakeClone<kvs::Int32 >( data, size );
    case kvs::Type::TypeInt64:  return ::MakeClone<kvs::Int64 >( data, size );
    case kvs::Type::TypeUInt8:  return ::MakeClone<kvs::UInt8 >( data, size );
    case kvs::Type::TypeUInt16: return ::MakeClone<kvs::UInt16>( data, size );
    case kvs::Type::TypeUInt32: return ::MakeClone<kvs::UInt32>( data, size );
    case kvs::Type::TypeUInt64: return ::MakeClone<kvs::UInt64>( data, size );
    case kvs::Type::TypeReal32: return ::MakeClone<kvs::Real32>( data, size );
    case kvs::Type::TypeReal64: return ::MakeClone<kvs::Real64>( data, size );
    default: break;
    }
    KVS_ASSERT( false );
    return AnyValueArray();
}

void AnyValueArray::release()
{
    m_values.reset();
    m_size = 0;
    m_size_of_value = 0;
    m_type_id = kvs::Type::UnknownType;
    m_type_info = TypeInfo();
}

bool AnyValueArray::unique() const
{
    return m_values.unique();
}

template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Int8>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::UInt8>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Int16>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::UInt16>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Int32>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::UInt32>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Int64>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::UInt64>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Real32>& values );
template AnyValueArray::AnyValueArray( const kvs::ValueArray<kvs::Real64>& values );

template void AnyValueArray::allocate<kvs::Int8>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::UInt8>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::Int16>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::UInt16>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::Int32>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::UInt32>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::Int64>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::UInt64>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::Real32>( const size_t nvalues );
template void AnyValueArray::allocate<kvs::Real64>( const size_t nvalues );

} // end of namespace kvs
