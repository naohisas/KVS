/*****************************************************************************/
/**
 *  @file   ParallelCoordinatesRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ParallelCoordinatesRenderer.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/TableObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

using ThisClass = ParallelCoordinatesRenderer;

void ThisClass::setAntiAliasingEnabled( const bool aa, const bool msaa ) const
{
    m_enable_anti_aliasing = aa;
    m_enable_multisample_anti_aliasing = msaa;
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing mode.
 *  @param  multisample [in] if true, multisampling is available
 */
/*===========================================================================*/
void ThisClass::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing mode.
 */
/*===========================================================================*/
void ThisClass::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

/*===========================================================================*/
/**
 *  @brief  Updates color map range.
 *  @param  table [in] pointer to the table data
 */
/*===========================================================================*/
void ThisClass::updateColorMapRange( const kvs::TableObject* table )
{
    const auto color_axis_min_value = static_cast<float>( table->minValue( m_active_axis ) );
    const auto color_axis_max_value = static_cast<float>( table->maxValue( m_active_axis ) );
    m_color_map.setRange( color_axis_min_value, color_axis_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Updates anti-aliasing.
 */
/*===========================================================================*/
void ThisClass::updateAntiAliasing()
{
    if ( m_enable_anti_aliasing )
    {
#if defined ( GL_MULTISAMPLE )
        if ( m_enable_multisample_anti_aliasing )
        {
            GLint buffers = 0;
            GLint samples = 0;
            kvs::OpenGL::GetIntegerv( GL_SAMPLE_BUFFERS, &buffers );
            kvs::OpenGL::GetIntegerv( GL_SAMPLES, &samples );
            if ( buffers > 0 && samples > 1 ) kvs::OpenGL::Enable( GL_MULTISAMPLE );
        }
        else
#endif
        {
            kvs::OpenGL::Enable( GL_LINE_SMOOTH );
            kvs::OpenGL::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Render parallel coordinates.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer to light
 */
/*===========================================================================*/
void ThisClass::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    const auto* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    const float dpr = camera->devicePixelRatio();
    const int x0 = m_margins.left();
    const int x1 = camera->windowWidth() - m_margins.right();
    const int y0 = m_margins.top();
    const int y1 = camera->windowHeight() - m_margins.bottom();

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    this->updateColorMapRange( table );
    this->updateAntiAliasing();

    const size_t naxes = table->numberOfColumns();
    const float dx = float( x1 - x0 ) / ( naxes - 1 );

    auto y_normalize = [&] ( double v, double v_min, double v_max )
    {
        return y1 - ( y1 - y0 ) * ( v - v_min ) / ( v_max - v_min );
    };

    auto y_value = [&] ( const size_t axis, const size_t row )
    {
        const kvs::Real64 min_value = table->minValue( axis );
        const kvs::Real64 max_value = table->maxValue( axis );
        const kvs::Real64 value = table->column( axis )[row].to<kvs::Real64>();
        return y_normalize( value, min_value, max_value );
    };

    kvs::OpenGL::Render2D render( kvs::OpenGL::Viewport() );
    render.begin();
    {
        const auto& color_axis_values = table->column( m_active_axis );
        const size_t nrows = table->column(0).size();
        for ( size_t i = 0; i < nrows; ++i )
        {
            if ( !table->insideRange( i ) ) continue;

            kvs::OpenGL::SetLineWidth( m_line_width * dpr );
            kvs::OpenGL::Begin( GL_LINE_STRIP );
            {
                const auto color_value = color_axis_values[i].to<kvs::Real64>();
                const auto color = m_color_map.at( static_cast<float>( color_value ) );
                kvs::OpenGL::Color( kvs::RGBAColor( color, m_line_opacity ) );

                auto x = static_cast<kvs::Real64>( m_margins.left() );
                for ( size_t j = 0; j < naxes; ++j, x += dx )
                {
                    const auto p = kvs::Vec2( x, y_value( j, i ) );
                    kvs::OpenGL::Vertex( p * dpr );
                }
            }
            kvs::OpenGL::End();
        }
    }
    render.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
