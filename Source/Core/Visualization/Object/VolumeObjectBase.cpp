/****************************************************************************/
/**
 *  @file   VolumeObjectBase.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "VolumeObjectBase.h"


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
