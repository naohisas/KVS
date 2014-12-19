/*****************************************************************************/
/**
 *  @file   StreamlineBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StreamlineBase.cpp 1779 2014-05-30 08:23:40Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Streamline.h"
#include <kvs/DebugNew>
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new streamline class.
 */
/*===========================================================================*/
StreamlineBase::StreamlineBase():
    kvs::MapperBase(),
    m_seed_points( NULL ),
    m_integration_method( Streamline::RungeKutta2nd ),
    m_integration_direction( Streamline::ForwardDirection ),
    m_integration_interval( 0.35f ),
    m_vector_length_threshold( 0.000001f ),
    m_integration_times_threshold( 256 ),
    m_enable_boundary_condition( true ),
    m_enable_vector_length_condition( true ),
    m_enable_integration_times_condition( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the streamline class.
 */
/*===========================================================================*/
StreamlineBase::~StreamlineBase()
{
    delete m_seed_points;
}

/*===========================================================================*/
/**
 *  @brief  Set the seed points.
 *  @param  seed_points [in] pointer to the seed points
 */
/*===========================================================================*/
void StreamlineBase::setSeedPoints( const kvs::PointObject* seed_points )
{
    // Shallow copy.
    m_seed_points = new kvs::PointObject();
    m_seed_points->setCoords( seed_points->coords() );

    // Maybe, following process is not needed.
    m_seed_points->setColor( kvs::RGBColor( 255, 255, 255 ) );
    m_seed_points->setSize( 1 );
}

/*===========================================================================*/
/**
 *  @brief  Calculates the streamlines.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void StreamlineBase::mapping( const kvs::VolumeObjectBase* volume )
{
    if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
    {
        const kvs::StructuredVolumeObject* structured_volume =
            reinterpret_cast<const kvs::StructuredVolumeObject*>( volume );

        this->extract_lines( structured_volume );
    }
    else // volume->volumeType() == kvs::VolumeObjectBase::Unstructured
    {
        kvsMessageError("Unstructured volume object is not supported in the current system.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Extracts the line segments.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void StreamlineBase::extract_lines(
    const kvs::StructuredVolumeObject* volume )
{
    kvs::IgnoreUnusedVariable( volume );

    // Calculated data arrays.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::UInt8>  colors;

    // Calculate streamline for each seed point.
    const size_t npoints = m_seed_points->numberOfVertices();
    for ( size_t index = 0; index < npoints; index++ )
    {
        std::vector<kvs::Real32> line_coords;
        std::vector<kvs::UInt8> line_colors;

        if ( this->calculate_line( &line_coords, &line_colors, index ) )
        {
            if ( !this->check_for_acceptance( line_coords ) ) continue;

            // Set the first vertex ID to the connections.
            const size_t dimension = 3;
            const size_t start_id = coords.size() / dimension;
            connections.push_back( static_cast<kvs::UInt32>( start_id ) );

            // Set the line coordinate and color value array to the coords and colors, respectively.
            const size_t ncoords = line_coords.size();
            for( size_t i = 0; i < ncoords; i++ )
            {
                coords.push_back( line_coords[i] );
                colors.push_back( line_colors[i] );
            }

            // Set the last vertex ID to the connections.
            const size_t last_id = coords.size() / dimension - 1;
            connections.push_back( static_cast<kvs::UInt32>( last_id ) );
        }
    }

    SuperClass::setLineType( kvs::LineObject::Polyline );
    SuperClass::setColorType( kvs::LineObject::VertexColor );
    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setSize( 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Calculate a streamline from the starting point that specified by the index.
 *  @param  coords [out] pointer to the coordinates value array
 *  @param  colors [out] pointer to the color value array
 *  @param  index [in] index of the starting point
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::calculate_line(
    std::vector<kvs::Real32>* coords,
    std::vector<kvs::UInt8>* colors,
    const size_t index )
{
    const kvs::Vec3 seed_point = m_seed_points->coord( index );
    if ( !this->check_for_inside_volume( seed_point ) ) return false;

    const kvs::Vec3 seed_vector = this->calculate_vector( seed_point );
    if ( m_integration_direction == StreamlineBase::BothDirections )
    {
        // Forward direction.
        std::vector<kvs::Real32> tmp_coords1;
        std::vector<kvs::UInt8> tmp_colors1;
        m_integration_direction = StreamlineBase::ForwardDirection;
        if ( !this->calculate_one_side( &tmp_coords1, &tmp_colors1, seed_point, seed_vector ) )
        {
            return false;
        }

        // backward direction.
        std::vector<kvs::Real32> tmp_coords2;
        std::vector<kvs::UInt8> tmp_colors2;
        m_integration_direction = StreamlineBase::BackwardDirection;
        if ( !this->calculate_one_side( &tmp_coords2, &tmp_colors2, seed_point, seed_vector ) )
        {
            return false;
        }

        m_integration_direction = StreamlineBase::BothDirections;

        const size_t nvertices1 = tmp_coords1.size() / 3;
        for( size_t i = 0; i < nvertices1; i++ )
        {
            const size_t id  = nvertices1 - i - 1;
            const size_t id3 = 3 * id;

            coords->push_back( tmp_coords1[id3  ] );
            coords->push_back( tmp_coords1[id3+1] );
            coords->push_back( tmp_coords1[id3+2] );
            colors->push_back( tmp_colors1[id3]   );
            colors->push_back( tmp_colors1[id3+1] );
            colors->push_back( tmp_colors1[id3+2] );
        }

        const size_t nvertices2 = tmp_coords2.size() / 3;
        for( size_t i = 1; i < nvertices2; i++ )
        {
            const size_t id3 = 3 * i;

            coords->push_back( tmp_coords2[id3  ] );
            coords->push_back( tmp_coords2[id3+1] );
            coords->push_back( tmp_coords2[id3+2] );
            colors->push_back( tmp_colors2[id3]   );
            colors->push_back( tmp_colors2[id3+1] );
            colors->push_back( tmp_colors2[id3+2] );
        }
    }
    else
    {
        // Forward or backword direction.
        return this->calculate_one_side( &(*coords), &(*colors), seed_point, seed_vector );
    }


    return true;
}

/*===========================================================================*/
/**
 *  @brief  Calculate a streamline from the seed point along the seed vector.
 *  @param  coords [out] pointer to the coordinate data array
 *  @param  colors [out] pointer to the color data array
 *  @param  seed_point [in] seed point
 *  @param  seed_vector [in] seed vector
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::calculate_one_side(
    std::vector<kvs::Real32>* coords,
    std::vector<kvs::UInt8>* colors,
    const kvs::Vec3& seed_point,
    const kvs::Vec3& seed_vector )
{
    // Register the seed point.
    kvs::Vec3 current_vertex = seed_point;
    kvs::Vec3 next_vertex = seed_point;

    coords->push_back( seed_point.x() );
    coords->push_back( seed_point.y() );
    coords->push_back( seed_point.z() );

    // Register the vector on the seed point.
    kvs::Vec3 current_vector = seed_vector;
    kvs::Vec3 previous_vector = seed_vector;

    // Set the color of seed point.
    kvs::RGBColor col = this->calculate_color( current_vector );

    colors->push_back( col.r() );
    colors->push_back( col.g() );
    colors->push_back( col.b() );

    size_t integral_times = 0;
    for ( ; ; )
    {
        // Calculate the next vertex.
        if ( !this->calculate_next_vertex(
                 current_vertex,
                 current_vector,
                 &next_vertex ) )
        {
            return true;
        }

        // Check the termination.
        if ( this->check_for_termination(
                 current_vertex,
                 current_vector,
                 integral_times,
                 next_vertex ) )
        {
            return true;
        }

        // Update the vertex and vector.
        current_vertex = next_vertex;
        previous_vector = current_vector;

        coords->push_back( current_vertex.x() );
        coords->push_back( current_vertex.y() );
        coords->push_back( current_vertex.z() );

        // Interpolate vector from vertex of cell.
        current_vector = this->interpolate_vector( current_vertex, previous_vector );

        // Set color of vertex.
        kvs::RGBColor col = this->calculate_color( current_vector );

        colors->push_back( col.r() );
        colors->push_back( col.g() );
        colors->push_back( col.b() );

        integral_times++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculate a next vertex.
 *  @param  current_vertex [in] current vertex
 *  @param  current_direction [in] current direction vector
 *  @param  next_vertex [in] next vertex
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::calculate_next_vertex(
    const kvs::Vec3& current_vertex,
    const kvs::Vec3& current_direction,
    kvs::Vec3* next_vertex )
{
    switch( m_integration_method )
    {
    case StreamlineBase::Euler:
        return this->integrate_by_euler( current_vertex, current_direction, &(*next_vertex) );
    case StreamlineBase::RungeKutta2nd:
        return this->integrate_by_runge_kutta_2nd( current_vertex, current_direction, &(*next_vertex) );
    case StreamlineBase::RungeKutta4th:
        return this->integrate_by_runge_kutta_4th( current_vertex, current_direction, &(*next_vertex) );
    default: break;
    }

    kvsMessageError( "Specified integral method is not defined." );
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Integrate by Eular.
 *  @param  current_vertex [in] current vertex
 *  @param  current_direction [in] current direction vector
 *  @param  next_vertex [in] next vertex
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::integrate_by_euler(
    const kvs::Vec3& current_vertex,
    const kvs::Vec3& current_direction,
    kvs::Vec3* next_vertex )
{
    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( current_vertex ) ) return false;
    }

    const float integration_direction = static_cast<float>( m_integration_direction );
    const kvs::Vec3 k1 = current_direction.normalized() * integration_direction;
    *next_vertex = current_vertex + m_integration_interval * k1;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Integrate by Runge-Kutta 2nd.
 *  @param  current_vertex [in] current vertex
 *  @param  current_direction [in] current direction vector
 *  @param  next_vertex [in] next vertex
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::integrate_by_runge_kutta_2nd(
    const kvs::Vec3& current_vertex,
    const kvs::Vec3& current_direction,
    kvs::Vec3* next_vertex )
{
    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( current_vertex ) ) return false;
    }

    const float integration_direction = static_cast<float>( m_integration_direction );
    const kvs::Vec3 k1 = current_direction.normalized() * integration_direction;
    // Interpolate vector from vertex of cell.
    const kvs::Vec3 vertex = current_vertex + 0.5f * m_integration_interval * k1;

    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( vertex ) ) return false;
    }

    const kvs::Vec3 direction = this->interpolate_vector( vertex, current_direction );
    const kvs::Vec3 k2 = direction.normalized() * integration_direction;
    *next_vertex = vertex + m_integration_interval * k2;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Integrate by Runge-Kutta 4th.
 *  @param  current_vertex [in] current vertex
 *  @param  current_direction [in] current direction vector
 *  @param  next_vertex [in] next vertex
 *  @return 
 */
/*===========================================================================*/
bool StreamlineBase::integrate_by_runge_kutta_4th(
    const kvs::Vec3& current_vertex,
    const kvs::Vec3& current_direction,
    kvs::Vec3* next_vertex )
{
    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( current_vertex ) ) return false;
    }

    // Calculate integration interval.

    const float integration_direction = static_cast<float>( m_integration_direction );
    const kvs::Vec3 k1 = current_direction.normalized() * integration_direction;

    // Interpolate vector from vertex of cell.
    const kvs::Vec3 vertex2 = current_vertex + 0.5f * m_integration_interval * k1;

    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( vertex2 ) ) return false;
    }

    const kvs::Vec3 direction2 = this->interpolate_vector( vertex2, current_direction );
    const kvs::Vec3 k2 = direction2.normalized() * integration_direction;

    // Interpolate vector from vertex of cell.
    const kvs::Vec3 vertex3 = current_vertex + 0.5f * m_integration_interval * k2;

    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( vertex3 ) ) return false;
    }

    const kvs::Vec3 direction3 = this->interpolate_vector( vertex3, current_direction );
    const kvs::Vec3 k3 = direction3.normalized() * integration_direction;

    // Interpolate vector from vertex of cell.
    const kvs::Vec3 vertex4 = current_vertex + m_integration_interval * k3;

    if ( m_enable_boundary_condition )
    {
        if ( !this->check_for_inside_volume( vertex4 ) ) return false;
    }

    const kvs::Vec3 direction4 = this->interpolate_vector( vertex4, current_direction );
    const kvs::Vec3 k4 = direction4.normalized() * integration_direction;

    *next_vertex = current_vertex + integration_direction * ( k1 + 2.0f * ( k2 + k3 ) + k4 ) / 6.0f;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Check whether the seed point is included in the volume.
 *  @param  point [in] seed point
 *  @return true, if the seed point is inside the volume
 */
