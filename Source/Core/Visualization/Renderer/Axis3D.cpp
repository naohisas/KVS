#include "Axis3D.h"
#include <kvs/OpenGL>
#include <kvs/Vector3>
#include <kvs/Matrix44>
#include <kvs/RGBColor>
#include <kvs/String>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/LineObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis3D class.
 */
/*===========================================================================*/
Axis3D::Axis3D():
    m_axis_color( kvs::RGBColor::Black() ),
    m_axis_width( 3.0f ),
    m_grid_draw_mode( Axis3D::StaticGrid ),
    m_gridline_pattern( Axis3D::SolidLine ),
    m_ngridlines( kvs::Vec3u( 5, 5, 5 ) ),
    m_gridline_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_gridline_width( 1.0f ),
    m_x_label("X"),
    m_y_label("Y"),
    m_z_label("Z"),
    m_label_color( kvs::RGBColor::Black() ),
    m_value_color( kvs::RGBColor::Black() ),
    m_background_color( kvs::RGBAColor::White() ),
    m_enable_anti_aliasing( false ),
    m_show_labels( true ),
    m_show_values( true ),
    m_show_gridlines( true ),
    m_show( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void Axis3D::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    if ( !m_show ) return;

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    {
        // Anti-aliasing.
        if ( m_enable_anti_aliasing )
        {
            kvs::OpenGL::Enable( GL_LINE_SMOOTH );
            kvs::OpenGL::Enable( GL_BLEND );
            kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        }

        // Center of the object.
        const kvs::Vec3 min_coord = object->minObjectCoord();
        const kvs::Vec3 max_coord = object->maxObjectCoord();
        const kvs::Vec3 center = ( max_coord + min_coord ) * 0.5f;

        // Depth values (Z values) in the camera coordinate system.
        GLfloat m[16]; kvs::OpenGL::GetModelViewMatrix( m );
        const kvs::Real32 x_min = min_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 x_max = max_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 y_min = center.x() * m[2] + min_coord.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 y_max = center.x() * m[2] + max_coord.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 z_min = center.x() * m[2] + center.y() * m[6] + min_coord.z() * m[10] + m[14];
        const kvs::Real32 z_max = center.x() * m[2] + center.y() * m[6] + max_coord.z() * m[10] + m[14];

        kvs::OpenGL::Enable( GL_DEPTH_TEST );

        switch ( m_grid_draw_mode )
        {
        case Axis3D::StaticGrid:
        {
            const kvs::Real32 depth_x = min_coord.x();
            const kvs::Real32 depth_y = min_coord.y();
            const kvs::Real32 depth_z = min_coord.z();
            this->draw_xy_axes( min_coord, max_coord, depth_z );
            this->draw_yz_axes( min_coord, max_coord, depth_x );
            this->draw_zx_axes( min_coord, max_coord, depth_y );
            this->draw_xy_gridlines( min_coord, max_coord, depth_z );
            this->draw_yz_gridlines( min_coord, max_coord, depth_x );
            this->draw_zx_gridlines( min_coord, max_coord, depth_y );
            this->draw_labels( min_coord, max_coord );
            break;
        }
        case Axis3D::FarGrid:
        {
            const kvs::Real32 depth_x = x_min <= x_max ? min_coord.x() : max_coord.x();
            const kvs::Real32 depth_y = y_min <= y_max ? min_coord.y() : max_coord.y();
            const kvs::Real32 depth_z = z_min <= z_max ? min_coord.z() : max_coord.z();
            this->draw_xy_axes( min_coord, max_coord, depth_z );
            this->draw_yz_axes( min_coord, max_coord, depth_x );
            this->draw_zx_axes( min_coord, max_coord, depth_y );
            this->draw_xy_gridlines( min_coord, max_coord, depth_z );
            this->draw_yz_gridlines( min_coord, max_coord, depth_x );
            this->draw_zx_gridlines( min_coord, max_coord, depth_y );
            this->draw_labels( min_coord, max_coord );
            break;
        }
        default: break;
        }
    }

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Returns a pointer to the line object represents the axis.
 *  @param  object [in] pointer to the object
 *  @return pointer to the line object
 *  @note   The returned pointer is allocated in this method, but it has not
 *          been deallocated in this class.
 */
/*===========================================================================*/
kvs::LineObject* Axis3D::outputLineObject( const kvs::ObjectBase* object ) const
{
    const kvs::Real32 min_x = object->minObjectCoord().x();
    const kvs::Real32 min_y = object->minObjectCoord().y();
    const kvs::Real32 min_z = object->minObjectCoord().z();
    const kvs::Real32 max_x = object->maxObjectCoord().x();
    const kvs::Real32 max_y = object->maxObjectCoord().y();
    const kvs::Real32 max_z = object->maxObjectCoord().z();

    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connections;
    std::vector<kvs::Real32> sizes;
    std::vector<kvs::UInt8> colors;

    coords.push_back( min_x ); coords.push_back( max_y ); coords.push_back( min_z ); // 0
    coords.push_back( min_x ); coords.push_back( min_y ); coords.push_back( min_z ); // 1
    coords.push_back( max_x ); coords.push_back( min_y ); coords.push_back( min_z ); // 2
    coords.push_back( max_x ); coords.push_back( max_y ); coords.push_back( min_z ); // 3
    coords.push_back( min_x ); coords.push_back( max_y ); coords.push_back( max_z ); // 4
    coords.push_back( min_x ); coords.push_back( min_y ); coords.push_back( max_z ); // 5
    coords.push_back( max_x ); coords.push_back( min_y ); coords.push_back( max_z ); // 6

    connections.push_back( 0 ); connections.push_back( 1 );
    connections.push_back( 1 ); connections.push_back( 2 );
    connections.push_back( 2 ); connections.push_back( 3 );
    connections.push_back( 3 ); connections.push_back( 0 );
    connections.push_back( 0 ); connections.push_back( 4 );
    connections.push_back( 1 ); connections.push_back( 5 );
    connections.push_back( 2 ); connections.push_back( 6 );
    connections.push_back( 4 ); connections.push_back( 5 );
    connections.push_back( 5 ); connections.push_back( 6 );

    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );
    sizes.push_back( m_axis_width );

    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );
    colors.push_back( m_axis_color.r() ); colors.push_back( m_axis_color.g() ); colors.push_back( m_axis_color.b() );

    if ( m_show_gridlines )
    {
        const kvs::Real32 interval_x = ( max_x - min_x ) / ( m_ngridlines.x() + 1 );
        const kvs::Real32 interval_y = ( max_y - min_y ) / ( m_ngridlines.y() + 1 );
        const kvs::Real32 interval_z = ( max_z - min_z ) / ( m_ngridlines.z() + 1 );

        kvs::Real32 x = min_x;
        for ( size_t i = 0; i < m_ngridlines.x(); i++ )
        {
            x += interval_x;
            coords.push_back( x ); coords.push_back( max_y ); coords.push_back( min_z );
            coords.push_back( x ); coords.push_back( min_y ); coords.push_back( min_z );

            coords.push_back( x ); coords.push_back( min_y ); coords.push_back( min_z );
            coords.push_back( x ); coords.push_back( min_y ); coords.push_back( max_z );
        }

        kvs::Real32 y = min_y;
        for ( size_t i = 0; i < m_ngridlines.y(); i++ )
        {
            y += interval_y;
            coords.push_back( min_x ); coords.push_back( y ); coords.push_back( max_z );
            coords.push_back( min_x ); coords.push_back( y ); coords.push_back( min_z );

            coords.push_back( min_x ); coords.push_back( y ); coords.push_back( min_z );
            coords.push_back( max_x ); coords.push_back( y ); coords.push_back( min_z );
        }

        kvs::Real32 z = min_z;
        for ( size_t i = 0; i < m_ngridlines.z(); i++ )
        {
            z += interval_z;
            coords.push_back( max_x ); coords.push_back( min_y ); coords.push_back( z );
            coords.push_back( min_x ); coords.push_back( min_y ); coords.push_back( z );

            coords.push_back( min_x ); coords.push_back( max_y ); coords.push_back( z );
            coords.push_back( min_x ); coords.push_back( min_y ); coords.push_back( z );
        }

        for ( size_t i = 7; i < coords.size() / 3; i++ )
        {
            connections.push_back( i );
        }

        const size_t ngridlines = m_ngridlines.x() * m_ngridlines.y() * m_ngridlines.z();
        for( size_t i = 0; i < ngridlines; i++ )
        {
            sizes.push_back( m_gridline_width );
            colors.push_back( m_gridline_color.r() );
            colors.push_back( m_gridline_color.g() );
            colors.push_back( m_gridline_color.b() );
        }
    }

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineType( kvs::LineObject::Segment );
    line_object->setColorType( kvs::LineObject::LineColor );
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    line_object->setSizes( kvs::ValueArray<kvs::Real32>( sizes ) );
    line_object->setColors( kvs::ValueArray<kvs::UInt8>( colors ) );

    return line_object;
}

