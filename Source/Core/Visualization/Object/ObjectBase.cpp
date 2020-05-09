/*****************************************************************************/
/**
 *  @file   ObjectBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectBase.cpp 1801 2014-08-05 14:45:34Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ObjectBase.h"
#include <iomanip>
#include <kvs/Camera>
#include <kvs/Math>
#include <kvs/OpenGL>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ObjectBase class.
 */
/*===========================================================================*/
ObjectBase::ObjectBase():
    kvs::XformControl(),
    m_object_type( UnknownObject ),
    m_name( "unknown" ),
    m_min_object_coord( kvs::Vec3::Constant( -3.0 ) ),
    m_max_object_coord( kvs::Vec3::Constant(  3.0 ) ),
    m_min_external_coord( kvs::Vec3::Constant( -3.0 ) ),
    m_max_external_coord( kvs::Vec3::Constant(  3.0 ) ),
    m_has_min_max_object_coords( false ),
    m_has_min_max_external_coords( false ),
    m_show_flag( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ObjectBase class.
 */
/*===========================================================================*/
ObjectBase::~ObjectBase()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator.
 */
/*===========================================================================*/
ObjectBase& ObjectBase::operator = ( const ObjectBase& object )
{
    kvs::XformControl::operator =( object );
    m_object_type = object.m_object_type;
    m_name = object.m_name;
    m_min_object_coord = object.m_min_object_coord;
    m_max_object_coord = object.m_max_object_coord;
    m_min_external_coord = object.m_min_external_coord;
    m_max_external_coord = object.m_max_external_coord;
    m_has_min_max_object_coords = object.m_has_min_max_object_coords;
    m_has_min_max_external_coords = object.m_has_min_max_external_coords;
    m_object_center = object.m_object_center;
    m_external_center = object.m_external_center;
    m_normalize = object.m_normalize;
    m_show_flag = object.m_show_flag;

    return *this;
}

/*===========================================================================*/
/**
 *  @breif  Sets the min/max object coordinates.
 *  @param  min_coord [in] min. object coordinate value
 *  @param  max_coord [in] max. object coordinate value
 */
/*===========================================================================*/
void ObjectBase::setMinMaxObjectCoords(
    const kvs::Vec3& min_coord,
    const kvs::Vec3& max_coord )
{
    m_min_object_coord = min_coord;
    m_max_object_coord = max_coord;
    m_has_min_max_object_coords = true;
    this->updateNormalizeParameters();
}

/*===========================================================================*/
/**
 *  @breif  Sets the min/max external coordinates.
 *  @param  min_coord [in] min. external coordinate value
 *  @param  max_coord [in] max. external coordinate value
 */
/*===========================================================================*/
void ObjectBase::setMinMaxExternalCoords(
    const kvs::Vec3& min_coord,
    const kvs::Vec3& max_coord )
{
    m_min_external_coord = min_coord;
    m_max_external_coord = max_coord;
    m_has_min_max_external_coords = true;
    this->updateNormalizeParameters();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the object base.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void ObjectBase::print( std::ostream& os, const kvs::Indent& indent ) const
{
    const std::ios_base::fmtflags flags( os.flags() );
    os << indent << "Name : " << this->name() << std::endl;
    os.setf( std::ios::boolalpha );
    os << indent << "Set of min/max object coord : " << this->hasMinMaxObjectCoords() << std::endl;
    os << indent << "Set of min/max external coord : " << this->hasMinMaxObjectCoords() << std::endl;
    os.unsetf( std::ios::boolalpha );
    os << indent << "Min object coord : " << this->minObjectCoord() << std::endl;
    os << indent << "Max object coord : " << this->maxObjectCoord() << std::endl;
    os << indent << "Min external coord : " << this->minExternalCoord() << std::endl;
    os << indent << "Max external coord : " << this->maxExternalCoord() << std::endl;
    os << indent << "Object center : " << this->objectCenter() << std::endl;
    os << indent << "External center : " << this->externalCenter() << std::endl;
    os << indent << "Normalize parameter : " << this->normalize() << std::endl;
    os.flags( flags );
}

/*===========================================================================*/
/**
 *  @brief  Read an object data from the specified file.
 *  @param  filename [in] input filename
 *  @return false because the function has not implemented
 */
/*===========================================================================*/
bool ObjectBase::read( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Write the object data to the specified file.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return false because the function has not implemented
 */
/*===========================================================================*/
bool ObjectBase::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::IgnoreUnusedVariable( filename );
    kvs::IgnoreUnusedVariable( ascii );
    kvs::IgnoreUnusedVariable( external );
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Updates the normalize parameters.
 */
/*===========================================================================*/
void ObjectBase::updateNormalizeParameters()
{
    kvs::Vec3 diff_obj = m_max_object_coord - m_min_object_coord;
    kvs::Vec3 diff_ext = m_max_external_coord - m_min_external_coord;

    m_object_center = ( m_max_object_coord + m_min_object_coord ) * 0.5f;
    m_external_center = ( m_max_external_coord + m_min_external_coord ) * 0.5f;

    if ( kvs::Math::Equal( diff_obj.x(), 0.0f ) &&
         kvs::Math::Equal( diff_obj.y(), 0.0f ) &&
         kvs::Math::Equal( diff_obj.z(), 0.0f ) )
    {
        // In the case of 'diff_obj == (0,0,0)', such as a point object composed of
        // a single vertex point, the normalization parameters is set to '(1,1,1)'.
        m_normalize = kvs::Vec3( 1.0f, 1.0f, 1.0f );
    }
    else
    {
        m_normalize.x() =
            ( !kvs::Math::Equal( diff_obj.x(), 0.0f ) ) ? diff_ext.x() / diff_obj.x() :
            ( kvs::Math::Equal( diff_obj.y(), 0.0f ) ) ? diff_ext.z() / diff_obj.z() :
            ( kvs::Math::Equal( diff_obj.z(), 0.0f ) ) ? diff_ext.y() / diff_obj.y() :
            kvs::Math::Max( diff_ext.y() / diff_obj.y(), diff_ext.z() / diff_obj.z() );

        m_normalize.y() =
            ( !kvs::Math::Equal( diff_obj.y(), 0.0f ) ) ? diff_ext.y() / diff_obj.y() :
            ( kvs::Math::Equal( diff_obj.x(), 0.0f ) ) ? diff_ext.z() / diff_obj.z() :
            ( kvs::Math::Equal( diff_obj.z(), 0.0f ) ) ? diff_ext.x() / diff_obj.x() :
            kvs::Math::Max( diff_ext.x() / diff_obj.x(), diff_ext.z() / diff_obj.z() );

        m_normalize.z() =
            ( !kvs::Math::Equal( diff_obj.z(), 0.0f ) ) ? diff_ext.z() / diff_obj.z() :
            ( kvs::Math::Equal( diff_obj.x(), 0.0f ) ) ? diff_ext.y() / diff_obj.y() :
            ( kvs::Math::Equal( diff_obj.y(), 0.0f ) ) ? diff_ext.x() / diff_obj.x() :
            kvs::Math::Max( diff_ext.x() / diff_obj.x(), diff_ext.y() / diff_obj.y() );
    }
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const ObjectBase& object )
{
    const std::ios_base::fmtflags flags( os.flags() );
    os << "Name: " << object.name() << std::endl;
    os.setf( std::ios::boolalpha );
    os << "Set of min/max object coord:  " << object.hasMinMaxObjectCoords() << std::endl;
    os << "Set of min/max external coord:  " << object.hasMinMaxObjectCoords() << std::endl;
    os.unsetf( std::ios::boolalpha );
    os << "Min object coord:  " << object.minObjectCoord() << std::endl;
    os << "Max object coord:  " << object.maxObjectCoord() << std::endl;
    os << "Min external coord:  " << object.minExternalCoord() << std::endl;
    os << "Max external coord:  " << object.maxExternalCoord() << std::endl;
    os << "Object center:  " << object.objectCenter() << std::endl;
    os << "External center:  " << object.externalCenter() << std::endl;
    os << "Normalize parameter:  " << object.normalize();
    os.flags( flags );

    return os;
}

} // end of namespace kvs
