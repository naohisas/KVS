/****************************************************************************/
/**
 *  @file AnyValueArray.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AnyValueArray.cpp 1244 2012-07-10 11:44:23Z s.yamada0808@gmail.com $
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

}

namespace kvs
{

namespace temporal
{

TypeInfo::TypeInfo()
{
    m_id = kvs::Type::UnknownType;
}

TypeInfo::TypeInfo( kvs::Type::TypeID id )
{
    m_id = id;
}

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
#if KVS_ENABLE_DEPRECATED
    case kvs::Type::TypeString: return typeid( std::string );
#endif
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
#if KVS_ENABLE_DEPRECATED
    case kvs::Type::TypeString: return "string";
#endif
    default: break;
    }
    KVS_ASSERT( false );
    return NULL;
}

} // temporal

AnyValueArray::AnyValueArray()
{
    m_size = 0;
    m_size_of_value = 0;
    m_type_id = kvs::Type::UnknownType;
}

size_t AnyValueArray::byteSize() const
{
    return this->size() * m_size_of_value;
}

const kvs::SharedPointer<void>& AnyValueArray::sharedPointer() const
{
    return m_values;
}

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
#if KVS_ENABLE_DEPRECATED
    case kvs::Type::TypeString: return ::MakeClone<std::string>( data, size );
#endif
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

#if KVS_ENABLE_DEPRECATED
void AnyValueArray::deallocate()
{
    this->release();
}
#endif

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
#if KVS_ENABLE_DEPRECATED
template AnyValueArray::AnyValueArray( const kvs::ValueArray<std::string>& values );
#endif

//template kvs::Int8&   AnyValueArray::at<kvs::Int8>( const size_t index );
//template kvs::UInt8&  AnyValueArray::at<kvs::UInt8>( const size_t index );
//template kvs::Int16&  AnyValueArray::at<kvs::Int16>( const size_t index );
//template kvs::UInt16& AnyValueArray::at<kvs::UInt16>( const size_t index );
//template kvs::Int32&  AnyValueArray::at<kvs::Int32>( const size_t index );
//template kvs::UInt32& AnyValueArray::at<kvs::UInt32>( const size_t index );
//template kvs::Int64&  AnyValueArray::at<kvs::Int64>( const size_t index );
//template kvs::UInt64& AnyValueArray::at<kvs::UInt64>( const size_t index );
//template kvs::Real32& AnyValueArray::at<kvs::Real32>( const size_t index );
//template kvs::Real64& AnyValueArray::at<kvs::Real64>( const size_t index );
//template std::string& AnyValueArray::at<std::string>( const size_t index );
//
//template const kvs::Int8&   AnyValueArray::at<kvs::Int8>( const size_t index ) const;
//template const kvs::UInt8&  AnyValueArray::at<kvs::UInt8>( const size_t index ) const;
//template const kvs::Int16&  AnyValueArray::at<kvs::Int16>( const size_t index ) const ;
//template const kvs::UInt16& AnyValueArray::at<kvs::UInt16>( const size_t index ) const;
//template const kvs::Int32&  AnyValueArray::at<kvs::Int32>( const size_t index ) const;
//template const kvs::UInt32& AnyValueArray::at<kvs::UInt32>( const size_t index ) const;
//template const kvs::Int64&  AnyValueArray::at<kvs::Int64>( const size_t index ) const;
//template const kvs::UInt64& AnyValueArray::at<kvs::UInt64>( const size_t index ) const;
//template const kvs::Real32& AnyValueArray::at<kvs::Real32>( const size_t index ) const;
//template const kvs::Real64& AnyValueArray::at<kvs::Real64>( const size_t index ) const;
//template const std::string& AnyValueArray::at<std::string>( const size_t index ) const;

//template kvs::Int8   AnyValueArray::to<kvs::Int8>( const size_t index ) const;
//template kvs::UInt8  AnyValueArray::to<kvs::UInt8>( const size_t index ) const;
//template kvs::Int16  AnyValueArray::to<kvs::Int16>( const size_t index ) const;
//template kvs::UInt16 AnyValueArray::to<kvs::UInt16>( const size_t index ) const;
//template kvs::Int32  AnyValueArray::to<kvs::Int32>( const size_t index ) const;
//template kvs::UInt32 AnyValueArray::to<kvs::UInt32>( const size_t index ) const;
//template kvs::Int64  AnyValueArray::to<kvs::Int64>( const size_t index ) const;
//template kvs::UInt64 AnyValueArray::to<kvs::UInt64>( const size_t index ) const;
//template kvs::Real32 AnyValueArray::to<kvs::Real32>( const size_t index ) const;
//template kvs::Real64 AnyValueArray::to<kvs::Real64>( const size_t index ) const;
//template std::string AnyValueArray::to<std::string>( const size_t index ) const;

//template const kvs::Int8*   AnyValueArray::pointer<kvs::Int8>() const;
//template const kvs::UInt8*  AnyValueArray::pointer<kvs::UInt8>() const;
//template const kvs::Int16*  AnyValueArray::pointer<kvs::Int16>() const;
//template const kvs::UInt16* AnyValueArray::pointer<kvs::UInt16>() const;
//template const kvs::Int32*  AnyValueArray::pointer<kvs::Int32>() const;
//template const kvs::UInt32* AnyValueArray::pointer<kvs::UInt32>() const;
//template const kvs::Int64*  AnyValueArray::pointer<kvs::Int64>() const;
//template const kvs::UInt64* AnyValueArray::pointer<kvs::UInt64>() const;
//template const kvs::Real32* AnyValueArray::pointer<kvs::Real32>() const;
//template const kvs::Real64* AnyValueArray::pointer<kvs::Real64>() const;
//template const std::string* AnyValueArray::pointer<std::string>() const;
//
//template kvs::Int8*   AnyValueArray::pointer<kvs::Int8>();
//template kvs::UInt8*  AnyValueArray::pointer<kvs::UInt8>();
//template kvs::Int16*  AnyValueArray::pointer<kvs::Int16>();
//template kvs::UInt16* AnyValueArray::pointer<kvs::UInt16>();
//template kvs::Int32*  AnyValueArray::pointer<kvs::Int32>();
//template kvs::UInt32* AnyValueArray::pointer<kvs::UInt32>();
//template kvs::Int64*  AnyValueArray::pointer<kvs::Int64>();
//template kvs::UInt64* AnyValueArray::pointer<kvs::UInt64>();
//template kvs::Real32* AnyValueArray::pointer<kvs::Real32>();
//template kvs::Real64* AnyValueArray::pointer<kvs::Real64>();
//template std::string* AnyValueArray::pointer<std::string>();

#if KVS_ENABLE_DEPRECATED

template AnyValueArray::AnyValueArray( const kvs::Int8* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::UInt8* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::Int16* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::UInt16* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::Int32* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::UInt32* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::Int64* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::UInt64* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::Real32* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const kvs::Real64* values, const size_t nvalues );
template AnyValueArray::AnyValueArray( const std::string* values, const size_t nvalues );

template AnyValueArray::AnyValueArray( const std::vector<kvs::Int8>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::UInt8>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::Int16>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::UInt16>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::Int32>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::UInt32>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::Int64>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::UInt64>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::Real32>& values );
template AnyValueArray::AnyValueArray( const std::vector<kvs::Real64>& values );
template AnyValueArray::AnyValueArray( const std::vector<std::string>& values );

template void* AnyValueArray::allocate<kvs::Int8>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::UInt8>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::Int16>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::UInt16>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::Int32>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::UInt32>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::Int64>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::UInt64>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::Real32>( const size_t nvalues );
template void* AnyValueArray::allocate<kvs::Real64>( const size_t nvalues );
template void* AnyValueArray::allocate<std::string>( const size_t nvalues );

template void AnyValueArray::shallowCopy<kvs::Int8>( const kvs::ValueArray<kvs::Int8>& values );
template void AnyValueArray::shallowCopy<kvs::UInt8>( const kvs::ValueArray<kvs::UInt8>& values );
template void AnyValueArray::shallowCopy<kvs::Int16>( const kvs::ValueArray<kvs::Int16>& values );
template void AnyValueArray::shallowCopy<kvs::UInt16>( const kvs::ValueArray<kvs::UInt16>& values );
template void AnyValueArray::shallowCopy<kvs::Int32>( const kvs::ValueArray<kvs::Int32>& values );
template void AnyValueArray::shallowCopy<kvs::UInt32>( const kvs::ValueArray<kvs::UInt32>& values );
template void AnyValueArray::shallowCopy<kvs::Int64>( const kvs::ValueArray<kvs::Int64>& values );
template void AnyValueArray::shallowCopy<kvs::UInt64>( const kvs::ValueArray<kvs::UInt64>& values );
template void AnyValueArray::shallowCopy<kvs::Real32>( const kvs::ValueArray<kvs::Real32>& values );
template void AnyValueArray::shallowCopy<kvs::Real64>( const kvs::ValueArray<kvs::Real64>& values );
template void AnyValueArray::shallowCopy<std::string>( const kvs::ValueArray<std::string>& values );

template void AnyValueArray::deepCopy<kvs::Int8>( const kvs::Int8* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::UInt8>( const kvs::UInt8* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::Int16>( const kvs::Int16* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::UInt16>( const kvs::UInt16* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::Int32>( const kvs::Int32* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::UInt32>( const kvs::UInt32* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::Int64>( const kvs::Int64* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::UInt64>( const kvs::UInt64* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::Real32>( const kvs::Real32* values, const size_t nvalues );
template void AnyValueArray::deepCopy<kvs::Real64>( const kvs::Real64* values, const size_t nvalues );
template void AnyValueArray::deepCopy<std::string>( const std::string* values, const size_t nvalues );

#else

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

#endif

} // end of namespace kvs
