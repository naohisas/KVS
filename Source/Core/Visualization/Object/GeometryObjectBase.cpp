/*****************************************************************************/
/**
 *  @file   GeometryObjectBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GeometryObjectBase.cpp 1759 2014-05-05 06:14:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GeometryObjectBase.h"
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GeometryObjectBase class.
 */
/*===========================================================================*/
GeometryObjectBase::GeometryObjectBase():
    m_geometry_type( UnknownGeometryType )
{
    BaseClass::setObjectType( Geometry );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copy.
 *  @param  object [in] object
 */
/*===========================================================================*/
void GeometryObjectBase::shallowCopy( const GeometryObjectBase& object )
{
    BaseClass::operator=( object );
    m_geometry_type = object.geometryType();
    m_coords = object.coords();
    m_colors = object.colors();
    m_normals = object.normals();
}

/*===========================================================================*/
/**
 *  @brief  Deep copy.
 *  @param  object [in] object
 */
/*===========================================================================*/
void GeometryObjectBase::deepCopy( const GeometryObjectBase& object )
{
    BaseClass::operator=( object );
    m_geometry_type = object.geometryType();
    m_coords = object.coords().clone();
    m_colors = object.colors().clone();
    m_normals = object.normals().clone();
}

/*===========================================================================*/
/**
 *  @brief  Clears the object.
 */
/*===========================================================================*/
void GeometryObjectBase::clear()
{
    m_coords.release();
    m_colors.release();
    m_normals.release();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the geometry object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void GeometryObjectBase::print( std::ostream& os, const kvs::Indent& indent ) const
{
    ObjectBase::print( os, indent );
    os << indent << "Number of vertices : " << this->numberOfVertices() << std::endl;
    os << indent << "Number of colors : " << this->numberOfColors() << std::endl;
    os << indent << "Number of normal vectors : " << this->numberOfNormals() << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Sets a color value.
 *  @param  color [in] color value
 */
/*===========================================================================*/
void GeometryObjectBase::setColor( const kvs::RGBColor& color )
{
    m_colors.allocate( 3 );
    m_colors[0] = color.r();
    m_colors[1] = color.g();
    m_colors[2] = color.b();
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of the vertices.
 *  @return number of the vertices
 */
/*===========================================================================*/
size_t GeometryObjectBase::numberOfVertices() const
{
    const size_t dimension = 3;
    return m_coords.size() / dimension;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of the colors.
 *  @return number of the colors
 */
/*===========================================================================*/
size_t GeometryObjectBase::numberOfColors() const
{
    const size_t nchannels = 3;
    return m_colors.size() / nchannels;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of the normal vectors.
 *  @return number of the normal vectors
 */
/*===========================================================================*/
size_t GeometryObjectBase::numberOfNormals() const
{
    const size_t dimension = 3;
    return m_normals.size() / dimension;
}

/*===========================================================================*/
/**
 *  @brief  Returns the coordinate value.
 *  @param  index [in] index of the coordinate value
 *  @return coordinate value
 */
/*===========================================================================*/
const kvs::Vec3 GeometryObjectBase::coord( const size_t index ) const
{
    const size_t dimension = 3;
    return kvs::Vec3( m_coords.data() + dimension * index );
}

/*===========================================================================*/
/**
 *  @brief  Returns the color value.
 *  @param  index [in] index of the color value
 *  @return color value
 */
/*===========================================================================*/
const kvs::RGBColor GeometryObjectBase::color( const size_t index ) const
{
    const size_t nchannels = 3;
    return kvs::RGBColor( m_colors.data() + nchannels * index );
}

/*===========================================================================*/
/**
 *  @brief  Returns the normal vector.
 *  @param  index [in] index of the normal vector
 *  @return normal vector
 */
/*===========================================================================*/
const kvs::Vec3 GeometryObjectBase::normal( const size_t index ) const
{
    const size_t dimension = 3;
    return kvs::Vec3( m_normals.data() + dimension * index );
}

/*==========================================================================*/
/**
 *  @brief  Updates the min/max coordinates.
 */
/*==========================================================================*/
void GeometryObjectBase::updateMinMaxCoords()
{
    this->calculate_min_max_coords();
}

/*==========================================================================*/
/**
 *  @brief  Calculates the min/max coordinate values.
 */
/*==========================================================================*/
void GeometryObjectBase::calculate_min_max_coords()
{
    if ( this->coords().empty() ) return;

    KVS_ASSERT( this->coords().size() % 3 == 0 );

    const kvs::Real32* coord = this->coords().data();
    const kvs::Real32* const end = coord + this->coords().size();

    kvs::Real32 x = *( coord++ );
    kvs::Real32 y = *( coord++ );
    kvs::Real32 z = *( coord++ );

    kvs::Vec3 min_coord( x, y, z );
    kvs::Vec3 max_coord( x, y, z );

    while ( coord < end )
    {
        x = *( coord++ );
        y = *( coord++ );
        z = *( coord++ );

        min_coord.x() = kvs::Math::Min( min_coord.x(), x );
        min_coord.y() = kvs::Math::Min( min_coord.y(), y );
        min_coord.z() = kvs::Math::Min( min_coord.z(), z );

        max_coord.x() = kvs::Math::Max( max_coord.x(), x );
        max_coord.y() = kvs::Math::Max( max_coord.y(), y );
        max_coord.z() = kvs::Math::Max( max_coord.z(), z );
    }

    this->setMinMaxObjectCoords( min_coord, max_coord );

    if ( !( this->hasMinMaxExternalCoords() ) )
    {
        this->setMinMaxExternalCoords( this->minObjectCoord(), this->maxObjectCoord() );
    }
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const kvs::GeometryObjectBase& object )
{
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::ObjectBase&>( object ) << std::endl;
    static_cast<const kvs::ObjectBase&>( object ).print( os );
#endif
    os << "Number of vertices:  " << object.numberOfVertices() << std::endl;
    os << "Number of colors:  " << object.numberOfColors() << std::endl;
    os << "Number of normal vectors:  " << object.numberOfNormals();

    return os;
}

} // end of namespace kvs
