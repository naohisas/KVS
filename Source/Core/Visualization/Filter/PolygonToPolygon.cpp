/*****************************************************************************/
/**
 *  @file   PolygonToPolygon.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "PolygonToPolygon.h"
#include <map>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonToPolygon class.
 */
/*===========================================================================*/
PolygonToPolygon::PolygonToPolygon():
    kvs::FilterBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonToPolygon class.
 *  @param  object [in] pointer to a polygon data
 */
/*===========================================================================*/
PolygonToPolygon::PolygonToPolygon( const kvs::PolygonObject* object ):
    kvs::FilterBase()
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the PolygonToPolygon class.
 */
/*===========================================================================*/
PolygonToPolygon::~PolygonToPolygon()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes this class.
 *  @param  object [in] pointer to the object
 *  @return pointer to the converted object
 */
/*===========================================================================*/
PolygonToPolygon::SuperClass* PolygonToPolygon::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
      BaseClass::setSuccess( false );
        kvsMessageError( "Input object is NULL." );
        return NULL;
    }

    const kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );
    if ( !polygon )
    {
        BaseClass::setSuccess( false );
        kvsMessageError( "Input object is not supported." );
        return NULL;
    }

    if ( polygon->polygonType() == kvs::PolygonObject::Triangle )
    {
        this->calculate_triangle_connections( polygon );
        this->calculate_triangle_normals();
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError( "Input polygon type is not supported." );
        return NULL;
    }

    SuperClass::updateMinMaxCoords();
    BaseClass::setSuccess( true );

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Calculate triangle polygon connections and reduce vertex.
 *  @param  object [in] pointer to the polygon data
 */
/*===========================================================================*/
void PolygonToPolygon::calculate_triangle_connections( const kvs::PolygonObject* object )
{
    const size_t nvertices = object->numberOfVertices();
    const size_t npolygons = object->numberOfConnections();
    const size_t ncolors = object->numberOfColors();
    if ( npolygons > 0 && nvertices != 3 * npolygons )
    {
        SuperClass::setCoords( object->coords() );
        SuperClass::setConnections( object->connections() );
        SuperClass::setOpacity( 255 );
        SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
        SuperClass::setColorType( kvs::PolygonObject::VertexColor );
        if ( ncolors == nvertices ) SuperClass::setColors( object->colors() );
        else SuperClass::setColor( object->color() );
        return;
    }

    std::multimap<float, unsigned int> indices;
    std::vector<float> coords;
    std::vector<unsigned int> connections;
    std::vector<unsigned char> colors;
    const float* p_coords = object->coords().pointer();
    const unsigned char* p_colors = object->colors().pointer();
    const bool has_colors = ncolors > 1;

    for ( size_t i = 0; i < nvertices / 3; i++ )
    {
        kvs::Vec3 vertex[3];
        kvs::RGBColor color[3];
        bool has_vertex[3];
        size_t index[3];
        for ( size_t j = 0; j < 3; j++ )
        {
            vertex[j] = kvs::Vec3( p_coords + i * 9 + j * 3 );
            has_vertex[j] = false;
            index[j] = 0;
            if ( has_colors )
            {
                color[j] = kvs::RGBColor( p_colors + i * 9 + j * 3 );
            }
        }

        for ( size_t j = 0; j < 3; j++ )
        {
            const float key = vertex[j].x() + vertex[j].y() + vertex[j].z();
            const size_t nresults = indices.count( key );
            if ( nresults > 0 )
            {
                std::multimap<float, unsigned int>::iterator itr = indices.find( key );
                for ( size_t k = 0; k < nresults; k++ )
                {
                    const size_t result = itr->second;
                    kvs::Vector3f v( coords[ result * 3 ], coords[ result * 3 + 1 ], coords[ result * 3 + 2 ] );
                    if ( vertex[j].x() == v.x() && vertex[j].y() == v.y() && vertex[j].z() == v.z() )
                    {
                        index[j] = result;
                        has_vertex[j] = true;
                    }
                    itr++;
                }
            }
        }

        for ( size_t j = 0; j < 3; j++ )
        {
            if ( !has_vertex[j] )
            {
                connections.push_back( coords.size() / 3 );
                indices.insert( std::multimap<float, unsigned int>::value_type(
                    vertex[j].x() + vertex[j].y() + vertex[j].z(), coords.size() / 3 ) );
                coords.push_back( vertex[j].x() );
                coords.push_back( vertex[j].y() );
                coords.push_back( vertex[j].z() );
                if ( has_colors )
                {
                    colors.push_back( color[j].r() );
                    colors.push_back( color[j].g() );
                    colors.push_back( color[j].b() );
                }
            }
            else
            {
                connections.push_back( index[j] );
            }
        }
    }

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    if ( has_colors ) SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    else SuperClass::setColor( object->color() );
}

/*===========================================================================*/
/**
 *  @brief  Calculate triangle polygon normals.
 */
/*===========================================================================*/
void PolygonToPolygon::calculate_triangle_normals()
{
    const size_t nvertices = SuperClass::numberOfVertices();
    const size_t npolygons = SuperClass::numberOfConnections();
    const float* p_coords = SuperClass::coords().pointer();
    const unsigned int* p_connections = SuperClass::connections().pointer();

    kvs::ValueArray<float> normals( 3 * nvertices );
    normals.fill( 0x00 );

    for ( size_t i = 0; i < npolygons; i++ )
    {
        size_t index[3];
        kvs::Vec3 vertex[3];
        for ( size_t j = 0; j < 3; j++ )
        {
            index[j] = p_connections[ 3 * i + j ];
            vertex[j] = kvs::Vec3( p_coords + 3 * index[j] );
        }

        const kvs::Vector3f normal( ( vertex[1] - vertex[0] ).cross( vertex[2] - vertex[0] ) );
        for ( size_t j = 0; j < 3; j++ )
        {
            normals[ 3 * index[j]     ] += normal.x();
            normals[ 3 * index[j] + 1 ] += normal.y();
            normals[ 3 * index[j] + 2 ] += normal.z();
        }
    }

    // Normalize normals.
    const float* p_normals = normals.pointer();
    for ( size_t i = 0; i < nvertices; i++ )
    {
        kvs::Vec3 normal( p_normals + i * 3 );
        normal.normalize();
        normals[ i * 3     ] = normal.x();
        normals[ i * 3 + 1 ] = normal.y();
        normals[ i * 3 + 2 ] = normal.z();
    }

    SuperClass::setNormals( normals );
    SuperClass::setNormalType( kvs::PolygonObject::VertexNormal );
}

} // end of namespace kvs
