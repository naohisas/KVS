/*****************************************************************************/
/**
 *  @file   PointObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointObject.cpp 1759 2014-05-05 06:14:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "PointObject.h"
#include <cstring>
#include <kvs/KVSMLPointObject>
#include <kvs/LineObject>
#include <kvs/PolygonObject>
#include <kvs/Assert>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLPointObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLPointObject::ExternalAscii; }
        else { return kvs::KVSMLPointObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLPointObject::ExternalBinary;
    }
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PointObject class.
 */
/*===========================================================================*/
PointObject::PointObject()
{
    BaseClass::setGeometryType( Point );
    this->setSize( 1 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PointObject class.
 *  @param  line [in] line object
 */
/*===========================================================================*/
PointObject::PointObject( const kvs::LineObject& line )
{
    BaseClass::setGeometryType( Point );
    BaseClass::setCoords( line.coords() );

    if ( line.colorType() == kvs::LineObject::VertexColor )
    {
        BaseClass::setColors( line.colors() );
    }
    else
    {
        BaseClass::setColor( line.color() );
    }

    this->setSize( line.size() );

    BaseClass::setMinMaxObjectCoords(
        line.minObjectCoord(),
        line.maxObjectCoord() );

    BaseClass::setMinMaxExternalCoords(
        line.minExternalCoord(),
        line.maxExternalCoord() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PointObject class.
 *  @param  polygon [in] polygon object
 */
/*===========================================================================*/
PointObject::PointObject( const kvs::PolygonObject& polygon )
{
    BaseClass::setGeometryType( Point );
    BaseClass::setCoords( polygon.coords() );

    if ( polygon.colorType() == kvs::PolygonObject::VertexColor )
    {
        BaseClass::setColors( polygon.colors() );
    }
    else
    {
        BaseClass::setColor( polygon.color() );
    }

    if ( polygon.normalType() == kvs::PolygonObject::VertexNormal )
    {
        BaseClass::setNormals( polygon.normals() );
    }

    this->setSize( 1.0f );

    BaseClass::setMinMaxObjectCoords(
        polygon.minObjectCoord(),
        polygon.maxObjectCoord() );

    BaseClass::setMinMaxExternalCoords(
        polygon.minExternalCoord(),
        polygon.maxExternalCoord() );
}

/*===========================================================================*/
/**
 *  @brief  Add the point object.
 *  @param  other [in] point object
 */
/*===========================================================================*/
void PointObject::add( const PointObject& other )
{
    if ( this->coords().size() == 0 )
    {
        // Copy the object.
        BaseClass::setCoords( other.coords() );
        BaseClass::setNormals( other.normals() );
        BaseClass::setColors( other.colors() );
        this->setSizes( other.sizes() );

        BaseClass::setMinMaxObjectCoords(
            other.minObjectCoord(),
            other.maxObjectCoord() );
        BaseClass::setMinMaxExternalCoords(
            other.minExternalCoord(),
            other.maxExternalCoord() );
    }
    else
    {
        if ( !BaseClass::hasMinMaxObjectCoords() )
        {
            BaseClass::updateMinMaxCoords();
        }

        kvs::Vec3 min_object_coord( BaseClass::minObjectCoord() );
        kvs::Vec3 max_object_coord( BaseClass::maxObjectCoord() );

        min_object_coord.x() = kvs::Math::Min( min_object_coord.x(), other.minObjectCoord().x() );
        min_object_coord.y() = kvs::Math::Min( min_object_coord.y(), other.minObjectCoord().y() );
        min_object_coord.z() = kvs::Math::Min( min_object_coord.z(), other.minObjectCoord().z() );

        max_object_coord.x() = kvs::Math::Max( max_object_coord.x(), other.maxObjectCoord().x() );
        max_object_coord.y() = kvs::Math::Max( max_object_coord.y(), other.maxObjectCoord().y() );
        max_object_coord.z() = kvs::Math::Max( max_object_coord.z(), other.maxObjectCoord().z() );

        BaseClass::setMinMaxObjectCoords( min_object_coord, max_object_coord );
        BaseClass::setMinMaxExternalCoords( min_object_coord, max_object_coord );

        // Integrate the coordinate values.
        kvs::ValueArray<kvs::Real32> coords;
        const size_t ncoords = this->coords().size() + other.coords().size();
        coords.allocate( ncoords );
        kvs::Real32* pcoords = coords.data();

        // x,y,z, ... + x,y,z, ... = x,y,z, ... ,x,y,z, ...
        memcpy( pcoords, this->coords().data(), this->coords().byteSize() );
        memcpy( pcoords + this->coords().size(), other.coords().data(), other.coords().byteSize() );
        BaseClass::setCoords( coords );

        // Integrate the normal vectors.
        kvs::ValueArray<kvs::Real32> normals;
        if ( this->normals().size() > 0 )
        {
            if ( other.normals().size() > 0 )
            {
                // nx,ny,nz, ... + nx,ny,nz, ... = nx,ny,nz, ... ,nx,ny,nz, ...
                const size_t nnormals = this->normals().size() + other.normals().size();
                normals.allocate( nnormals );
                kvs::Real32* pnormals = normals.data();
                memcpy( pnormals, this->normals().data(), this->normals().byteSize() );
                memcpy( pnormals + this->normals().size(), other.normals().data(), other.normals().byteSize() );
            }
            else
            {
                // nx,ny,nz, ... + (none) = nx,ny,nz, ... ,0,0,0, ...
                const size_t nnormals = this->normals().size() + other.coords().size();
                normals.allocate( nnormals );
                kvs::Real32* pnormals = normals.data();
                memcpy( pnormals, this->normals().data(), this->normals().byteSize() );
                memset( pnormals + this->normals().size(), 0, other.coords().byteSize() );
            }
        }
        else
        {
            if ( other.normals().size() > 0 )
            {
                const size_t nnormals = this->coords().size() + other.normals().size();
                normals.allocate( nnormals );
                kvs::Real32* pnormals = normals.data();
                // (none) + nx,ny,nz, ... = 0,0,0, ... ,nz,ny,nz, ...
                memset( pnormals, 0, this->coords().byteSize() );
                memcpy( pnormals + this->coords().size(), other.normals().data(), other.normals().byteSize() );
            }
        }
        BaseClass::setNormals( normals );

        // Integrate the color values.
        kvs::ValueArray<kvs::UInt8> colors;
        if ( this->colors().size() > 1 )
        {
            if ( other.colors().size() > 1 )
            {
                // r,g,b, ... + r,g,b, ... = r,g,b, ... ,r,g,b, ...
                const size_t ncolors = this->colors().size() + other.colors().size();
                colors.allocate( ncolors );
                kvs::UInt8* pcolors = colors.data();
                memcpy( pcolors, this->colors().data(), this->colors().byteSize() );
                memcpy( pcolors + this->colors().size(), other.colors().data(), other.colors().byteSize() );
            }
            else
            {
                // r,g,b, ... + R,G,B = r,g,b, ... ,R,G,B, ... ,R,G,B
                const size_t ncolors = this->colors().size() + other.coords().size();
                colors.allocate( ncolors );
                kvs::UInt8* pcolors = colors.data();
                memcpy( pcolors, this->colors().data(), this->colors().byteSize() );
                pcolors += this->colors().size();
                const kvs::RGBColor color = other.color();
                for ( size_t i = 0; i < other.coords().size(); i += 3 )
                {
                    *(pcolors++) = color.r();
                    *(pcolors++) = color.g();
                    *(pcolors++) = color.b();
                }
            }
        }
        else
        {
            if ( other.colors().size() > 1 )
            {
                // R,G,B + r,g,b, ... = R,G,B, ... ,R,G,B, r,g,b, ...
                const size_t ncolors = this->coords().size() + other.colors().size();
                colors.allocate( ncolors );
                kvs::UInt8* pcolors = colors.data();
                const kvs::RGBColor color = this->color();
                for ( size_t i = 0; i < this->coords().size(); i += 3 )
                {
                    *(pcolors++) = color.r();
                    *(pcolors++) = color.g();
                    *(pcolors++) = color.b();
                }
                memcpy( pcolors, other.colors().data(), other.colors().byteSize() );
            }
            else
            {
                const kvs::RGBColor color1 = this->color();
                const kvs::RGBColor color2 = other.color();
                if ( color1 == color2 )
                {
                    // R,G,B + R,G,B = R,G,B
                    const size_t ncolors = 3;
                    colors.allocate( ncolors );
                    kvs::UInt8* pcolors = colors.data();
                    *(pcolors++) = color1.r();
                    *(pcolors++) = color1.g();
                    *(pcolors++) = color1.b();
                }
                else
                {
                    // R,G,B + R,G,B = R,G,B, ... ,R,G,B, ...
                    const size_t ncolors = this->coords().size() + other.coords().size();
                    colors.allocate( ncolors );
                    kvs::UInt8* pcolors = colors.data();
                    for ( size_t i = 0; i < this->coords().size(); i += 3 )
                    {
                        *(pcolors++) = color1.r();
                        *(pcolors++) = color1.g();
                        *(pcolors++) = color1.b();
                    }
                    for ( size_t i = 0; i < other.coords().size(); i += 3 )
                    {
                        *(pcolors++) = color2.r();
                        *(pcolors++) = color2.g();
                        *(pcolors++) = color2.b();
                    }
                }
            }
        }
        BaseClass::setColors( colors );

        // Integrate the size values.
        kvs::ValueArray<kvs::Real32> sizes;
        if ( this->sizes().size() > 1 )
        {
            if ( other.sizes().size() > 1 )
            {
                // s, ... + s, ... = s, ... ,s, ...
                const size_t nsizes = this->sizes().size() + other.sizes().size();
                sizes.allocate( nsizes );
                kvs::Real32* psizes = sizes.data();
                memcpy( psizes, this->sizes().data(), this->sizes().byteSize() );
                memcpy( psizes + this->sizes().size(), other.sizes().data(), other.sizes().byteSize() );
            }
            else
            {
                // s, ... + S = s, ... ,S, ... ,S
                const size_t nsizes = this->sizes().size() + other.coords().size();
                sizes.allocate( nsizes );
                kvs::Real32* psizes = sizes.data();
                memcpy( psizes, this->sizes().data(), this->sizes().byteSize() );
                psizes += this->colors().size();
                const kvs::Real32 size = other.size();
                for ( size_t i = 0; i < other.coords().size(); i++ )
                {
                    *(psizes++) = size;
                }
            }
        }
        else
        {
            if ( other.sizes().size() > 1 )
            {
                // S + s, ... = S, ... ,S, s, ...
                const size_t nsizes = this->coords().size() + other.sizes().size();
                sizes.allocate( nsizes );
                kvs::Real32* psizes = sizes.data();
                const kvs::Real32 size = this->size();
                for ( size_t i = 0; i < this->coords().size(); i++ )
                {
                    *(psizes++) = size;
                }
                memcpy( psizes, other.sizes().data(), other.sizes().byteSize() );
            }
            else
            {
                const kvs::Real32 size1 = this->size();
                const kvs::Real32 size2 = other.size();
                if ( size1 == size2 )
                {
                    // S + S = S
                    const size_t nsizes = 1;
                    sizes.allocate( nsizes );
                    kvs::Real32* psizes = sizes.data();
                    *(psizes++) = size1;
                }
                else
                {
                    // S + S = S, ... , S, ...
                    const size_t nsizes = this->coords().size() + other.coords().size();
                    sizes.allocate( nsizes );
                    kvs::Real32* psizes = sizes.data();
                    for ( size_t i = 0; i < this->coords().size(); i++ )
                    {
                        *(psizes++) = size1;
                    }
                    for ( size_t i = 0; i < other.coords().size(); i++ )
                    {
                        *(psizes++) = size2;
                    }
                }
            }
        }
        this->setSizes( sizes );
    }
}

/*===========================================================================*/
/**
 *  @brief  Shallow copy the point object.
 *  @param  other [in] point object
 */
/*===========================================================================*/
void PointObject::shallowCopy( const PointObject& other )
{
    BaseClass::shallowCopy( other );
    m_sizes = other.sizes();
}

/*===========================================================================*/
/**
 *  @brief  Deep copy the point object.
 *  @param  other [in] point object
 */
/*===========================================================================*/
void PointObject::deepCopy( const PointObject& other )
{
    BaseClass::deepCopy( other );
    m_sizes = other.sizes().clone();
}

/*===========================================================================*/
/**
 *  @brief  Clear the point object.
 */
/*===========================================================================*/
void PointObject::clear()
{
    BaseClass::clear();
    m_sizes.release();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the point object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void PointObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object type : " << "point object" << std::endl;
    BaseClass::print( os, indent );
    os << indent << "Number of sizes : " << this->numberOfSizes() << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a point object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool PointObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLPointObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not a point object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLPointObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    this->setCoords( kvsml.coords() );
    this->setColors( kvsml.colors() );
    this->setNormals( kvsml.normals() );
    this->setSizes( kvsml.sizes() );

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

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the point object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool PointObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLPointObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );
    kvsml.setCoords( this->coords() );
    kvsml.setColors( this->colors() );
    kvsml.setNormals( this->normals() );
    kvsml.setSizes( this->sizes() );

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

/*===========================================================================*/
/**
 *  @brief  Sets a size value.
 *  @param  size [in] size value
 */
/*===========================================================================*/
void PointObject::setSize( const kvs::Real32 size )
{
    m_sizes.allocate( 1 );
    m_sizes[0] = size;
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const PointObject& object )
{
    os << "Object type:  " << "point object" << std::endl;
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::GeometryObjectBase&>( object ) << std::endl;
    static_cast<const kvs::GeometryObjectBase&>( object ).print( os );
#endif
    os << "Number of sizes:  " << object.numberOfSizes();

    return os;
}

} // end of namespace kvs
