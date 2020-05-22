/*****************************************************************************/
/**
 *  @file   Axis2D.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Axis2D.h"
#include "ValueAxis.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/TableObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis2D class.
 */
/*===========================================================================*/
Axis2D::Axis2D():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_title( "" ),
    m_title_font(),
    m_title_offset( 5 ),
    m_background_color( kvs::RGBColor::White() ),
    m_borderline_color( 0, 0, 0, 0.0f ),
    m_borderline_width( 0.0f ),
    m_x_axis( new kvs::ValueAxis( kvs::ValueAxis::Bottom ) ),
    m_y_axis( new kvs::ValueAxis( kvs::ValueAxis::Left ) )
{
    m_title_font.setStyleToBold();
    m_title_font.setSize( 22 );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Axis2D class.
 */
/*===========================================================================*/
Axis2D::~Axis2D()
{
    if ( m_x_axis ) { delete m_x_axis; }
    if ( m_y_axis ) { delete m_y_axis; }
}

/*===========================================================================*/
/**
 *  @brief  Render 2D axes.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void Axis2D::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        //        x0                 x1
        //    +--------------------------+
        //    |   |                  |   |
        // y0 |---+------------------+---|
        //    |   |                  |   |
        //    |   |                  |   |
        //    |   |                  |   |
        //    |   |                  |   |
        //    |   |                  |   |
        // y1 |---+------------------+---|
        //    |   |                  |   |
        //    +--------------------------+
        //
        const float dpr = camera->devicePixelRatio();
        const float x0 = m_left_margin;
        const float x1 = camera->windowWidth() - m_right_margin;
        const float y0 = m_top_margin;
        const float y1 = camera->windowHeight() - m_bottom_margin;
        const kvs::Vec4 rect( x0, x1, y0, y1 );

        // Draw background and borderlines.
        this->draw_background( rect, dpr );
        this->draw_borderline( rect, dpr );

        // Draw x-axis.
        if ( m_x_axis->label().empty() ) { m_x_axis->setLabel( table->label(0) ); }
        m_x_axis->setRect( rect );
        m_x_axis->setRange( table->minValue(0), table->maxValue(0) );
        m_x_axis->draw( m_painter );

        // Draw y-axis.
        if ( m_y_axis->label().empty() ) { m_y_axis->setLabel( table->label(1) ); }
        m_y_axis->setRect( rect );
        m_y_axis->setRange( table->minValue(1), table->maxValue(1) );
        m_y_axis->draw( m_painter );

        // Draw title.
        this->draw_title( rect );
    }
    m_painter.end();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Draw the axis title
 *  @param  rect [in] plot region
 */
/*===========================================================================*/
void Axis2D::draw_title( const kvs::Vec4& rect )
{
    if ( m_title.size() > 0 )
    {
        const float x0 = rect[0];
        const float x1 = rect[1];
        const float y0 = rect[2];
        const kvs::FontMetrics& metrics = m_painter.fontMetrics();
        const kvs::Font font = m_painter.font();
        {
            m_painter.setFont( m_title_font );
            const float px = ( x0 + x1 - metrics.width( m_title ) ) * 0.5f;
            const float py = y0 - m_title_offset;
            m_painter.drawText( kvs::Vec2( px, py ), m_title );
        }
        m_painter.setFont( font );
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw background of the plot region.
 *  @param  rect [in] plot region
 *  @param  dpr [in] device pixel ratio
 */
/*===========================================================================*/
void Axis2D::draw_background( const kvs::Vec4& rect, const float dpr )
{
    if ( m_background_color.a() > 0.0f )
    {
        const float x0 = rect[0];
        const float x1 = rect[1];
        const float y0 = rect[2];
        const float y1 = rect[3];
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw borderline of the plot region.
 *  @param  rect [in] plot region
 *  @param  dpr [in] device pixel ratio
 */
/*===========================================================================*/
void Axis2D::draw_borderline( const kvs::Vec4& rect, const float dpr )
{
    if ( m_borderline_color.a() > 0.0f )
    {
        const float x0 = rect[0];
        const float x1 = rect[1];
        const float y0 = rect[2];
        const float y1 = rect[3];
        const int d = int( m_borderline_width * 0.5 );
        kvs::OpenGL::SetLineWidth( m_borderline_width * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_borderline_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y1 ) * dpr, kvs::Vec2( x1 + d, y1 ) * dpr ); // bottom
        kvs::OpenGL::Vertices( kvs::Vec2( x0, y1 + d ) * dpr, kvs::Vec2( x0, y0 - d ) * dpr ); // left
        kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y0 ) * dpr, kvs::Vec2( x1 + d, y0 ) * dpr ); // top
        kvs::OpenGL::Vertices( kvs::Vec2( x1, y1 + d ) * dpr, kvs::Vec2( x1, y0 - d ) * dpr ); // right
        kvs::OpenGL::End();
    }
}

} // end of namespace kvs
