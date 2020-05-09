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
#include <kvs/KVSMLStructuredVolumeObject>


namespace
{

/*==========================================================================*/
/**
 *  @brief  Converts to the grid type from the given string.
 *  @param  grid_type [in] grid type string
 *  @return grid type
 */
/*==========================================================================*/
kvs::StructuredVolumeObject::GridType GetGridType( const std::string& grid_type )
{
    if (      grid_type == "uniform"     ) { return kvs::StructuredVolumeObject::Uniform;     }
    else if ( grid_type == "rectilinear" ) { return kvs::StructuredVolumeObject::Rectilinear; }
    else if ( grid_type == "curvilinear" ) { return kvs::StructuredVolumeObject::Curvilinear; }
    else
    {
        kvsMessageError( "Unknown grid type '%s'.", grid_type.c_str() );
        return kvs::StructuredVolumeObject::UnknownGridType;
    }
}

const std::string GetGridTypeName( const kvs::StructuredVolumeObject::GridType type )
{
    switch( type )
    {
    case kvs::StructuredVolumeObject::Uniform: return "uniform";
    case kvs::StructuredVolumeObject::Rectilinear: return "rectiliear";
    case kvs::StructuredVolumeObject::Curvilinear: return "curvilinear";
    default: return "unknown";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLStructuredVolumeObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLStructuredVolumeObject::ExternalAscii; }
        else { return kvs::KVSMLStructuredVolumeObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLStructuredVolumeObject::ExternalBinary;
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

/*===========================================================================*/
/**
 *  @brief  Read a structured volume object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool StructuredVolumeObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLStructuredVolumeObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not a structured volume object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLStructuredVolumeObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    this->setGridType( ::GetGridType( kvsml.gridType() ) );
    this->setResolution( kvsml.resolution() );
    this->setVeclen( kvsml.veclen() );
    this->setValues( kvsml.values() );

    if ( this->gridType() == kvs::StructuredVolumeObject::Rectilinear ||
         this->gridType() == kvs::StructuredVolumeObject::Curvilinear )
    {
        this->setCoords( kvsml.coords() );
    }

    if ( kvsml.hasExternalCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minExternalCoord() );
        const kvs::Vec3 max_coord( kvsml.maxExternalCoord() );
        this->setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( kvsml.hasObjectCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minObjectCoord() );
        const kvs::Vec3 max_coord( kvsml.maxObjectCoord() );
        this->setMinMaxObjectCoords( min_coord, max_coord );
    }
    else
    {
        this->updateMinMaxCoords();
    }

    if ( kvsml.hasLabel() ) { this->setLabel( kvsml.label() ); }
    if ( kvsml.hasUnit() ) { this->setUnit( kvsml.unit() ); }

    if ( kvsml.hasMinValue() && kvsml.hasMaxValue() )
    {
        const double min_value = kvsml.minValue();
        const double max_value = kvsml.maxValue();
        this->setMinMaxValues( min_value, max_value );
    }
    else
    {
        this->updateMinMaxValues();
        const double min_value = kvsml.hasMinValue() ? kvsml.minValue() : this->minValue();
        const double max_value = kvsml.hasMaxValue() ? kvsml.maxValue() : this->maxValue();
        this->setMinMaxValues( min_value, max_value );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the structured volume object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool StructuredVolumeObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLStructuredVolumeObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );

    if ( this->label() != "" ) { kvsml.setLabel( this->label() ); }
    if ( this->unit() != "" ) { kvsml.setUnit( this->unit() ); }

    switch ( this->gridType() )
    {
    case kvs::StructuredVolumeObject::Uniform:
    {
        kvsml.setGridType("uniform");
        break;
    }
    case kvs::StructuredVolumeObject::Rectilinear:
    {
        kvsml.setGridType("rectilinear");
        kvsml.setCoords( this->coords() );
        break;
    }
    case kvs::StructuredVolumeObject::Curvilinear:
    {
        kvsml.setGridType("curvilinear");
        kvsml.setCoords( this->coords() );
        break;
    }
    case kvs::StructuredVolumeObject::UnknownGridType:
    {
        kvsMessageError( "Unknown grid type." );
        break;
    }
    default:
    {
        kvsMessageError( "Not supported grid type." );
        break;
    }
    }

    kvsml.setVeclen( this->veclen() );
    kvsml.setResolution( this->resolution() );
    kvsml.setValues( this->values() );

    if ( this->hasMinMaxValues() )
    {
        kvsml.setMinValue( this->minValue() );
        kvsml.setMaxValue( this->maxValue() );
    }

    if ( this->hasMinMaxObjectCoords() )
    {
        kvsml.setMinMaxObjectCoords( this->minObjectCoord(), this->maxObjectCoord() );
    }

    if ( this->hasMinMaxExternalCoords() )
    {
        kvsml.setMinMaxExternalCoords( this->minExternalCoord(), this->maxExternalCoord() );
    }

    return kvsml.write( filename );
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