/*===========================================================================*/
/**
 *  @brief  Draws the axes (border lines) on the XY plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_z [in] Z coordinate value of the XY plane
 */
/*===========================================================================*/
void Axis3D::draw_xy_axes( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_z )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    kvs::OpenGL::SetPolygonOffset( 1, -m_axis_width );
    kvs::OpenGL::Color( m_axis_color );
    kvs::OpenGL::SetLineWidth( m_axis_width );
    kvs::OpenGL::Begin( GL_LINE_LOOP );
    kvs::OpenGL::Vertex( min_coord.x(), min_coord.y(), depth_z );
    kvs::OpenGL::Vertex( max_coord.x(), min_coord.y(), depth_z );
    kvs::OpenGL::Vertex( max_coord.x(), max_coord.y(), depth_z );
    kvs::OpenGL::Vertex( min_coord.x(), max_coord.y(), depth_z );
    kvs::OpenGL::End();

    kvs::OpenGL::SetPolygonOffset( 1, m_axis_width );
    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( m_background_color );
    kvs::OpenGL::Vertex( min_coord.x(), min_coord.y(), depth_z );
    kvs::OpenGL::Vertex( max_coord.x(), min_coord.y(), depth_z );
    kvs::OpenGL::Vertex( max_coord.x(), max_coord.y(), depth_z );
    kvs::OpenGL::Vertex( min_coord.x(), max_coord.y(), depth_z );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the axes (border lines) on the YZ plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_x [in] X coordinate value of the YZ plane
 */
