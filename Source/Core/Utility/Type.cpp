#include "Type.h"


namespace kvs
{

Type::TypeID Type::GetID( const std::type_info& type )
{
    if (      type == typeid( kvs::Int8   ) ) return TypeInt8;
    else if ( type == typeid( kvs::Int16  ) ) return TypeInt16;
    else if ( type == typeid( kvs::Int32  ) ) return TypeInt32;
    else if ( type == typeid( kvs::Int64  ) ) return TypeInt64;
    else if ( type == typeid( kvs::UInt8  ) ) return TypeUInt8;
    else if ( type == typeid( kvs::UInt16 ) ) return TypeUInt16;
    else if ( type == typeid( kvs::UInt32 ) ) return TypeUInt32;
    else if ( type == typeid( kvs::UInt64 ) ) return TypeUInt64;
    else if ( type == typeid( kvs::Real32 ) ) return TypeReal32;
    else if ( type == typeid( kvs::Real64 ) ) return TypeReal64;
#if KVS_ENABLE_DEPRECATED
    else if ( type == typeid( std::string ) ) return TypeString;
#endif
    return UnknownType;
}

}
