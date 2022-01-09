/*****************************************************************************/
/**
 *  @file   ParallelAxisInteractor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ParallelAxisInteractor.h"
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectManager>
#include <kvs/Camera>
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParallelAxisInteractor class.
 *  @param  axis [in] parallel axis
 */
/*===========================================================================*/
ParallelAxisInteractor::ParallelAxisInteractor( const kvs::ParallelAxis* axis ):
    m_axis( axis )
{
    BaseClass::addEventType( kvs::EventBase::MousePressEvent );
    BaseClass::addEventType( kvs::EventBase::MouseMoveEvent );
    BaseClass::addEventType( kvs::EventBase::MouseReleaseEvent );
    BaseClass::addEventType( kvs::EventBase::MouseDoubleClickEvent );
    BaseClass::addEventType( kvs::EventBase::PaintEvent );
}

/*===========================================================================*/
/**
 *  @brief  Returns pointer to the table object.
 *  @return pointer to the table object
 */
/*===========================================================================*/
kvs::TableObject* ParallelAxisInteractor::table()
{
    if ( m_table ) { return m_table; }
    const auto rid = BaseClass::scene()->rendererManager()->rendererID( m_axis );
    const auto oid = BaseClass::scene()->IDManager()->objectID( rid )[0];
    auto* object = BaseClass::scene()->objectManager()->object( oid );
    return m_table = kvs::TableObject::DownCast( object );
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void ParallelAxisInteractor::mousePressEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    // x_min: left side of the content region
    // x_max: right side of the content region
    const auto width = BaseClass::screen()->width();
    const auto x_min = m_axis->margins().left();
    const auto x_max = width - m_axis->margins().right();
    const float cw = x_max - x_min; // content width

    // Update mouse pressing position.
    m_p0 = { e->x(), e->y() };
    m_p = m_p0;

    // Select the axis and determin the action (moving or setting)
    // for its range bar
    const float w = m_range_width * 0.5f;
    const float w2 = w * 0.5f;
    const float ex = static_cast<float>( e->x() );
    const auto stride = cw / ( naxes - 1 );
    auto x = static_cast<float>( x_min );
    for ( size_t i = 0; i < naxes; ++i, x += stride )
    {
        if ( x - w2 < ex && ex < x + w2 )
        {
            // Moving the range
            m_selected_axis = i;
            m_range_moving = true;
        }
        else if ( x - w < ex && ex < x + w )
        {
            // Selecting the range
            m_selected_axis = i;
            m_range_setting = true;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void ParallelAxisInteractor::mouseMoveEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    // y_min: top side of the content region
    // y_max: bottom side of the content region
    const auto height = BaseClass::screen()->height();
    const auto y_min = m_axis->margins().top();
    const auto y_max = height - m_axis->margins().bottom();
    const double ch = y_max - y_min; // content height

    // Update mouse moving position.
    m_p1 = { e->x(), e->y() };
    kvs::Math::Clamp( m_p1.y(), int( y_min ), int( y_max ) );

    const auto axis = m_selected_axis;
    const auto v_min = table->minValue( axis );
    const auto v_max = table->maxValue( axis );
    const auto dv = v_max - v_min;

    if ( m_range_moving )
    {
        const auto dy = m_p1.y() - m_p.y();
        const auto v = ( dv * -dy ) / ch;
        table->moveRange( axis, v );
        m_p = m_p1;
    }

    if ( m_range_setting )
    {
        const auto y0 = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto y1 = kvs::Math::Max( m_p0.y(), m_p1.y() );
        const auto v0 = v_min + ( dv * y_max - dv * y1 ) / ch;
        const auto v1 = v_min + ( dv * y_max - dv * y0 ) / ch;
        table->setMinRange( axis, v0 );
        table->setMaxRange( axis, v1 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void ParallelAxisInteractor::mouseReleaseEvent( kvs::MouseEvent* e )
{
    m_range_moving = false;
    m_range_setting = false;
}

/*===========================================================================*/
/**
 *  @brief  Mouse double-click event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void ParallelAxisInteractor::mouseDoubleClickEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    // x_min: left side of the content region
    // x_max: right side of the content region
    const auto width = BaseClass::screen()->width();
    const auto x_min = m_axis->margins().left();
    const auto x_max = width - m_axis->margins().right();
    const float cw = x_max - x_min; // content width

    const float w = m_range_width * 0.5f;
    const float w2 = w * 0.5f;
    const auto ex = static_cast<float>( e->x() );
    const auto stride = cw / ( naxes - 1 );
    auto x = static_cast<float>( x_min );
    for ( size_t i = 0; i < naxes; i++, x += stride )
    {
        // Reset the range of the axis clicked.
        if ( x - w2 < ex && ex < x + w2 )
        {
            table->resetRange(i);
            return;
        }
    }

    // Reset all ranges when clicking on the backgground.
    table->resetRange();

    m_range_setting = false;
    m_range_moving = false;
}

/*===========================================================================*/
/**
 *  @brief  Paint event function.
 */
/*===========================================================================*/
void ParallelAxisInteractor::paintEvent()
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    // Draw range rectangles for each axis.
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    m_painter.begin( BaseClass::screen() );
    {
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const auto dpr = BaseClass::scene()->camera()->devicePixelRatio();
        const auto width = BaseClass::screen()->width();
        const auto height = BaseClass::screen()->height();
        const auto content = m_axis->margins().content( width, height );
        const double cw = content.width();
        const double ch = content.height();

        const double stride = cw / ( naxes - 1 );
        const double w = m_range_width * 0.5f;
        auto x = static_cast<double>( content.x0() );
        for ( size_t i = 0; i < naxes; ++i, x += stride )
        {
            // Min/max values for the axis.
            const auto v_min = table->minValue(i);
            const auto v_max = table->maxValue(i);
            const auto dv = v_max - v_min;

            // Value ranges for the axis.
            const auto v0 = table->minRange(i);
            const auto v1 = table->maxRange(i);

            // Range rectangle for the axis.
            const auto y0 = content.y0() + ( ch * v_max - ch * v1 ) / dv;
            const auto y1 = content.y0() + ( ch * v_max - ch * v0 ) / dv;

            // Draw background of the rectangle.
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Color( m_range_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x - w, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x + w, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x + w, y1 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x - w, y1 ) * dpr );
            kvs::OpenGL::End();

            // Draw edges of the rectangle.
            kvs::OpenGL::SetLineWidth( m_range_edge_width * dpr );
            kvs::OpenGL::Begin( GL_LINE_LOOP );
            kvs::OpenGL::Color( m_range_edge_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x - w, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x + w, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x + w, y1 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x - w, y1 ) * dpr );
            kvs::OpenGL::End();
        }
    }
    m_painter.end();
}

} // end of namespace kvs