/*===========================================================================*/
void Axis3D::draw_yz_axes( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_x )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    kvs::OpenGL::SetPolygonOffset( 1, -m_axis_width );
    kvs::OpenGL::Color( m_axis_color );
    kvs::OpenGL::SetLineWidth( m_axis_width );
    kvs::OpenGL::Begin( GL_LINE_LOOP );
    kvs::OpenGL::Vertex( depth_x, min_coord.y(), min_coord.z() );
    kvs::OpenGL::Vertex( depth_x, max_coord.y(), min_coord.z() );
    kvs::OpenGL::Vertex( depth_x, max_coord.y(), max_coord.z() );
    kvs::OpenGL::Vertex( depth_x, min_coord.y(), max_coord.z() );
    kvs::OpenGL::End();

    kvs::OpenGL::SetPolygonOffset( 1, m_axis_width );
    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( m_background_color );
    kvs::OpenGL::Vertex( depth_x, min_coord.y(), min_coord.z() );
    kvs::OpenGL::Vertex( depth_x, max_coord.y(), min_coord.z() );
    kvs::OpenGL::Vertex( depth_x, max_coord.y(), max_coord.z() );
    kvs::OpenGL::Vertex( depth_x, min_coord.y(), max_coord.z() );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the axes (border lines) on the ZX plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_y [in] Y coordinate value of the ZX plane
 */
