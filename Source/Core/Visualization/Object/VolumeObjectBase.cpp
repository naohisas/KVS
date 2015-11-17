/****************************************************************************/
/**
 *  @file   VolumeObjectBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VolumeObjectBase.cpp 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "VolumeObjectBase.h"
#include <kvs/Range>


namespace
{

template<typename T>
kvs::Range GetMinMaxValues( const kvs::VolumeObjectBase* volume )
{
    KVS_ASSERT( volume->values().size() != 0 );
    KVS_ASSERT( volume->values().size() == volume->veclen() * volume->numberOfNodes() );

    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const T* const end = value + volume->numberOfNodes() * volume->veclen();

    if ( volume->veclen() == 1 )
    {
        T min_value = *value;
        T max_value = *value;

        while ( value < end )
        {
            min_value = kvs::Math::Min( *value, min_value );
            max_value = kvs::Math::Max( *value, max_value );
            ++value;
        }

        return kvs::Range( static_cast<double>( min_value ), static_cast<double>( max_value ) );
    }
    else
    {
        kvs::Real64 min_value = kvs::Value<kvs::Real64>::Max();
        kvs::Real64 max_value = kvs::Value<kvs::Real64>::Min();

        const size_t veclen = volume->veclen();

        while ( value < end )
        {
            kvs::Real64 magnitude = 0.0;
            for ( size_t i = 0; i < veclen; ++i )
            {
                magnitude += static_cast<kvs::Real64>( ( *value ) * ( *value ) );
                ++value;
            }

            min_value = kvs::Math::Min( magnitude, min_value );
            max_value = kvs::Math::Max( magnitude, max_value );
        }

        return kvs::Range( std::sqrt( min_value ), std::sqrt( max_value ) );
    }
}

}

namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new empty VolumeObjectBase.
 */
/*==========================================================================*/
VolumeObjectBase::VolumeObjectBase():
    m_volume_type( UnknownVolumeType ),
    m_label( "" ),
    m_unit( "" ),
    m_veclen( 0 ),
    m_has_min_max_values( false ),
    m_min_value( 0.0 ),
    m_max_value( 0.0 )
{
    BaseClass::setObjectType( Volume );
}

/*==========================================================================*/
/**
 *  @brief  Sets the min/max values.
 *  @param  min_value [in] Minimum value.
 *  @param  max_value [in] Maximum value.
 */
/*==========================================================================*/
void VolumeObjectBase::setMinMaxValues(
    const kvs::Real64 min_value,
    const kvs::Real64 max_value ) const
{
    m_min_value = min_value;
    m_max_value = max_value;
    m_has_min_max_values = true;
}

/*==========================================================================*/
/**
 *  @brief  Updates the min/max node value.
 */
/*==========================================================================*/
void VolumeObjectBase::updateMinMaxValues() const
{
    kvs::Range range;
    switch ( m_values.typeID() )
    {
    case kvs::Type::TypeInt8:   { range = ::GetMinMaxValues<kvs::Int8  >( this ); break; }
    case kvs::Type::TypeInt16:  { range = ::GetMinMaxValues<kvs::Int16 >( this ); break; }
    case kvs::Type::TypeInt32:  { range = ::GetMinMaxValues<kvs::Int32 >( this ); break; }
    case kvs::Type::TypeInt64:  { range = ::GetMinMaxValues<kvs::Int64 >( this ); break; }
    case kvs::Type::TypeUInt8:  { range = ::GetMinMaxValues<kvs::UInt8 >( this ); break; }
    case kvs::Type::TypeUInt16: { range = ::GetMinMaxValues<kvs::UInt16>( this ); break; }
    case kvs::Type::TypeUInt32: { range = ::GetMinMaxValues<kvs::UInt32>( this ); break; }
    case kvs::Type::TypeUInt64: { range = ::GetMinMaxValues<kvs::UInt64>( this ); break; }
    case kvs::Type::TypeReal32: { range = ::GetMinMaxValues<kvs::Real32>( this ); break; }
    case kvs::Type::TypeReal64: { range = ::GetMinMaxValues<kvs::Real64>( this ); break; }
    default: break;
    }

    this->setMinMaxValues( range.lower(), range.upper() );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys from the specified volume object.
 *  @param  object [in] pointer to the volume object
 */
/*===========================================================================*/
void VolumeObjectBase::shallowCopy( const VolumeObjectBase& object )
{
    BaseClass::operator=( object );
    m_volume_type = object.volumeType();
    m_has_min_max_values = object.hasMinMaxValues();
    m_min_value = object.minValue();
    m_max_value = object.maxValue();
    m_label = object.label();
    m_veclen = object.veclen();
    m_coords = object.coords();
    m_values = object.values();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys from the specified volume object.
 *  @param  object [in] pointer to the volume object
 */
/*===========================================================================*/
void VolumeObjectBase::deepCopy( const VolumeObjectBase& object )
{
    BaseClass::operator=( object );
    m_volume_type = object.volumeType();
    m_has_min_max_values = object.hasMinMaxValues();
    m_min_value = object.minValue();
    m_max_value = object.maxValue();
    m_label = object.label();
    m_veclen = object.veclen();
    m_coords = object.coords().clone();
    m_values = object.values().clone();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the volume object base.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void VolumeObjectBase::print( std::ostream& os, const kvs::Indent& indent ) const
{
    BaseClass::print( os, indent );
    const std::ios_base::fmtflags flags( os.flags() );
    os << indent << "Veclen : " << this->veclen() << std::endl;
    os.setf( std::ios::boolalpha );
    os << indent << "Set of min/max value : " << this->hasMinMaxValues() << std::endl;
    os.unsetf( std::ios::boolalpha );
    os << indent << "Min. value : " << this->minValue() << std::endl;
    os << indent << "Max. value : " << this->maxValue() << std::endl;
    os.flags( flags );
}

std::ostream& operator << ( std::ostream& os, const kvs::VolumeObjectBase& object )
{
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::ObjectBase&>( object ) << std::endl;
    static_cast<const kvs::ObjectBase&>( object ).print( os );
#endif

    const std::ios_base::fmtflags flags( os.flags() );
    os << "Veclen:  " << object.veclen() << std::endl;
    os.setf( std::ios::boolalpha );
    os << "Set of min/max value:  " << object.hasMinMaxValues() << std::endl;
    os.unsetf( std::ios::boolalpha );
    os << "Min value:  " << object.minValue() << std::endl;
    os << "Max value:  " << object.maxValue();
    os.flags( flags );

    return os;
}

} // end of namespace kvs
