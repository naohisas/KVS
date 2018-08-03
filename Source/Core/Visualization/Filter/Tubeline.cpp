/*****************************************************************************/
/**
 *  @file   Tubeline.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tubeline.cpp 1420 2013-02-21 14:15:30Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "Tubeline.h"
#include <kvs/Quaternion>
#include <kvs/Math>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a vertex array of the line object.
 *  @param  line [in] pointer to the line object
 *  @return vertex array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> GetVertexArray( const kvs::LineObject* line )
{
    if ( line->lineType() == kvs::LineObject::Strip )
    {
        return line->coords();
    }
    else if ( line->lineType() == kvs::LineObject::Uniline )
    {
        const kvs::Real32* v = line->coords().data();
        const kvs::UInt32* c = line->connections().data();

        const size_t dimension = 3;
        const size_t nvertices = line->numberOfConnections();
        kvs::ValueArray<kvs::Real32> vertices( nvertices * dimension );
        for ( size_t i = 0, index = 0; i < nvertices; i++, index += dimension )
        {
            const kvs::UInt32 id = c[i];
            vertices[ index + 0 ] = v[ id * dimension + 0 ];
            vertices[ index + 1 ] = v[ id * dimension + 1 ];
            vertices[ index + 2 ] = v[ id * dimension + 2 ];
        }

        return vertices;
    }
    else
    {
        const kvs::ValueArray<kvs::Real32> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a vertex array of the line object.
 *  @param  line [in] line object
 *  @param  id1 [in] ID of the start vertex
 *  @param  id2 [in] ID of the end vertex
 *  @return vertex array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> GetVertexArray(
    const kvs::LineObject* line,
    const size_t id1,
    const size_t id2 )
{
    if ( line->lineType() == kvs::LineObject::Polyline )
    {
        const kvs::Real32* v = line->coords().data();

        const size_t dimension = 3;
        const size_t nvertices = id2 - id1 + 1;
        kvs::ValueArray<kvs::Real32> vertices( nvertices * dimension );
        for ( size_t i = 0, index = 0; i < nvertices; i++, index += dimension )
        {
            const size_t id = id1 + i;
            vertices[ index + 0 ] = v[ id * dimension + 0 ];
            vertices[ index + 1 ] = v[ id * dimension + 1 ];
            vertices[ index + 2 ] = v[ id * dimension + 2 ];
        }

        return vertices;
    }
    else if ( line->lineType() == kvs::LineObject::Segment )
    {
        const kvs::Real32* v = line->coords().data();

        const size_t dimension = 3;
        const size_t nvertices = 2;
        kvs::ValueArray<kvs::Real32> vertices( nvertices * dimension );

        vertices[0] = v[ id1 * dimension + 0 ];
        vertices[1] = v[ id1 * dimension + 1 ];
        vertices[2] = v[ id1 * dimension + 2 ];

        vertices[3] = v[ id2 * dimension + 0 ];
        vertices[4] = v[ id2 * dimension + 1 ];
        vertices[5] = v[ id2 * dimension + 2 ];

        return vertices;
    }
    else
    {
        const kvs::ValueArray<kvs::Real32> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a color array of the line object.
 *  @param  line [in] pointer to the line object
 *  @return color array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt8> GetColorArray( const kvs::LineObject* line )
{
    if ( line->numberOfColors() == 1 )
    {
        const kvs::UInt8* c = line->colors().data();

        const size_t ncomponents = 3;
        const size_t ncolors = line->numberOfVertices();
        kvs::ValueArray<kvs::UInt8> colors( ncolors * ncomponents );
        for ( size_t i = 0, index = 0; i < ncolors; i++, index += ncomponents )
        {
            colors[ index + 0 ] = c[0];
            colors[ index + 1 ] = c[1];
            colors[ index + 2 ] = c[2];
        }

        return colors;
    }
    else if ( line->numberOfColors() > 1 )
    {
        return line->colors();
    }
    else
    {
        const kvs::ValueArray<kvs::UInt8> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a color array of the line object.
 *  @param  line [in] pointer to the line object
 *  @param  id1 [in] ID of the start vertex
 *  @param  id2 [in] ID of the end vertex
 *  @param  con_id [in]
 *  @return color array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt8> GetColorArray(
    const kvs::LineObject* line,
    const size_t id1,
    const size_t id2,
    const size_t con_id )
{
    // Ponter to the color array of the line object.
    const kvs::UInt8* c = line->colors().data();

    // Number of components and number of colors.
    const size_t ncomponents = 3;
    const size_t ncolors = id2 - id1 + 1;

    if ( line->numberOfColors() == 1 )
    {
        kvs::ValueArray<kvs::UInt8> colors( ncomponents * ncolors );
        for ( size_t i = 0, index = 0; i < ncolors; i++, index += ncomponents )
        {
            colors[ index + 0 ] = c[0];
            colors[ index + 1 ] = c[1];
            colors[ index + 2 ] = c[2];
        }

        return colors;
    }
    else if ( line->numberOfColors() > 1 )
    {
        if ( line->colorType() == kvs::LineObject::LineColor )
        {
            kvs::ValueArray<kvs::UInt8> colors( ncomponents * ncolors );

            const kvs::UInt32* n = line->connections().data();

            size_t counter = 0;
            for ( size_t i = 0, index = 0; i < con_id; i++, index += 2 )
            {
                const size_t index_0 = n[ index + 0 ];
                const size_t index_1 = n[ index + 1 ];
                counter += index_1 - index_0;
            }
            for ( size_t i = 0, index = 0; i < ncolors - 1; i++ , index += ncomponents )
            {
                const size_t j = ( i + counter ) * ncomponents;
                colors[ index + 0 ] = c[ j + 0 ];
                colors[ index + 1 ] = c[ j + 1 ];
                colors[ index + 2 ] = c[ j + 2 ];
            }
            const size_t i1 = ( ncolors - 1 ) * ncomponents;
            const size_t i2 = ( ncolors - 2 + counter ) * ncomponents;
            colors[ i1 + 0 ] = c[ i2 + 0 ];
            colors[ i1 + 1 ] = c[ i2 + 1 ];
            colors[ i1 + 2 ] = c[ i2 + 2 ];

            return colors;
        }
        else // kvs::LineObject::VertexColor
        {
            kvs::ValueArray<kvs::UInt8> colors( ncomponents * ncolors );
            for ( size_t i = 0, index = 0; i < ncolors; i++, index += ncomponents )
            {
                const size_t id = id1 + i;
                colors[ index + 0 ] = c[ id * ncomponents + 0 ];
                colors[ index + 1 ] = c[ id * ncomponents + 1 ];
                colors[ index + 2 ] = c[ id * ncomponents + 2 ];
            }

            return colors;
        }
    }
    else
    {
        const kvs::ValueArray<kvs::UInt8> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a size array of the line object.
 *  @param  line [in] pointer of the line object
 *  @return size array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> GetSizeArray( const kvs::LineObject* line )
{
    if ( line->numberOfSizes() == 1 )
    {
        const kvs::Real32* s = line->sizes().data();

        const size_t nsizes = line->numberOfVertices() - 1;
        kvs::ValueArray<kvs::Real32> sizes( nsizes );
        for ( size_t i = 0; i < nsizes; i++ )
        {
            sizes[i] = s[0];
        }

        return sizes;
    }
    else if ( line->numberOfSizes() > 1 )
    {
        return line->sizes();
    }
    else
    {
        const kvs::ValueArray<kvs::Real32> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a size array of the line object.
 *  @param  line [in] pointer to the line object
 *  @param  id1 [in] ID of the start vertex
 *  @param  id2 [in] ID of the end vertex
 *  @param  con_id [in]
 *  @return size array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> GetSizeArray(
    const kvs::LineObject* line,
    const size_t id1,
    const size_t id2,
    const size_t con_id )
{
    // Ponter to the size array of the line object.
    const kvs::Real32* s = line->sizes().data();

    // Number of components of the size array.
    const size_t nsizes = id2 - id1 + 1;

    if ( line->numberOfSizes() == 1 )
    {
        kvs::ValueArray<kvs::Real32> sizes( nsizes );
        for ( size_t i = 0; i < nsizes; i++ )
        {
            sizes[i] = s[0];
        }

        return sizes;
    }
    else if ( line->numberOfSizes() > 1 )
    {
        kvs::ValueArray<kvs::Real32> sizes( nsizes );

        const kvs::UInt32* n = line->connections().data();

        size_t counter = 0;
        for ( size_t i = 0, index = 0; i < con_id; i++, index += 2 )
        {
            const size_t index_0 = n[ index + 0 ];
            const size_t index_1 = n[ index + 1 ];
            counter += index_1 - index_0;
        }
        for ( size_t i = 0; i < nsizes; i++ )
        {
            sizes[i] = s[ i + counter ];
        }

        return sizes;
    }
    else
    {
        const kvs::ValueArray<kvs::Real32> empty;
        return empty;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a color array of the line object.
 *  @param  line [in] pointer to the line object
 *  @return color array
 */
