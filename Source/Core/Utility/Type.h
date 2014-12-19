/****************************************************************************/
/**
 *  @file Type.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Type.h 1336 2012-10-30 22:37:49Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS_CORE_TYPE_H_INCLUDE
#define KVS_CORE_TYPE_H_INCLUDE

#include <typeinfo>
#include <kvs/Compiler>
#include <kvs/Platform>

#if KVS_ENABLE_DEPRECATED
#include <string>
#endif

namespace kvs
{

typedef char    Int8;
typedef unsigned char  UInt8;
typedef short          Int16;
typedef unsigned short UInt16;
typedef int            Int32;
typedef unsigned int   UInt32;

#if defined ( KVS_COMPILER_VC )
typedef signed __int64   Int64;
typedef unsigned __int64 UInt64;
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
typedef long          Int64;
typedef unsigned long UInt64;
#else                               // LLP64
typedef long long          Int64;
typedef unsigned long long UInt64;
#endif
#endif

typedef float       Real32;
typedef double      Real64;
#if KVS_ENABLE_DEPRECATED
typedef long double Real128;
#endif


class Type
{
public:
    enum TypeID
    {
        TypeInt8,
        TypeInt16,
        TypeInt32,
        TypeInt64,
        TypeUInt8,
        TypeUInt16,
        TypeUInt32,
        TypeUInt64,
        TypeReal32,
        TypeReal64,
#if KVS_ENABLE_DEPRECATED
        TypeString,
#endif
        UnknownType
    };

    template <typename T>
    static TypeID GetID()
    {
        return UnknownType;
    }

    static TypeID GetID( const std::type_info& type );
};

template <> inline Type::TypeID Type::GetID<kvs::Int8  >() { return TypeInt8;   }
template <> inline Type::TypeID Type::GetID<kvs::Int16 >() { return TypeInt16;  }
template <> inline Type::TypeID Type::GetID<kvs::Int32 >() { return TypeInt32;  }
template <> inline Type::TypeID Type::GetID<kvs::Int64 >() { return TypeInt64;  }
template <> inline Type::TypeID Type::GetID<kvs::UInt8 >() { return TypeUInt8;  }
template <> inline Type::TypeID Type::GetID<kvs::UInt16>() { return TypeUInt16; }
template <> inline Type::TypeID Type::GetID<kvs::UInt32>() { return TypeUInt32; }
template <> inline Type::TypeID Type::GetID<kvs::UInt64>() { return TypeUInt64; }
template <> inline Type::TypeID Type::GetID<kvs::Real32>() { return TypeReal32; }
template <> inline Type::TypeID Type::GetID<kvs::Real64>() { return TypeReal64; }
#if KVS_ENABLE_DEPRECATED
template <> inline Type::TypeID Type::GetID<std::string>() { return TypeString; }
#endif

} // end of namespace kvs

#endif // KVS_CORE_TYPE_H_INCLUDE
