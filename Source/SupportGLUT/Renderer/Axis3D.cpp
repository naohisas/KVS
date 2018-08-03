/*****************************************************************************/
/**
 *  @file   Axis3D.cp
 *  @author Naohisa Sakamoto
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
#include "Axis3D.h"
#include <kvs/OpenGL>
#include <kvs/Vector3>
#include <kvs/RGBColor>
#include <kvs/String>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/LineObject>
#include <kvs/glut/GLUT>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis3D class.
 */
/*===========================================================================*/
Axis3D::Axis3D():
    m_axis_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_axis_width( 3.0f ),
    m_grid_draw_mode( Axis3D::StaticGrid ),
    m_gridline_pattern( Axis3D::SolidLine ),
    m_ngridlines( kvs::Vector3ui( 5, 5, 5 ) ),
    m_gridline_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_gridline_width( 1.0f ),
    m_x_label("X"),
    m_y_label("Y"),
    m_z_label("Z"),
    m_label_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_value_color( kvs::RGBColor( 0, 0, 0 ) ),
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
    if ( !m_show ) return;

    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
        glEnable( GL_LINE_SMOOTH );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    glEnable( GL_DEPTH_TEST );
    {
        // Center of the object.
        const kvs::Vector3f min_coord = object->minObjectCoord();
        const kvs::Vector3f max_coord = object->maxObjectCoord();
        const kvs::Vector3f center = ( max_coord + min_coord ) * 0.5f;

        // Depth values (Z values) in the camera coordinate system.
        GLfloat m[16]; glGetFloatv( GL_MODELVIEW_MATRIX, m );
        const kvs::Real32 x_min = min_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 x_max = max_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 y_min = center.x() * m[2] + min_coord.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 y_max = center.x() * m[2] + max_coord.y() * m[6] + center.z() * m[10] + m[14];
        const kvs::Real32 z_min = center.x() * m[2] + center.y() * m[6] + min_coord.z() * m[10] + m[14];
        const kvs::Real32 z_max = center.x() * m[2] + center.y() * m[6] + max_coord.z() * m[10] + m[14];

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

    glDisable( GL_DEPTH_TEST );
    glPopAttrib();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Sets a axis color.
 *  @param  color [in] axis color
 */
/*===========================================================================*/
void Axis3D::setAxisColor( const kvs::RGBColor& color )
{
    m_axis_color = color;
}

/*===========================================================================*/
/**
 *  @brief  Sets a axis width.
 *  @param  width [in] axis width
 */
/*===========================================================================*/
void Axis3D::setAxisWidth( const kvs::Real32 width )
{
    m_axis_width = width;
}

/*===========================================================================*/
/**
 *  @brief  Sets a grid-draw mode.
 *  @param  mode [in] grid-draw mode
 */
/*===========================================================================*/
void Axis3D::setGridDrawMode( const GridDrawMode mode )
{
    m_grid_draw_mode = mode;
}

/*===========================================================================*/
/**
 *  @brief  Sets a gridline pattern.
 *  @param  pattern [in] gridline pattern
 */
/*===========================================================================*/
void Axis3D::setGridlinePattern( const GridlinePattern pattern )
{
    m_gridline_pattern = pattern;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of gridlines.
 *  @param  ngridlines [in] number of gridlines
 */
/*===========================================================================*/
void Axis3D::setNumberOfGridlines( const kvs::Vector3ui& ngridlines )
{
    m_ngridlines = ngridlines;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of gridlines along the X axis.
 *  @param  ngridlines [in] number of gridlines
 */
/*===========================================================================*/
void Axis3D::setNumberOfXGridlines( const size_t ngridlines )
{
    m_ngridlines[0] = kvs::UInt32( ngridlines );
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of gridlines along the Y axis.
 *  @param  ngridlines [in] number of gridlines
 */
/*===========================================================================*/
void Axis3D::setNumberOfYGridlines( const size_t ngridlines )
{
    m_ngridlines[1] = kvs::UInt32( ngridlines );
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of gridlines along the Z axis.
 *  @param  ngridlines [in] number of gridlines
 */
/*===========================================================================*/
void Axis3D::setNumberOfZGridlines( const size_t ngridlines )
{
    m_ngridlines[2] = kvs::UInt32( ngridlines );
}

/*===========================================================================*/
/**
 *  @brief  Sets a gridline color.
 *  @param  color [in] color
 */
/*===========================================================================*/
void Axis3D::setGridlineColor( const kvs::RGBColor& color )
{
    m_gridline_color = color;
}

/*===========================================================================*/
/**
 *  @brief  Sets a gridline width.
 *  @param  width [in] gridline width
 */
/*===========================================================================*/
void Axis3D::setGridlineWidth( const kvs::Real32 width )
{
    m_gridline_width = width;
}

/*===========================================================================*/
/**
 *  @brief  Sets a label of the X axis.
 *  @param  label [in] label
 */
/*===========================================================================*/
void Axis3D::setXLabel( const std::string& label )
{
    m_x_label = label;
}

/*===========================================================================*/
/**
 *  @brief  Sets a label of the Y axis.
 *  @param  label [in] label
 */
/*===========================================================================*/
void Axis3D::setYLabel( const std::string& label )
{
    m_y_label = label;
}

/*===========================================================================*/
/**
 *  @brief  Sets a label of the Z axis.
 *  @param  label [in] label
 */
/*===========================================================================*/
void Axis3D::setZLabel( const std::string& label )
{
    m_z_label = label;
}

/*===========================================================================*/
/**
 *  @brief  Sets a label color.
 *  @param  color [in] color
 */
/*===========================================================================*/
void Axis3D::setLabelColor( const kvs::RGBColor& color )
{
    m_label_color = color;
}

/*===========================================================================*/
/**
 *  @brief  Sets a value color.
 *  @param  color [in] color
 */
/*===========================================================================*/
void Axis3D::setValueColor( const kvs::RGBColor& color )
{
    m_value_color = color;
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing.
 */
/*===========================================================================*/
void Axis3D::enableAntiAliasing()
{
    m_enable_anti_aliasing = true;
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing.
 */
/*===========================================================================*/
void Axis3D::disableAntiAliasing()
{
    m_enable_anti_aliasing = false;
}

/*===========================================================================*/
/**
 *  @brief  Shows the labels.
 */
/*===========================================================================*/
void Axis3D::showLabels()
{
    m_show_labels = true;
}

/*===========================================================================*/
/**
 *  @brief  Shows the values.
 */
/*===========================================================================*/
void Axis3D::showValues()
{
    m_show_values = true;
}

/*===========================================================================*/
/**
 *  @brief  Shows the gridlines.
 */
/*===========================================================================*/
void Axis3D::showGridlines()
{
    m_show_gridlines = true;
}

/*===========================================================================*/
/**
 *  @brief  Shows the axis.
 */
/*===========================================================================*/
void Axis3D::show()
{
    m_show = true;
}

/*===========================================================================*/
/**
 *  @brief  Hides the labels.
 */
/*===========================================================================*/
void Axis3D::hideLabels()
{
    m_show_labels = false;
}

/*===========================================================================*/
/**
 *  @brief  Hides the values.
 */
/*===========================================================================*/
void Axis3D::hideValues()
{
    m_show_values = false;
}

/*===========================================================================*/
/**
 *  @brief  Hides the gridlines.
 */
/*===========================================================================*/
void Axis3D::hideGridlines()
{
    m_show_gridlines = false;
}

/*===========================================================================*/
/**
 *  @brief  Hides the axis.
 */
/*===========================================================================*/
void Axis3D::hide()
{
    m_show = false;
}

/*===========================================================================*/
/**
 *  @brief  Returns the axis color.
 *  @return axis color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis3D::axisColor() const
{
    return m_axis_color;
}

/*===========================================================================*/
/**
 *  @brief  Returns the axis width.
 *  @return axis width
 */
/*===========================================================================*/
kvs::Real32 Axis3D::axisWidth() const
{
    return m_axis_width;
}

/*===========================================================================*/
/**
 *  @brief  Returns the grid-draw mode.
 *  @return grid-draw mode
 */
/*===========================================================================*/
Axis3D::GridDrawMode Axis3D::gridDrawMode() const
{
    return m_grid_draw_mode;
}

/*===========================================================================*/
/**
 *  @brief  Returns the gridline pattern.
 *  @return gridline pattern
 */
/*===========================================================================*/
Axis3D::GridlinePattern Axis3D::gridlinePattern() const
{
    return m_gridline_pattern;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of gridlines.
 *  @return number of gridlines
 */
/*===========================================================================*/
const kvs::Vector3ui& Axis3D::numberOfGridlines() const
{
    return m_ngridlines;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of gridlines along the X axis.
 *  @return number of gridlines
 */
/*===========================================================================*/
size_t Axis3D::numberOfXGridlines() const
{
    return m_ngridlines.x();
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of gridlines along the Y axis.
 *  @return number of gridlines
 */
/*===========================================================================*/
size_t Axis3D::numberOfYGridlines() const
{
    return m_ngridlines.y();
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of gridlines along the Z axis.
 *  @return number of gridlines
 */
/*===========================================================================*/
size_t Axis3D::numberOfZGridlines() const
{
    return m_ngridlines.z();
}

/*===========================================================================*/
/**
 *  @brief  Returns the gridline color.
 *  @return gridline color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis3D::gridlineColor() const
{
    return m_gridline_color;
}

/*===========================================================================*/
/**
 *  @brief  Returns the gridline width.
 *  @return gridline width
 */
/*===========================================================================*/
kvs::Real32 Axis3D::gridlineWidth() const
{
    return m_gridline_width;
}

/*===========================================================================*/
/**
 *  @brief  Returns the label of the X axis.
 *  @return label of the X axis
 */
/*===========================================================================*/
const std::string& Axis3D::xLabel() const
{
    return m_x_label;
}

/*===========================================================================*/
/**
 *  @brief  Returns the label of the Y axis.
 *  @return label of the Y axis
 */
/*===========================================================================*/
const std::string& Axis3D::yLabel() const
{
    return m_y_label;
}

/*===========================================================================*/
/**
 *  @brief  Returns the label of the Z axis.
 *  @return label of the Z axis
 */
/*===========================================================================*/
const std::string& Axis3D::zLabel() const
{
    return m_z_label;
}

/*===========================================================================*/
/**
 *  @brief  Returns the label color.
 *  @return label color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis3D::labelColor() const
{
    return m_label_color;
}

/*===========================================================================*/
/**
 *  @brief  Returns the value color.
 *  @return value color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis3D::valueColor() const
{
    return m_value_color;
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
void Axis3D::draw_xy_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_z )
{
    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b() );
    glLineWidth( m_axis_width );

    glBegin( GL_LINE_LOOP );
    glVertex3f( min_coord.x(), min_coord.y(), depth_z );
    glVertex3f( max_coord.x(), min_coord.y(), depth_z );
    glVertex3f( max_coord.x(), max_coord.y(), depth_z );
    glVertex3f( min_coord.x(), max_coord.y(), depth_z );
    glEnd();

    glPopAttrib();
}

/*===========================================================================*/
/**
 *  @brief  Draws the axes (border lines) on the YZ plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_x [in] X coordinate value of the YZ plane
 */
/*===========================================================================*/
void Axis3D::draw_yz_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_x )
{
    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b() );
    glLineWidth( m_axis_width );

    glBegin( GL_LINE_LOOP );
    glVertex3f( depth_x, min_coord.y(), min_coord.z() );
    glVertex3f( depth_x, max_coord.y(), min_coord.z() );
    glVertex3f( depth_x, max_coord.y(), max_coord.z() );
    glVertex3f( depth_x, min_coord.y(), max_coord.z() );
    glEnd();

    glPopAttrib();
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
    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b() );
    glLineWidth( m_axis_width );

    glBegin( GL_LINE_LOOP );
    glVertex3f( min_coord.x(), depth_y, min_coord.z() );
    glVertex3f( min_coord.x(), depth_y, max_coord.z() );
    glVertex3f( max_coord.x(), depth_y, max_coord.z() );
    glVertex3f( max_coord.x(), depth_y, min_coord.z() );
    glEnd();

    glPopAttrib();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the XY plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_z [in] Z coordinate value of the XY plane
 */
/*===========================================================================*/
void Axis3D::draw_xy_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_z )
{
    if ( !m_show_gridlines ) return;

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_gridline_color.r(), m_gridline_color.g(), m_gridline_color.b() );
    glLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 1, 0xF0F0 );
    }

    // X lines.
    glBegin( GL_LINES );
    kvs::Real32 x = min_coord.x();
    const kvs::Real32 x_interval = ( max_coord.x() - min_coord.x() ) / ( m_ngridlines.x() + 1 );
    for ( size_t i = 0; i < m_ngridlines.x(); i++ )
    {
        x += x_interval;
        glVertex3f( x, min_coord.y(), depth_z );
        glVertex3f( x, max_coord.y(), depth_z );
    }
    glEnd();

    // Y lines.
    glBegin( GL_LINES );
    kvs::Real32 y = min_coord.y();
    const kvs::Real32 y_interval = ( max_coord.y() - min_coord.y() ) / ( m_ngridlines.y() + 1 );
    for ( size_t i = 0; i < m_ngridlines.y(); i++ )
    {
        y += y_interval;
        glVertex3f( min_coord.x(), y, depth_z );
        glVertex3f( max_coord.x(), y, depth_z );
    }
    glEnd();

    glPopAttrib();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the YZ plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_x [in] X coordinate value of the YZ plane
 */
/*===========================================================================*/
void Axis3D::draw_yz_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_x )
{
    if ( !m_show_gridlines ) return;

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_gridline_color.r(), m_gridline_color.g(), m_gridline_color.b() );
    glLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 1, 0xF0F0 );
    }

    // Y lines.
    glBegin( GL_LINES );
    kvs::Real32 y = min_coord.z();
    const kvs::Real32 y_interval = ( max_coord.y() - min_coord.y() ) / ( m_ngridlines.y() + 1 );
    for ( size_t i = 0; i < m_ngridlines.y(); i++ )
    {
        y += y_interval;
        glVertex3f( depth_x, y, min_coord.z() );
        glVertex3f( depth_x, y, max_coord.z() );
    }
    glEnd();

    // Z lines.
    glBegin( GL_LINES );
    kvs::Real32 z = min_coord.z();
    const kvs::Real32 z_interval = ( max_coord.z() - min_coord.z() ) / ( m_ngridlines.z() + 1 );
    for ( size_t i = 0; i < m_ngridlines.z(); i++ )
    {
        z += z_interval;
        glVertex3f( depth_x, min_coord.y(), z );
        glVertex3f( depth_x, max_coord.y(), z );
    }
    glEnd();

    glPopAttrib();
}

/*===========================================================================*/
/**
 *  @brief  Draws the gridlines on the ZX plane.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 *  @param  depth_y [in] Y coordinate value of the ZX plane
 */
/*===========================================================================*/
void Axis3D::draw_zx_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_y )
{
    if ( !m_show_gridlines ) return;

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glColor3ub( m_gridline_color.r(), m_gridline_color.g(), m_gridline_color.b() );
    glLineWidth( m_gridline_width );

    if ( m_gridline_pattern == DashedLine )
    {
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 1, 0xF0F0 );
    }

    // Z lines.
    glBegin( GL_LINES );
    kvs::Real32 z = min_coord.z();
    const kvs::Real32 z_interval = ( max_coord.z() - min_coord.z() ) / ( m_ngridlines.z() + 1 );
    for ( size_t i = 0; i < m_ngridlines.z(); i++ )
    {
        z += z_interval;
        glVertex3f( min_coord.x(), depth_y, z );
        glVertex3f( max_coord.x(), depth_y, z );
    }
    glEnd();

    // X lines.
    glBegin( GL_LINES );
    kvs::Real32 x = min_coord.x();
    const kvs::Real32 x_interval = ( max_coord.x() - min_coord.x() ) / ( m_ngridlines.x() + 1 );
    for ( size_t i = 0; i < m_ngridlines.x(); i++ )
    {
        x += x_interval;
        glVertex3f( x, depth_y, min_coord.z() );
        glVertex3f( x, depth_y, max_coord.z() );
    }
    glEnd();

    glPopAttrib();
}