/*===========================================================================*/
kvs::PolygonObject::ColorType GetColorType( const kvs::LineObject* line )
{
    kvs::PolygonObject::ColorType type = kvs::PolygonObject::VertexColor;
    switch ( line->colorType() )
    {
    case kvs::LineObject::VertexColor:
        type = kvs::PolygonObject::VertexColor;
        break;
    case kvs::LineObject::LineColor:
        type = kvs::PolygonObject::PolygonColor;
        break;
    default:
        break;
    }

    return type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of the vertices of the polygon object.
 *  @param  line [in] pointer to the line object
 *  @return number of the vertices
 */
/*===========================================================================*/
size_t GetNumberOfVertices( const kvs::LineObject* line )
{
    size_t nvertices = 0;
    switch ( line->lineType() )
    {
    case kvs::LineObject::Strip:
        nvertices = line->numberOfVertices();
        break;
    case kvs::LineObject::Uniline:
        nvertices = line->numberOfConnections();
        break;
    default:
        break;
    }

    return nvertices;
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new tubeline class.
 */
/*===========================================================================*/
Tubeline::Tubeline( void ):
    kvs::FilterBase(),
    m_ndivisions( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new tubeline class and executes mapping.
 *  @param  line [in] pointer to the line object
 *  @param  ndivisions [in] number of divisions
 */
/*===========================================================================*/
Tubeline::Tubeline(
    const kvs::LineObject* line,
    const size_t ndivisions ):
    m_ndivisions( ndivisions )
{
    this->exec( line );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the tubeline class.
 */
/*===========================================================================*/
Tubeline::~Tubeline( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Set a number of divisions.
 *  @param  ndivisions [in] number of divisions
 */
/*===========================================================================*/
void Tubeline::setNumberOfDivisions( const size_t ndivisions )
{
    m_ndivisions = ndivisions;
}

/*===========================================================================*/
/**
 *  @brief  Execites the filter process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the filtered object
 */
/*===========================================================================*/
Tubeline::SuperClass* Tubeline::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::LineObject* line = kvs::LineObject::DownCast( object );
    if ( !line )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not supported.");
        return NULL;
    }

    // Set the min/max coordinates.
    SuperClass::setMinMaxObjectCoords( line->minObjectCoord(), line->maxObjectCoord() );
    SuperClass::setMinMaxExternalCoords( line->minExternalCoord(), line->maxExternalCoord() );

    const kvs::LineObject::LineType type = line->lineType();
    if ( type == kvs::LineObject::Strip )
    {
        this->filtering_strip( line );
    }
    else if ( type == kvs::LineObject::Uniline )
    {
        this->filtering_uniline( line );
    }
    else if ( type == kvs::LineObject::Polyline )
    {
        this->filtering_polyline( line );
    }
    else if ( type == kvs::LineObject::Segment )
    {
        this->filtering_segment( line );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unknown line type.");
        return NULL;
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Creates tube's polygon for the strip type of the line object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void Tubeline::filtering_strip( const kvs::LineObject* line )
{
    const kvs::ValueArray<kvs::Real32> line_vertices = ::GetVertexArray( line );
    const kvs::ValueArray<kvs::Real32> line_sizes = ::GetSizeArray( line );
    const kvs::ValueArray<kvs::UInt8> line_colors = ::GetColorArray( line );
    const size_t nvertices = ::GetNumberOfVertices( line );
    const kvs::PolygonObject::ColorType color_type = ::GetColorType( line );

    std::vector<kvs::Real32> vertices;
    std::vector<kvs::UInt8> colors;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::Real32> normals;
    this->calculate_tubes(
        &vertices,
        &colors,
        &connections,
        &normals,
        line_vertices,
        line_sizes,
        line_colors,
        nvertices,
        color_type );

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertices ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setColorType( color_type );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*===========================================================================*/
/**
 *  @brief  Creates tube's polygon for the uniline type of the line object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void Tubeline::filtering_uniline( const kvs::LineObject* line )
{
    const kvs::ValueArray<kvs::Real32> line_vertices = ::GetVertexArray( line );
    const kvs::ValueArray<kvs::Real32> line_sizes = ::GetSizeArray( line );
    const kvs::ValueArray<kvs::UInt8> line_colors = ::GetColorArray( line );
    const size_t nvertices = ::GetNumberOfVertices( line );
    const kvs::PolygonObject::ColorType color_type = ::GetColorType( line );

    std::vector<kvs::Real32> vertices;
    std::vector<kvs::UInt8> colors;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::Real32> normals;
    this->calculate_tubes(
        &vertices,
        &colors,
        &connections,
        &normals,
        line_vertices,
        line_sizes,
        line_colors,
        nvertices,
        color_type );

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertices ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setColorType( color_type );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*===========================================================================*/
/**
 *  @brief  Creates tube's polygon for the polyline type of the line object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void Tubeline::filtering_polyline( const kvs::LineObject* line )
{
    std::vector<kvs::Real32> vertices;
    std::vector<kvs::UInt8> colors;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::Real32> normals;

    const kvs::PolygonObject::ColorType color_type = ::GetColorType( line );

    const kvs::UInt32* line_connections = line->connections().data();
    const size_t line_nconnections = line->numberOfConnections();
    for( size_t i = 0, index = 0; i < line_nconnections; i++, index += 2 )
    {
        const size_t id1 = line_connections[ index + 0 ];
        const size_t id2 = line_connections[ index + 1 ];

        const kvs::ValueArray<kvs::Real32> line_vertices = ::GetVertexArray( line, id1, id2 );
        const kvs::ValueArray<kvs::Real32> line_sizes = ::GetSizeArray( line, id1, id2, i );
        const kvs::ValueArray<kvs::UInt8> line_colors = ::GetColorArray( line, id1, id2, i );
        const size_t nvertices = id2 - id1 + 1;

        if ( line_sizes.size() < 2 ) continue;
        if ( line_colors.size() < 2 ) continue;

        std::vector<kvs::Real32> tmp_vertices;
        std::vector<kvs::UInt8> tmp_colors;
        std::vector<kvs::UInt32> tmp_connections;
        std::vector<kvs::Real32> tmp_normals;
        this->calculate_tubes(
            &tmp_vertices,
            &tmp_colors,
            &tmp_connections,
            &tmp_normals,
            line_vertices,
            line_sizes,
            line_colors,
            nvertices,
            color_type );

        const kvs::UInt32 nv = static_cast<kvs::UInt32>( vertices.size() / 3 );

        vertices.insert( vertices.end(), tmp_vertices.begin(), tmp_vertices.end() );
        normals.insert( normals.end(), tmp_normals.begin(), tmp_normals.end() );

        const size_t tmp_ncolors = ( color_type == kvs::PolygonObject::VertexColor ) ?
            tmp_colors.size() :
            tmp_colors.size() - m_ndivisions * 3;
        for( size_t j = 0; j < tmp_ncolors; j++ )
        {
            colors.push_back( tmp_colors[j] );
        }

        const size_t tmp_connections_size = tmp_connections.size();
        for ( size_t j = 0; j < tmp_connections_size; j++ )
        {
            connections.push_back( tmp_connections[j] + nv );
        }
    }

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertices ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setColorType( color_type );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*===========================================================================*/
/**
 *  @brief  Creates tube's polygon for the segment type of the line object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void Tubeline::filtering_segment( const kvs::LineObject* line )
{
    std::vector<kvs::Real32> vertices;
    std::vector<kvs::UInt8> colors;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::Real32> normals;

    const kvs::PolygonObject::ColorType color_type = ::GetColorType( line );

    const kvs::UInt32* line_connections = line->connections().data();
    const size_t line_nconnections = line->numberOfConnections();
    for( size_t i = 0, index = 0; i < line_nconnections; i++, index += 2 )
    {
        const size_t id1 = line_connections[ index + 0 ];
        const size_t id2 = line_connections[ index + 1 ];

        const size_t nvertices = 2;
        const kvs::ValueArray<kvs::Real32> line_vertices = ::GetVertexArray( line, id1, id2 );
//        const kvs::Real32 line_size = ( line->nsizes() == 1 ) ? line->size(0) : line->size(i);
        kvs::ValueArray<kvs::Real32> line_sizes(1);
        if ( line->numberOfSizes() == 1 ) line_sizes[0] = line->size(0);
        else line_sizes[0] = line->size(i);

        kvs::ValueArray<kvs::UInt8> line_colors(6);
        if ( line->numberOfColors() == 1 )
        {
            line_colors[0] = line->color(0).r();
            line_colors[1] = line->color(0).g();
            line_colors[2] = line->color(0).b();
            line_colors[3] = line->color(0).r();
            line_colors[4] = line->color(0).g();
            line_colors[5] = line->color(0).b();
        }
        else
        {
            if( color_type == kvs::PolygonObject::VertexColor )
            {
                line_colors[0] = line->color(id1).r();
                line_colors[1] = line->color(id1).g();
                line_colors[2] = line->color(id1).b();
                line_colors[3] = line->color(id2).r();
                line_colors[4] = line->color(id2).g();
                line_colors[5] = line->color(id2).b();
            }
            else
            {
                line_colors[0] = line->color(i).r();
                line_colors[1] = line->color(i).g();
                line_colors[2] = line->color(i).b();
                line_colors[3] = line->color(i).r();
                line_colors[4] = line->color(i).g();
                line_colors[5] = line->color(i).b();
            }
        }

        std::vector<kvs::Real32> tmp_vertices;
        std::vector<kvs::UInt8> tmp_colors;
        std::vector<kvs::UInt32> tmp_connections;
        std::vector<kvs::Real32> tmp_normals;
        this->calculate_tubes(
            &tmp_vertices,
            &tmp_colors,
            &tmp_connections,
            &tmp_normals,
            line_vertices,
            line_sizes,
            line_colors,
            nvertices,
            color_type );

        vertices.insert( vertices.end(), tmp_vertices.begin(), tmp_vertices.end() );
        normals.insert( normals.end(), tmp_normals.begin(), tmp_normals.end() );
        colors.insert( colors.end(), tmp_colors.begin(), tmp_colors.end() );

        const kvs::UInt32 nv = static_cast<kvs::UInt32>( vertices.size() / 3 );
        const size_t tmp_connections_size = tmp_connections.size();
        for ( size_t j = 0; j < tmp_connections_size; j++ )
        {
            connections.push_back( tmp_connections[j] + nv );
        }
    }

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( vertices ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setColorType( ::GetColorType( line ) );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*===========================================================================*/
/**
 *  @brief  Calculates tubeline polygon.
 *  @param  vertices [out] pointer to the vertex array of the polygon object
 *  @param  colors [out] pointer to the color array of the polygon object
 *  @param  connections [out] pointer to the connection array of the polygon object
 *  @param  normals [out] pointer to the normal array of the polygon object
 *  @param  line_vertices [in] vertex array of the line object
 *  @param  line_sizes [in] size array of the line object
 *  @param  line_colors [in] color array of the line object
 *  @param  nvertices [in] number of vertices
 *  @param  color_type [in] color type
 */
/*===========================================================================*/
void Tubeline::calculate_tubes(
    std::vector<kvs::Real32>* vertices,
    std::vector<kvs::UInt8>* colors,
    std::vector<kvs::UInt32>* connections,
    std::vector<kvs::Real32>* normals,
    const kvs::ValueArray<kvs::Real32> line_vertices,
    const kvs::ValueArray<kvs::Real32> line_sizes,
    const kvs::ValueArray<kvs::UInt8> line_colors,
    const size_t nvertices,
    const kvs::PolygonObject::ColorType color_type )
{
    // First tube position.
    kvs::Vector3f start_position( line_vertices[0], line_vertices[1], line_vertices[2] );
    kvs::Vector3f end_position( line_vertices[3], line_vertices[4], line_vertices[5] );

    // Initiali raduis parameters.
    kvs::Real32 pre_radius  = 0.0f;
    kvs::Real32 radius = line_sizes[0];
    kvs::Real32 post_radius = line_sizes[1];

    // Calculate vertices that are composed of the circles (cross-sections).
    std::vector<kvs::Vector3f> start_circle;
    std::vector<kvs::Vector3f> end_circle;
    this->calculate_circles(
        &start_circle,
        &end_circle,
        start_position,
        end_position,
        radius,
        pre_radius,
        post_radius );

    // Calculate colors at the cross-sections.
    kvs::RGBColor start_color( line_colors[0], line_colors[1], line_colors[2] );
    kvs::RGBColor end_color( line_colors[3], line_colors[4], line_colors[5] );

    size_t vertex_number = 0;

    this->set_vertices( vertices, start_circle, end_circle );
    this->set_colors( colors, start_color, end_color, color_type );
    this->set_connections_and_normals( connections, normals, start_circle, end_circle, vertex_number );

    const size_t nloops = nvertices - 1;
    for ( size_t i = 1, index = 3; i < nloops; i++, index += 3 )
    {
        // Update the tube position.
        const kvs::Real32 v0_x = line_vertices[ index + 0 ];
        const kvs::Real32 v0_y = line_vertices[ index + 1 ];
        const kvs::Real32 v0_z = line_vertices[ index + 2 ];
        const kvs::Real32 v1_x = line_vertices[ index + 3 ];
        const kvs::Real32 v1_y = line_vertices[ index + 4 ];
        const kvs::Real32 v1_z = line_vertices[ index + 5 ];
        start_position = kvs::Vector3f( v0_x, v0_y, v0_z );
        end_position = kvs::Vector3f( v1_x, v1_y, v1_z );

        // Update the reduis parameters.
        pre_radius = line_sizes[i-1];
        radius = line_sizes[i];
        post_radius = ( i == nvertices - 2 ) ? 0.0f : line_sizes[i+1];

        // Save parameters for the pre-tube.
        const std::vector<kvs::Vector3f> pre_end_circle = end_circle;
        const kvs::RGBColor pre_end_color = end_color;

        // Update the vertices of the tube's circles.
        start_circle.clear();
        end_circle.clear();
        this->calculate_circles(
            &start_circle,
            &end_circle,
            start_position,
            end_position,
            radius,
            pre_radius,
            post_radius );

        // tube + joint
        vertex_number = m_ndivisions * 2 * i + m_ndivisions * 2 * ( i - 1 );

        // Update the colors of the circles.
        const kvs::UInt8 c0_r = line_colors[ index + 0 ];
        const kvs::UInt8 c0_g = line_colors[ index + 1 ];
        const kvs::UInt8 c0_b = line_colors[ index + 2 ];
        const kvs::UInt8 c1_r = line_colors[ index + 3 ];
        const kvs::UInt8 c1_g = line_colors[ index + 4 ];
        const kvs::UInt8 c1_b = line_colors[ index + 5 ];
        start_color = kvs::RGBColor( c0_r, c0_g, c0_b );
        end_color = ( i == nvertices-1 ) ? start_color : kvs::RGBColor( c1_r, c1_g, c1_b );

        // Set the data of the polygon object.
        this->set_vertices( vertices, start_circle, end_circle );
        this->set_colors( colors, start_color, end_color, color_type );
        this->set_connections_and_normals( connections, normals, start_circle, end_circle, vertex_number );

        // Calculate joint tube.
        vertex_number += m_ndivisions * 2;
        this->set_vertices( vertices, pre_end_circle, start_circle );
        this->set_colors( colors, pre_end_color, start_color, color_type );
        this->set_connections_and_normals( connections, normals, pre_end_circle, start_circle, vertex_number );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates vertices on the circles.
 *  @param  start_circle [out] pointer to the vertex array of the start circle
 *  @param  end_circle [out] pointer to the vertex array of the end circle
 *  @param  start_position [in] position of the start vertex
 *  @param  end_position [in] position of the end vertex
 *  @param  radius [in] radius
 *  @param  pre_radius [in] pre-radius
 *  @param  post_radius [in] post-radius
 */
/*===========================================================================*/
void Tubeline::calculate_circles(
    std::vector<kvs::Vector3f>* start_circle,
    std::vector<kvs::Vector3f>* end_circle,
    const kvs::Vector3f& start_position,
    const kvs::Vector3f& end_position,
    const float radius,
    const float pre_radius,
    const float post_radius )
{
    const kvs::Vector3f vec1 = end_position - start_position;
    const float length = static_cast<float>( vec1.length() );

    //static kvs::Vector3f base( 0.0f, 0.0f, 1.0f ); ???
    const kvs::Vector3f base( 0.0f, 0.0f, 1.0f );
    const kvs::Vector3f axis = base.cross( vec1 );
    const float radian = static_cast<float>( std::acos( base.dot( vec1 ) / ( base.length() * vec1.length() ) ) );
    const kvs::Quaternion q( axis, radian );
    const kvs::Matrix33f mat = q.toMatrix();

    float diff_rad = static_cast<float>( 2.0f * kvs::Math::PI() / m_ndivisions );
    float min_z = pre_radius - length * 0.5f;
    float max_z = length * 0.5f - post_radius;
    if ( length - post_radius - pre_radius < 0  )
    {
        min_z = 0.0f;
        max_z = length;
    }

    const kvs::Vector3f pos = start_position + vec1 * 0.5f;
    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        const float rad = diff_rad * i;
        const float x = radius * std::cos( rad );
        const float y = radius * std::sin( rad );

        const kvs::Vector3f s_pos = mat * kvs::Vector3f( x, y, min_z ) + pos;
        start_circle->push_back( s_pos );

        const kvs::Vector3f e_pos = mat * kvs::Vector3f( x, y, max_z ) + pos;
        end_circle->push_back( e_pos );
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a vertex array.
 *  @param  vertices [out] pointer to the vertex array
 *  @param  start_circle [in] vertex array of the start circle
 *  @param  end_circle [in] vertex array of the end circle
 */
/*===========================================================================*/
void Tubeline::set_vertices(
    std::vector<kvs::Real32>* vertices,
    const std::vector<kvs::Vector3f>& start_circle,
    const std::vector<kvs::Vector3f>& end_circle )
{
    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        vertices->push_back( start_circle[i].x() );
        vertices->push_back( start_circle[i].y() );
        vertices->push_back( start_circle[i].z() );
    }

    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        vertices->push_back( end_circle[i].x() );
        vertices->push_back( end_circle[i].y() );
        vertices->push_back( end_circle[i].z() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a color array
 *  @param  colors [out] pointer to the color array
 *  @param  start_color [in] color of the start vertex
 *  @param  end_color [in] color of the end vertex
 *  @param  color_type [in] polygon color type
 */
/*===========================================================================*/
void Tubeline::set_colors(
    std::vector<kvs::UInt8>* colors,
    const kvs::RGBColor& start_color,
    const kvs::RGBColor& end_color,
    const kvs::PolygonObject::ColorType color_type )
{
    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        colors->push_back( start_color.r() );
        colors->push_back( start_color.g() );
        colors->push_back( start_color.b() );
    }

    if( color_type == kvs::PolygonObject::VertexColor )
    {
        for ( size_t i = 0; i < m_ndivisions; i++ )
        {
            colors->push_back( end_color.r() );
            colors->push_back( end_color.g() );
            colors->push_back( end_color.b() );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a connection array and a normal array.
 *  @param  connections [out] pointer to the connection array
 *  @param  normals [out] pointer to the normal array
 *  @param  start_circle [in] vertex array of the start circle
 *  @param  end_circle [in] vertex array of the end circle
 *  @param  vertex_number [in] vertex number
 */
/*===========================================================================*/
void Tubeline::set_connections_and_normals(
    std::vector<kvs::UInt32>* connections,
    std::vector<kvs::Real32>* normals,
    const std::vector<kvs::Vector3f>& start_circle,
    const std::vector<kvs::Vector3f>& end_circle,
    const size_t vertex_number )
{
    // create vertex index list
    std::vector<kvs::UInt32> v_ids;
    std::vector<kvs::UInt32> n_ids;

    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        const kvs::UInt32 index = static_cast<kvs::UInt32>( i );
        v_ids.push_back( vertex_number + index );
        n_ids.push_back( index );
    }
    v_ids.push_back( vertex_number );
    n_ids.push_back( 0 );

   /*  Simple example. Triangle pole.
    *
    *                     norm
    *                       ^
    *                       |
    *            base ------|--------- base + m_division
    *             /   \     |            /  \
    *            /     base+2           /    base+2 + m_division 
    *         base+1 -------------- base+1 + m_division 
    *
    */
    for ( size_t i = 0; i < m_ndivisions; i++ )
    {
        connections->push_back( v_ids[i] + m_ndivisions );
        connections->push_back( v_ids[i+1] + m_ndivisions );
        connections->push_back( v_ids[i+1] );
        connections->push_back( v_ids[i]   );

        const kvs::Vector3f v1 = start_circle[n_ids[i+1]] - start_circle[n_ids[i]];
        const kvs::Vector3f v2 = end_circle[n_ids[i+1]] - start_circle[n_ids[i]];
        const kvs::Vector3f norm = -v1.cross( v2 );
        normals->push_back( norm.x() );
        normals->push_back( norm.y() );
        normals->push_back( norm.z() );
    }
}

} // end of namespace kvs
