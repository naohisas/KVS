/****************************************************************************/
/**
 *  @file   Type.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <typeinfo>
#include <kvs/Compiler>
#include <kvs/Platform>

namespace kvs
{

using Int8 = char;
using UInt8 = unsigned char;
using Int16 = short;
using UInt16 = unsigned short;
using Int32 = int;
using UInt32 = unsigned int;
#if defined ( KVS_COMPILER_VC )
using Int64 = signed __int64;
using UInt64 = unsigned __int64;
#else
#if defined ( KVS_PLATFORM_CPU_64 ) // LP64
using Int64 = long;
using UInt64 = unsigned long;
#else                               // LLP64
using Int64 = long long;
using UInt64 = unsigned long long;
#endif
#endif
using Real32 = float;
using Real64 = double;


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

} // end of namespace kvs