/*===========================================================================*/
/**
 *  @brief  Draws the labels and values along each axis.
 *  @param  min_coord [in] minimum coordinate value in the object coordinate
 *  @param  max_coord [in] maximum coordinate value in the object coordinate
 */
/*===========================================================================*/
void Axis3D::draw_labels( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord )
{
    const kvs::Vector3f center = ( max_coord + min_coord ) * 0.5f;

    // Depth values (Z values) in the camera coordinate system.
    GLfloat m[16]; glGetFloatv( GL_MODELVIEW_MATRIX, m );
    const kvs::Real32 x_min = min_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
    const kvs::Real32 x_max = max_coord.x() * m[2] + center.y() * m[6] + center.z() * m[10] + m[14];
    const kvs::Real32 y_min = center.x() * m[2] + min_coord.y() * m[6] + center.z() * m[10] + m[14];
    const kvs::Real32 y_max = center.x() * m[2] + max_coord.y() * m[6] + center.z() * m[10] + m[14];
    const kvs::Real32 z_min = center.x() * m[2] + center.y() * m[6] + min_coord.z() * m[10] + m[14];
    const kvs::Real32 z_max = center.x() * m[2] + center.y() * m[6] + max_coord.z() * m[10] + m[14];

    kvs::Vector3f min_value = min_coord;
    kvs::Vector3f max_value = max_coord;
    kvs::Vector3f dvalue = max_value - min_value;
    dvalue.x() /= static_cast<float>( m_ngridlines.x() + 1 );
    dvalue.y() /= static_cast<float>( m_ngridlines.y() + 1 );
    dvalue.z() /= static_cast<float>( m_ngridlines.z() + 1 );

    kvs::Vector3f interval = max_value - min_value;
    interval.x() /= static_cast<float>( m_ngridlines.x() + 1 );
    interval.y() /= static_cast<float>( m_ngridlines.y() + 1 );
    interval.z() /= static_cast<float>( m_ngridlines.z() + 1 );

    const kvs::Vector3f diff( max_coord - min_coord );
    const kvs::Real32 length = kvs::Math::Max( diff.x(), diff.y(), diff.z() );
    const kvs::Real32 label_offset = length * 0.1f;
    const kvs::Real32 value_offset = length * 0.05f;

    // Draw X label and values.
    {
        kvs::Vector3f position( center.x(), min_coord.y(), min_coord.z() );
        if ( m_grid_draw_mode == FarGrid )
        {
            position.y() = y_min <= y_max ? min_coord.y() : max_coord.y();
            position.z() = z_min <= z_max ? max_coord.z() : min_coord.z();
        }

        // Label.
        if ( m_show_labels )
        {
            kvs::Vector3f offset( 0.0f, -label_offset, -label_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.y() = y_min <= y_max ? -label_offset :  label_offset;
                offset.z() = z_min <= z_max ?  label_offset : -label_offset;
            }

            glColor3ub( m_label_color.r(), m_label_color.g(), m_label_color.b() );

            const kvs::Vector3f label = position + offset;
            glRasterPos3f( label.x(), label.y(), label.z() );
            char* head = const_cast<char*>( m_x_label.c_str() );
            for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vector3f offset( 0.0f, -value_offset, -value_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.y() = y_min <= y_max ? -value_offset :  value_offset;
                offset.z() = z_min <= z_max ?  value_offset : -value_offset;
            }

            glColor3ub( m_value_color.r(), m_value_color.g(), m_value_color.b() );

            const size_t nlines = size_t( m_ngridlines.x() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.x() + dvalue.x() * i );
                const float x = min_coord.x() + interval.x() * i;
                const float y = position.y() + offset.y();
                const float z = position.z() + offset.z();
                glRasterPos3f( x, y, z );
                char* line_head = const_cast<char*>( number.c_str() );
                for( char* p = line_head; *p; p++ )
                {
                    glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
                }
            }
        }
    }

    // Draw Y label and values.
    {
        kvs::Vector3f position( min_coord.x(), center.y(), min_coord.z() );
        if ( m_grid_draw_mode == FarGrid )
        {
            position.x() = x_min <= x_max ? min_coord.x() : max_coord.x();
            position.z() = z_min <= z_max ? max_coord.z() : min_coord.z();
        }

        // Label.
        if ( m_show_labels )
        {
            kvs::Vector3f offset( -label_offset, 0.0f, -label_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ? -label_offset :  label_offset;
                offset.z() = z_min <= z_max ?  label_offset : -label_offset;
            }

            glColor3ub( m_label_color.r(), m_label_color.g(), m_label_color.b() );

            const kvs::Vector3f label = position + offset;
            glRasterPos3f( label.x(), label.y(), label.z() );
            char* head = const_cast<char*>( m_y_label.c_str() );
            for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vector3f offset( -value_offset, 0.0f, -value_offset );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ? -value_offset :  value_offset;
                offset.z() = z_min <= z_max ?  value_offset : -value_offset;
            }

            glColor3ub( m_value_color.r(), m_value_color.g(), m_value_color.b() );

            const size_t nlines = size_t( m_ngridlines.y() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.y() + dvalue.y() * i );
                const float x = position.x() + offset.x();
                const float y = min_coord.y() + interval.y() * i;
                const float z = position.z() + offset.y();
                glRasterPos3f( x, y, z );
                char* head = const_cast<char*>( number.c_str() );
                for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
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

        // Label.
        if ( m_show_labels )
        {
            kvs::Vector3f offset( -label_offset, -label_offset, 0.0f );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ?  label_offset : -label_offset;
                offset.y() = y_min <= y_max ? -label_offset :  label_offset;
            }

            glColor3ub( m_label_color.r(), m_label_color.g(), m_label_color.b() );

            const kvs::Vector3f label = position + offset;
            glRasterPos3f( label.x(), label.y(), label.z() );
            char* head = const_cast<char*>( m_z_label.c_str() );
            for ( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
        }

        // Values.
        if ( m_show_values )
        {
            kvs::Vector3f offset( -value_offset, -value_offset, 0.0f );
            if ( m_grid_draw_mode == FarGrid )
            {
                offset.x() = x_min <= x_max ?  value_offset : -value_offset;
                offset.y() = y_min <= y_max ? -value_offset :  value_offset;
            }

            glColor3ub( m_label_color.r(), m_value_color.g(), m_value_color.b() );

            const size_t nlines = size_t( m_ngridlines.z() ) + 2;
            for ( size_t i = 0; i < nlines; i++ )
            {
                std::string number = kvs::String::ToString( min_value.z() + dvalue.z() * i );
                const float x = position.x() + offset.x();
                const float y = position.y() + offset.y();
                const float z = min_coord.z() + interval.z() * i;
                glRasterPos3f( x, y, z );
                char* head = const_cast<char*>( number.c_str() );
                for( char* p = head; *p; p++ ) { glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p ); }
            }
        }
    }
}

} // end of namespace glut

} // end of namespace kvs