/*===========================================================================*/
void Axis3D::draw_zx_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_y )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    kvs::OpenGL::SetPolygonOffset( 1, -m_axis_width );
    kvs::OpenGL::Color( m_axis_color );
    kvs::OpenGL::SetLineWidth( m_axis_width );
    kvs::OpenGL::Begin( GL_LINE_LOOP );
    kvs::OpenGL::Vertex( min_coord.x(), depth_y, min_coord.z() );
    kvs::OpenGL::Vertex( min_coord.x(), depth_y, max_coord.z() );
    kvs::OpenGL::Vertex( max_coord.x(), depth_y, max_coord.z() );
    kvs::OpenGL::Vertex( max_coord.x(), depth_y, min_coord.z() );
    kvs::OpenGL::End();

    kvs::OpenGL::SetPolygonOffset( 1, m_axis_width );
    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( m_background_color );
    kvs::OpenGL::Vertex( min_coord.x(), depth_y, min_coord.z() );
    kvs::OpenGL::Vertex( min_coord.x(), depth_y, max_coord.z() );
    kvs::OpenGL::Vertex( max_coord.x(), depth_y, max_coord.z() );
    kvs::OpenGL::Vertex( max_coord.x(), depth_y, min_coord.z() );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the XY plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_z [in] Z coordinate value of the XY plane
 */
