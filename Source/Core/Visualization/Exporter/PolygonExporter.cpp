/*****************************************************************************/
/**
 *  @file   PolygonExporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonExporter.cpp 1722 2014-03-13 01:45:06Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "PolygonExporter.h"
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonExporter class for KVSMLPolygonObject format.
 *  @param  object [in] pointer to the input polygon object
 */
/*===========================================================================*/
PolygonExporter<kvs::KVSMLPolygonObject>::PolygonExporter( const kvs::PolygonObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the KVSMLPolygonObject format
 */
/*===========================================================================*/
kvs::KVSMLPolygonObject* PolygonExporter<kvs::KVSMLPolygonObject>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );
    if ( !polygon )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not polygon object.");
        return NULL;
    }

    switch ( polygon->polygonType() )
    {
    case kvs::PolygonObject::Triangle: this->setPolygonType( "triangle" ); break;
    case kvs::PolygonObject::Quadrangle: this->setPolygonType( "quadrangle" ); break;
    default: break;
    }

    switch ( polygon->colorType() )
    {
    case kvs::PolygonObject::VertexColor: this->setColorType( "vertex" ); break;
    case kvs::PolygonObject::PolygonColor: this->setColorType( "polygon" ); break;
    default: break;
    }

    switch ( polygon->normalType() )
    {
    case kvs::PolygonObject::VertexNormal: this->setNormalType( "vertex" ); break;
    case kvs::PolygonObject::PolygonNormal: this->setNormalType( "polygon" ); break;
    default: break;
    }

    this->setCoords( polygon->coords() );
    this->setColors( polygon->colors() );
    this->setConnections( polygon->connections() );
    this->setNormals( polygon->normals() );
    this->setOpacities( polygon->opacities() );

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonExporter class for STL format.
 *  @param  object [in] pointer to the input polygon object
 */
