/****************************************************************************/
/**
 *  @file AnyValue.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AnyValue.cpp 1141 2012-05-28 10:40:42Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#if KVS_ENABLE_DEPRECATED
#include "AnyValue.h"
#include <string>


namespace kvs
{

AnyValue::TypeInfo::~TypeInfo()
{
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Int8>::typeName() const
{
    return "char";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::UInt8>::typeName() const
{
    return "unsigned char";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Int16>::typeName() const
{
    return "short";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::UInt16>::typeName() const
{
    return "unsigned short";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Int32>::typeName() const
{
    return "int";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::UInt32>::typeName() const
{
    return "unsigned int";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Int64>::typeName() const
{
#if defined ( KVS_COMPILER_VC )
    return "signed __int64";
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
    return "long";
#else
    return "long long";
#endif
#endif
}

template<>
const char* AnyValue::SetTypeInfo<kvs::UInt64>::typeName() const
{
#if defined ( KVS_COMPILER_VC )
    return "unsigned __int64";
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
    return "unsigned long";
#else
    return "unsigned long long";
#endif
#endif
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Real32>::typeName() const
{
    return "float";
}

template<>
const char* AnyValue::SetTypeInfo<kvs::Real64>::typeName() const
{
    return "double";
}

template<>
const char* AnyValue::SetTypeInfo<std::string>::typeName() const
{
    return "string";
}

AnyValue::AnyValue()
    : m_type_info( NULL )
{
    memset( &m_value, 0, sizeof( value_type ) );
}

AnyValue::AnyValue( const AnyValue& other )
    : m_value( other.m_value )
    , m_type_info( other.m_type_info ? other.m_type_info->clone() : NULL )
{
}

AnyValue::~AnyValue()
{
    if ( m_type_info ) { delete m_type_info; }
}

AnyValue& AnyValue::operator =( const AnyValue& rhs )
{
    m_value = rhs.m_value;

    if ( m_type_info ) { delete m_type_info; }
    m_type_info = rhs.m_type_info ? rhs.m_type_info->clone() : NULL;

    return *this;
}

std::ostream& operator << ( std::ostream& os, const AnyValue& rhs )
{
    const std::type_info& type = rhs.m_type_info->type();
    if      ( type == typeid( kvs::Int8   ) ) { os << rhs.m_value.i8;   }
    else if ( type == typeid( kvs::UInt8  ) ) { os << rhs.m_value.ui8;  }
    else if ( type == typeid( kvs::Int16  ) ) { os << rhs.m_value.i16;  }
    else if ( type == typeid( kvs::UInt16 ) ) { os << rhs.m_value.ui16; }
    else if ( type == typeid( kvs::Int32  ) ) { os << rhs.m_value.i32;  }
    else if ( type == typeid( kvs::UInt32 ) ) { os << rhs.m_value.ui32; }
    else if ( type == typeid( kvs::Int64  ) ) { os << rhs.m_value.i64;  }
    else if ( type == typeid( kvs::UInt64 ) ) { os << rhs.m_value.ui64; }
    else if ( type == typeid( kvs::Real32 ) ) { os << rhs.m_value.r32;  }
    else if ( type == typeid( kvs::Real64 ) ) { os << rhs.m_value.r64;  }

    return os;
}

const AnyValue::TypeInfo* AnyValue::typeInfo() const
{
    return m_type_info;
}

template<> AnyValue::AnyValue( const kvs::Int8& value );
template<> AnyValue::AnyValue( const kvs::UInt8& value );
template<> AnyValue::AnyValue( const kvs::Int16& value );
template<> AnyValue::AnyValue( const kvs::UInt16& value );
template<> AnyValue::AnyValue( const kvs::Int32& value );
template<> AnyValue::AnyValue( const kvs::UInt32& value );
template<> AnyValue::AnyValue( const kvs::Int64& value );
template<> AnyValue::AnyValue( const kvs::UInt64& value );
template<> AnyValue::AnyValue( const kvs::Real32& value );
template<> AnyValue::AnyValue( const kvs::Real64& value );

template<> AnyValue& AnyValue::operator =( const kvs::Int8& value );
template<> AnyValue& AnyValue::operator =( const kvs::UInt8& value );
template<> AnyValue& AnyValue::operator =( const kvs::Int16& value );
template<> AnyValue& AnyValue::operator =( const kvs::UInt16& value );
template<> AnyValue& AnyValue::operator =( const kvs::Int32& value );
template<> AnyValue& AnyValue::operator =( const kvs::UInt32& value );
template<> AnyValue& AnyValue::operator =( const kvs::Int64& value );
template<> AnyValue& AnyValue::operator =( const kvs::UInt64& value );
template<> AnyValue& AnyValue::operator =( const kvs::Real32& value );
template<> AnyValue& AnyValue::operator =( const kvs::Real64& value );

template<> AnyValue::operator kvs::Int8 () const;
template<> AnyValue::operator kvs::UInt8 () const;
template<> AnyValue::operator kvs::Int16 () const;
template<> AnyValue::operator kvs::UInt16 () const;
template<> AnyValue::operator kvs::Int32 () const;
template<> AnyValue::operator kvs::UInt32 () const;
template<> AnyValue::operator kvs::Int64 () const;
template<> AnyValue::operator kvs::UInt64 () const;
template<> AnyValue::operator kvs::Real32 () const;
template<> AnyValue::operator kvs::Real64 () const;

} // end of namespace kvs
#endif