/*===========================================================================*/
bool StreamlineBase::check_for_inside_volume( const kvs::Vec3& point )
{
    switch ( BaseClass::volume()->volumeType() )
    {
    case kvs::VolumeObjectBase::Structured:
    {
        const kvs::StructuredVolumeObject* structured_volume =
            kvs::StructuredVolumeObject::DownCast( BaseClass::volume() );
        switch ( structured_volume->gridType() )
        {
        case kvs::StructuredVolumeObject::Uniform:
        {
            const float dimx = static_cast<float>( structured_volume->resolution().x() - 1 );
            const float dimy = static_cast<float>( structured_volume->resolution().y() - 1 );
            const float dimz = static_cast<float>( structured_volume->resolution().z() - 1 );

            if ( point.x() < 0.0f || dimx < point.x() ) return false;
            if ( point.y() < 0.0f || dimy < point.y() ) return false;
            if ( point.z() < 0.0f || dimz < point.z() ) return false;

            return true;
        }
        case kvs::StructuredVolumeObject::Rectilinear:
        {
            const kvs::Vec3& min_obj = structured_volume->minObjectCoord();
            const kvs::Vec3& max_obj = structured_volume->maxObjectCoord();

            if ( point.x() < min_obj.x() || max_obj.x() < point.x() ) return false;
            if ( point.y() < min_obj.y() || max_obj.y() < point.y() ) return false;
            if ( point.z() < min_obj.z() || max_obj.z() < point.z() ) return false;

            return true;
        }
        default: break;
        }
        break;
    }
    default: break;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Check the vector length.
 *  @param  direction [in] direction vector
 *  @return true if the vector length is smaller than the threshold
 */
/*===========================================================================*/
bool StreamlineBase::check_for_vector_length( const kvs::Vec3& direction )
{
    return direction.length() < m_vector_length_threshold;
}

/*===========================================================================*/
/**
 *  @brief  Check the number of integrations.
 *  @param  times [in] number of integrations
 *  @return true if the given number is bigger than the threshold
 */
/*===========================================================================*/
bool StreamlineBase::check_for_integration_times( const size_t times )
{
    return times >= m_integration_times_threshold;
}

} // end of namespace kvs
