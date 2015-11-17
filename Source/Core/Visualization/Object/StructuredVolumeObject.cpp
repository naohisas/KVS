/****************************************************************************/
/**
 *  @file   StructuredVolumeObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeObject.cpp 1762 2014-05-07 04:29:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "StructuredVolumeObject.h"


namespace
{

const std::string GetGridTypeName( const kvs::StructuredVolumeObject::GridType type )
{
    switch( type )
    {
    case kvs::StructuredVolumeObject::Uniform: return "uniform";
    case kvs::StructuredVolumeObject::Rectilinear: return "rectiliear";
    case kvs::StructuredVolumeObject::Curvilinear: return "curvilinear";
    default: return "unknown grid type";
    }
}

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new empty StructuredVolumeObject.
 */
/*==========================================================================*/
StructuredVolumeObject::StructuredVolumeObject():
    kvs::VolumeObjectBase(),
    m_grid_type( UnknownGridType ),
    m_resolution( kvs::Vec3ui( 0, 0, 0 ) )
{
    BaseClass::setVolumeType( Structured );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  object [in] structured volume object
 */
/*===========================================================================*/
void StructuredVolumeObject::shallowCopy( const StructuredVolumeObject& object )
{
    BaseClass::shallowCopy( object );
    this->m_grid_type = object.gridType();
    this->m_resolution = object.resolution();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  object [in] structured volume object
 */
/*===========================================================================*/
void StructuredVolumeObject::deepCopy( const StructuredVolumeObject& object )
{
    BaseClass::deepCopy( object );
    this->m_grid_type = object.gridType();
    this->m_resolution = object.resolution();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the structured volume object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void StructuredVolumeObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    if ( !this->hasMinMaxValues() ) this->updateMinMaxValues();
    os << indent << "Object type : " << "structured volume object" << std::endl;
    BaseClass::print( os, indent );
    os << indent << "Grid type : " << ::GetGridTypeName( this->gridType() ) << std::endl;
    os << indent << "Resolution : " << this->resolution() << std::endl;
    os << indent << "Number of nodes : " << this->numberOfNodes() << std::endl;
    os << indent << "Min. value : " << this->minValue() << std::endl;
    os << indent << "Max. value : " << this->maxValue() << std::endl;
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of nodes per line.
 */
/*==========================================================================*/
size_t StructuredVolumeObject::numberOfNodesPerLine() const
{
    return m_resolution.x();
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of nodes per slice.
 */
/*==========================================================================*/
size_t StructuredVolumeObject::numberOfNodesPerSlice() const
{
    return this->numberOfNodesPerLine() * m_resolution.y();
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of nodes.
 */
/*==========================================================================*/
size_t StructuredVolumeObject::numberOfNodes() const
{
    return this->numberOfNodesPerSlice() * m_resolution.z();
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of cells.
 *  @return number of cells
 */
/*===========================================================================*/
size_t StructuredVolumeObject::numberOfCells() const
{
    return ( m_resolution.x() - 1 ) * ( m_resolution.y() - 1 ) * ( m_resolution.z() - 1 );
}

/*==========================================================================*/
/**
 *  @brief  Update the min/max node coordinates.
 */
/*==========================================================================*/
void StructuredVolumeObject::updateMinMaxCoords()
{
    this->calculate_min_max_coords();
}

/*==========================================================================*/
/**
 *  @brief  Calculate the min/max coordinate values.
 */
/*==========================================================================*/
void StructuredVolumeObject::calculate_min_max_coords()
{
    kvs::Vec3 min_coord( 0.0f, 0.0f, 0.0f );
    kvs::Vec3 max_coord( 0.0f, 0.0f, 0.0f );

    switch ( m_grid_type )
    {
    case Uniform:
    {
        min_coord.set( 0.0f, 0.0f, 0.0f );
        max_coord.set(
            static_cast<float>( m_resolution.x() ) - 1.0f,
            static_cast<float>( m_resolution.y() ) - 1.0f,
            static_cast<float>( m_resolution.z() ) - 1.0f );

        break;
    }
    case Rectilinear:
    {
        const float* const coord = this->coords().data();

        min_coord.set(
            *( coord ),
            *( coord + m_resolution.x() ),
            *( coord + m_resolution.x() + m_resolution.y() ) );

        max_coord.set(
            *( coord + m_resolution.x() - 1 ),
            *( coord + m_resolution.x() + m_resolution.y() - 1 ),
            *( coord + m_resolution.x() + m_resolution.y() + m_resolution.z() - 1 ) );

        break;
    }
    case Curvilinear:
    {
        const float*       coord = this->coords().data();
        const float* const end   = coord + this->coords().size();

        float x = *( coord++ );
        float y = *( coord++ );
        float z = *( coord++ );

        min_coord.set( x, y, z );
        max_coord.set( x, y, z );

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

        break;
    }
    default:
    {
        break;
    }
    }

    this->setMinMaxObjectCoords( min_coord, max_coord );

    if ( !( this->hasMinMaxExternalCoords() ) )
    {
        this->setMinMaxExternalCoords(
            this->minObjectCoord(),
            this->maxObjectCoord() );
    }
}

std::ostream& operator << ( std::ostream& os, const StructuredVolumeObject& object )
{
    if ( !object.hasMinMaxValues() ) object.updateMinMaxValues();

    os << "Object type:  " << "structured volume object" << std::endl;
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::VolumeObjectBase&>( object ) << std::endl;
    static_cast<const kvs::VolumeObjectBase&>( object ).print( os );
#endif
    os << "Grid type:  " << ::GetGridTypeName( object.gridType() ) << std::endl;
    os << "Resolution:  " << object.resolution() << std::endl;
    os << "Number of nodes:  " << object.numberOfNodes() << std::endl;
    os << "Min. value:  " << object.minValue() << std::endl;
    os << "Max. value:  " << object.maxValue();

    return os;
}

} // end of namespace kvs