/*===========================================================================*/
PolygonExporter<kvs::Stl>::PolygonExporter( const kvs::PolygonObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the STL format
 */
/*===========================================================================*/
kvs::Stl* PolygonExporter<kvs::Stl>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );
    if ( !polygon )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not polygon object.");
        return NULL;
    }

    if ( polygon->polygonType() != kvs::PolygonObject::Triangle )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not triangle polygon.");
        return NULL;
    }

    if ( polygon->numberOfConnections() == 0 )
    {
        this->setCoords( polygon->coords() );
    }
    else
    {
        const size_t npolygons = polygon->connections().size() / 3;
        const kvs::UInt32* pconnections = polygon->connections().data();
        const kvs::Real32* pcoords = polygon->coords().data();
        kvs::ValueArray<kvs::Real32> coords( npolygons * 9 );
        for ( size_t i = 0; i < npolygons; i++ )
        {
            const kvs::UInt32 index0 = *(pconnections++);
            const kvs::UInt32 index1 = *(pconnections++);
            const kvs::UInt32 index2 = *(pconnections++);

            const kvs::Vec3 coord0( pcoords + 3 * index0 );
            const kvs::Vec3 coord1( pcoords + 3 * index1 );
            const kvs::Vec3 coord2( pcoords + 3 * index2 );

            coords[ 9 * i + 0 ] = coord0.x();
            coords[ 9 * i + 1 ] = coord0.y();
            coords[ 9 * i + 2 ] = coord0.z();

            coords[ 9 * i + 3 ] = coord1.x();
            coords[ 9 * i + 4 ] = coord1.y();
            coords[ 9 * i + 5 ] = coord1.z();

            coords[ 9 * i + 6 ] = coord2.x();
            coords[ 9 * i + 7 ] = coord2.y();
            coords[ 9 * i + 8 ] = coord2.z();
        }
        this->setCoords( coords );
    }

    if ( polygon->normalType() == kvs::PolygonObject::VertexNormal )
    {
        // Convert to kvs::PolygonObject::PolygonNormal type.
        const size_t npolygons = polygon->connections().size() / 3;
        const kvs::UInt32* pconnections = polygon->connections().data();
        const kvs::Real32* pnormals = polygon->normals().data();
        kvs::ValueArray<kvs::Real32> normals( npolygons * 3 );
        for ( size_t i = 0; i < npolygons; i++ )
        {
            const kvs::UInt32 index0 = *(pconnections++);
            const kvs::UInt32 index1 = *(pconnections++);
            const kvs::UInt32 index2 = *(pconnections++);

            const kvs::Vec3 normal0( pnormals + 3 * index0 );
            const kvs::Vec3 normal1( pnormals + 3 * index1 );
            const kvs::Vec3 normal2( pnormals + 3 * index2 );
            const kvs::Vec3 normal = ( normal0 + normal1 + normal2 ) / 3.0f;

            normals[ 3 * i + 0 ] = normal.x();
            normals[ 3 * i + 1 ] = normal.y();
            normals[ 3 * i + 2 ] = normal.z();
        }
        this->setNormals( normals );
    }
    else if ( polygon->normalType() == kvs::PolygonObject::PolygonNormal )
    {
        this->setNormals( polygon->normals() );
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonExporter class for PLY format.
 *  @param  object [in] pointer to the input polygon object
 */
/*===========================================================================*/
PolygonExporter<kvs::Ply>::PolygonExporter( const kvs::PolygonObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the PLY format
 */
/*===========================================================================*/
kvs::Ply* PolygonExporter<kvs::Ply>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );
    if ( !polygon )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not polygon object.");
        return NULL;
    }

    if ( polygon->polygonType() != kvs::PolygonObject::Triangle )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not triangle polygon.");
        return NULL;
    }

    this->setCoords( polygon->coords() );

    const size_t nvertices = polygon->coords().size() / 3;

    if ( polygon->colors().size() == 3 )
    {
        kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );

        const kvs::RGBColor color = polygon->color();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            colors[ 3 * i + 0 ] = color.r();
            colors[ 3 * i + 1 ] = color.g();
            colors[ 3 * i + 2 ] = color.b();
        }
        this->setColors( colors );
    }
    else if ( polygon->colors().size() > 3 )
    {
        if ( polygon->colorType() == kvs::PolygonObject::PolygonColor )
        {
            kvs::ValueArray<kvs::UInt32> temp( nvertices * 3 ); temp.fill( 0 );
            kvs::ValueArray<kvs::UInt32> counter( nvertices ); counter.fill( 0 );
            if ( polygon->numberOfConnections() == 0 )
            {
                const size_t npolygons = polygon->coords().size() / 3;
                for ( size_t i = 0; i < npolygons; i++ )
                {
                    const kvs::UInt32 index0 = 3 * i + 0;
                    const kvs::UInt32 index1 = 3 * i + 1;
                    const kvs::UInt32 index2 = 3 * i + 2;

                    const kvs::RGBColor color = polygon->color(i);
                    temp[ 3 * index0 + 0 ] += color.r();
                    temp[ 3 * index0 + 1 ] += color.g();
                    temp[ 3 * index0 + 2 ] += color.b();
                    counter[ index0 ] += 1;

                    temp[ 3 * index1 + 0 ] += color.r();
                    temp[ 3 * index1 + 1 ] += color.g();
                    temp[ 3 * index1 + 2 ] += color.b();
                    counter[ index1 ] += 1;

                    temp[ 3 * index2 + 0 ] += color.r();
                    temp[ 3 * index2 + 1 ] += color.g();
                    temp[ 3 * index2 + 2 ] += color.b();
                    counter[ index2 ] += 1;
                }

                kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
                for ( size_t i = 0; i < nvertices; i++ )
                {
                    if ( counter[i] == 0 ) continue;
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                }

                this->setColors( colors );
            }
            else // polygon->nconnections() > 0
            {
                const size_t npolygons = polygon->connections().size() / 3;
                const kvs::UInt32* pconnections = polygon->connections().data();
                for ( size_t i = 0; i < npolygons; i++ )
                {
                    const kvs::UInt32 index0 = *(pconnections++);
                    const kvs::UInt32 index1 = *(pconnections++);
                    const kvs::UInt32 index2 = *(pconnections++);

                    const kvs::RGBColor color = polygon->color(i);
                    temp[ 3 * index0 + 0 ] += color.r();
                    temp[ 3 * index0 + 1 ] += color.g();
                    temp[ 3 * index0 + 2 ] += color.b();
                    counter[ index0 ] += 1;

                    temp[ 3 * index1 + 0 ] += color.r();
                    temp[ 3 * index1 + 1 ] += color.g();
                    temp[ 3 * index1 + 2 ] += color.b();
                    counter[ index1 ] += 1;

                    temp[ 3 * index2 + 0 ] += color.r();
                    temp[ 3 * index2 + 1 ] += color.g();
                    temp[ 3 * index2 + 2 ] += color.b();
                    counter[ index2 ] += 1;
                }

                kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
                for ( size_t i = 0; i < nvertices; i++ )
                {
                    if ( counter[i] == 0 ) continue;
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                    colors[ 3 * i + 0 ] = kvs::UInt8( temp[ 3 * i + 0 ] / counter[i] );
                }

                this->setColors( colors );
            }
        }
        else // kvs::PolygonObject::VertexColor
        {
            this->setColors( polygon->colors() );
        }
    }

    if ( polygon->normals().size() > 0 )
    {
        if ( polygon->normalType() == kvs::PolygonObject::PolygonNormal )
        {
            kvs::ValueArray<kvs::Real32> normals( nvertices * 3 ); normals.fill( 0 );
            kvs::ValueArray<kvs::UInt32> counter( nvertices ); counter.fill( 0 );

            const size_t npolygons = polygon->connections().size() / 3;
            const kvs::UInt32* pconnections = polygon->connections().data();
            for ( size_t i = 0; i < npolygons; i++ )
            {
                const kvs::UInt32 index0 = *(pconnections++);
                const kvs::UInt32 index1 = *(pconnections++);
                const kvs::UInt32 index2 = *(pconnections++);

                const kvs::Vector3f normal = polygon->normal(i);
                normals[ 3 * index0 + 0 ] += normal.x();
                normals[ 3 * index0 + 1 ] += normal.y();
                normals[ 3 * index0 + 2 ] += normal.z();
                counter[ index0 ] += 1;

                normals[ 3 * index1 + 0 ] += normal.x();
                normals[ 3 * index1 + 1 ] += normal.y();
                normals[ 3 * index1 + 2 ] += normal.z();
                counter[ index1 ] += 1;

                normals[ 3 * index2 + 0 ] += normal.x();
                normals[ 3 * index2 + 1 ] += normal.y();
                normals[ 3 * index2 + 2 ] += normal.z();
                counter[ index2 ] += 1;
            }

            for ( size_t i = 0; i < nvertices; i++ )
            {
                if ( counter[i] == 0 ) continue;
                normals[ 3 * i + 0 ] /= static_cast<kvs::Real32>( counter[i] );
                normals[ 3 * i + 1 ] /= static_cast<kvs::Real32>( counter[i] );
                normals[ 3 * i + 2 ] /= static_cast<kvs::Real32>( counter[i] );
            }

            this->setNormals( normals );
        }
        else if ( polygon->normalType() == kvs::PolygonObject::VertexNormal )
        {
            this->setNormals( polygon->normals() );
        }
    }

    if ( polygon->connections().size() > 0 )
    {
        this->setConnections( polygon->connections() );
    }

    return this;
}

} // end of namespace kvs