/*===========================================================================*/
void Axis3D::draw_xy_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_z )
{
    if ( !m_show_gridlines ) return;

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    kvs::OpenGL::Color( m_gridline_color );
    kvs::OpenGL::SetLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        kvs::OpenGL::Enable( GL_LINE_STIPPLE );
        kvs::OpenGL::SetLineStipple( 1, 0xF0F0 );
    }

    // X lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 x = min_coord.x();
    const kvs::Real32 x_interval = ( max_coord.x() - min_coord.x() ) / ( m_ngridlines.x() + 1 );
    for ( size_t i = 0; i < m_ngridlines.x(); i++ )
    {
        x += x_interval;
        kvs::OpenGL::Vertex( x, min_coord.y(), depth_z );
        kvs::OpenGL::Vertex( x, max_coord.y(), depth_z );
    }
    kvs::OpenGL::End();

    // Y lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 y = min_coord.y();
    const kvs::Real32 y_interval = ( max_coord.y() - min_coord.y() ) / ( m_ngridlines.y() + 1 );
    for ( size_t i = 0; i < m_ngridlines.y(); i++ )
    {
        y += y_interval;
        kvs::OpenGL::Vertex( min_coord.x(), y, depth_z );
        kvs::OpenGL::Vertex( max_coord.x(), y, depth_z );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the YZ plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_x [in] X coordinate value of the YZ plane
 */
/*===========================================================================*/
void Axis3D::draw_yz_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_x )
{
    if ( !m_show_gridlines ) return;

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    kvs::OpenGL::Color( m_gridline_color );
    kvs::OpenGL::SetLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        kvs::OpenGL::Enable( GL_LINE_STIPPLE );
        kvs::OpenGL::SetLineStipple( 1, 0xF0F0 );
    }

    // Y lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 y = min_coord.y();
    const kvs::Real32 y_interval = ( max_coord.y() - min_coord.y() ) / ( m_ngridlines.y() + 1 );
    for ( size_t i = 0; i < m_ngridlines.y(); i++ )
    {
        y += y_interval;
        kvs::OpenGL::Vertex( depth_x, y, min_coord.z() );
        kvs::OpenGL::Vertex( depth_x, y, max_coord.z() );
    }
    kvs::OpenGL::End();

    // Z lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 z = min_coord.z();
    const kvs::Real32 z_interval = ( max_coord.z() - min_coord.z() ) / ( m_ngridlines.z() + 1 );
    for ( size_t i = 0; i < m_ngridlines.z(); i++ )
    {
        z += z_interval;
        kvs::OpenGL::Vertex( depth_x, min_coord.y(), z );
        kvs::OpenGL::Vertex( depth_x, max_coord.y(), z );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the ZX plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_y [in] Y coordinate value of the ZX plane
 */
/*===========================================================================*/
void Axis3D::draw_zx_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_y )
{
    if ( !m_show_gridlines ) return;

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    kvs::OpenGL::Color( m_gridline_color );
    kvs::OpenGL::SetLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        kvs::OpenGL::Enable( GL_LINE_STIPPLE );
        kvs::OpenGL::SetLineStipple( 1, 0xF0F0 );
    }

    // Z lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 z = min_coord.z();
    const kvs::Real32 z_interval = ( max_coord.z() - min_coord.z() ) / ( m_ngridlines.z() + 1 );
    for ( size_t i = 0; i < m_ngridlines.z(); i++ )
    {
        z += z_interval;
        kvs::OpenGL::Vertex( min_coord.x(), depth_y, z );
        kvs::OpenGL::Vertex( max_coord.x(), depth_y, z );
    }
    kvs::OpenGL::End();

    // X lines.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::Real32 x = min_coord.x();
    const kvs::Real32 x_interval = ( max_coord.x() - min_coord.x() ) / ( m_ngridlines.x() + 1 );
    for ( size_t i = 0; i < m_ngridlines.x(); i++ )
    {
        x += x_interval;
        kvs::OpenGL::Vertex( x, depth_y, min_coord.z() );
        kvs::OpenGL::Vertex( x, depth_y, max_coord.z() );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the labels and values along each axis.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 */
/*===========================================================================*/
void Axis3D::draw_labels( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord )
{
    const kvs::Mat4 m = kvs::OpenGL::ModelViewMatrix();

    // Center of the boundary box in the object coordinate system.
    const kvs::Vec3 center = ( max_coord + min_coord ) * 0.5f;

    // X-coordinate of the center in the camera coordinate system.
    const kvs::Real32 camx_cen = m[0].dot( kvs::Vec4( center, 1.0f ) );

    // Depth values (Z values) at each boundary plane in the camera coordinate system.
    const kvs::Real32 x_min = m[2].dot( kvs::Vec4( min_coord.x(), center.y(), center.z(), 1.0f ) );
    const kvs::Real32 x_max = m[2].dot( kvs::Vec4( max_coord.x(), center.y(), center.z(), 1.0f ) );
    const kvs::Real32 y_min = m[2].dot( kvs::Vec4( center.x(), min_coord.y(), center.z(), 1.0f ) );
    const kvs::Real32 y_max = m[2].dot( kvs::Vec4( center.x(), max_coord.y(), center.z(), 1.0f ) );
    const kvs::Real32 z_min = m[2].dot( kvs::Vec4( center.x(), center.y(), min_coord.z(), 1.0f ) );
    const kvs::Real32 z_max = m[2].dot( kvs::Vec4( center.x(), center.y(), max_coord.z(), 1.0f ) );

    const kvs::Vec3 min_value = min_coord;
    const kvs::Vec3 max_value = max_coord;
    kvs::Vec3 dvalue = max_value - min_value;
    dvalue.x() /= static_cast<float>( m_ngridlines.x() + 1 );
    dvalue.y() /= static_cast<float>( m_ngridlines.y() + 1 );
    dvalue.z() /= static_cast<float>( m_ngridlines.z() + 1 );

    kvs::Vec3 interval = max_value - min_value;
    interval.x() /= static_cast<float>( m_ngridlines.x() + 1 );
    interval.y() /= static_cast<float>( m_ngridlines.y() + 1 );
    interval.z() /= static_cast<float>( m_ngridlines.z() + 1 );

    const kvs::Vec3 diff( max_coord - min_coord );
    const kvs::Real32 length = kvs::Math::Max( diff.x(), diff.y(), diff.z() );
//    const kvs::Real32 label_offset = length * 0.4f;
    const kvs::Real32 label_offset = length * 0.25f;
    const kvs::Real32 value_offset = length * 0.05f;

    m_painter.begin( screen() );

    // Draw X label and values.
    {
        kvs::Vec3 position( center.x(), min_coord.y(), min_coord.z() );
        if ( m_grid_draw_mode == FarGrid )
        {
            position.y() = y_min <= y_max ? min_coord.y() : max_coord.y();
            position.z() = z_min <= z_max ? max_coord.z() : min_coord.z();
        }

        const kvs::Real32 camx_pos = m[0].dot( kvs::Vec4( position, 1.0f ) );
        if ( camx_cen <= camx_pos ) m_painter.font().setHorizontalAlign( kvs::Font::Left );
        else m_painter.font().setHorizontalAlign( kvs::Font::Right );

        // Label.
        if ( m_show_labels )
        {
            kvs::Vec3 offset( 0.0f, -label_offset, -label_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.y() = 0.0f;
                offset.z() = z_min <= z_max ?  label_offset : -label_offset;
            }

            m_painter.font().setColor( m_label_color );
            m_painter.font().setStyleToBold();
            m_painter.drawText( position + offset, m_x_label );
            m_painter.font().setStyleToRegular();
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vec3 offset( 0.0f, -value_offset, -value_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.y() = 0.0f;
                offset.z() = z_min <= z_max ?  value_offset : -value_offset;
            }

            m_painter.font().setColor( m_value_color );
            const size_t nlines = size_t( m_ngridlines.x() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.x() + dvalue.x() * i );
                const float x = min_coord.x() + interval.x() * i;
                const float y = position.y() + offset.y();
                const float z = position.z() + offset.z();
                m_painter.drawText( kvs::Vec3( x, y, z ), number );
            }
        }

        m_painter.font().setHorizontalAlign( kvs::Font::Left );
    }

    // Draw Y label and values.
    {
        kvs::Vector3f position( min_coord.x(), center.y(), min_coord.z() );
        if ( m_grid_draw_mode == FarGrid )
        {
            position.x() = x_min <= x_max ? min_coord.x() : max_coord.x();
            position.z() = z_min <= z_max ? max_coord.z() : min_coord.z();
        }

        const kvs::Real32 camx_pos = m[0].dot( kvs::Vec4( position, 1.0f ) );
        if ( camx_cen <= camx_pos ) m_painter.font().setHorizontalAlign( kvs::Font::Left );
        else m_painter.font().setHorizontalAlign( kvs::Font::Right );

        // Label.
        if ( m_show_labels )
        {
            kvs::Vec3 offset( -label_offset, 0.0f, -label_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = 0.0f;
                offset.z() = z_min <= z_max ?  label_offset : -label_offset;
            }

            m_painter.font().setColor( m_label_color );
            m_painter.font().setStyleToBold();
            m_painter.drawText( position + offset, m_y_label );
            m_painter.font().setStyleToRegular();
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vector3f offset( -value_offset, 0.0f, -value_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = 0.0f;
                offset.z() = z_min <= z_max ?  value_offset : -value_offset;
            }

            m_painter.font().setColor( m_value_color );
            const size_t nlines = size_t( m_ngridlines.y() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.y() + dvalue.y() * i );
                const float x = position.x() + offset.x();
                const float y = min_coord.y() + interval.y() * i;
                const float z = position.z() + offset.z();
                m_painter.drawText( kvs::Vec3( x, y, z ), number );
            }
        }
    }

    // Draw Z label and values.
    {
        kvs::Vector3f position( min_coord.x(), min_coord.y(), center.z() );
        if ( m_grid_draw_mode == FarGrid )
        {
            position.x() = x_min <= x_max ? max_coord.x() : min_coord.x();
            position.y() = y_min <= y_max ? min_coord.y() : max_coord.y();
        }

        const kvs::Real32 camx_pos = m[0].dot( kvs::Vec4( position, 1.0f ) );
        if ( camx_cen <= camx_pos ) m_painter.font().setHorizontalAlign( kvs::Font::Left );
        else m_painter.font().setHorizontalAlign( kvs::Font::Right );

        // Label.
        if ( m_show_labels )
        {
            kvs::Vec3 offset( -label_offset, -label_offset, 0.0f );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ?  label_offset : -label_offset;
                offset.y() = 0.0f;
            }

            m_painter.font().setColor( m_label_color );
            m_painter.font().setStyleToBold();
            m_painter.drawText( position + offset, m_z_label );
            m_painter.font().setStyleToRegular();
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vec3 offset( -value_offset, -value_offset, 0.0f );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ?  value_offset : -value_offset;
                offset.y() = 0.0f;
            }

            m_painter.font().setColor( m_value_color );
            const size_t nlines = size_t( m_ngridlines.z() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.z() + dvalue.z() * i );
                const float x = position.x() + offset.x();
                const float y = position.y() + offset.y();
                const float z = min_coord.z() + interval.z() * i;
                m_painter.drawText( kvs::Vec3( x, y, z ), number );
            }
        }
    }
    m_painter.end();
}

} // end of namespace kvs
