/*****************************************************************************/
/**
 *  @file   Axis2DInteractor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Axis2DInteractor.h"
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectManager>
#include <kvs/Camera>
#include <kvs/Range>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis2DInteractor class.
 *  @param  axis [in] axis 2D axis
 */
/*===========================================================================*/
Axis2DInteractor::Axis2DInteractor( const kvs::Axis2D* axis ):
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
kvs::TableObject* Axis2DInteractor::table()
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
void Axis2DInteractor::mousePressEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

    const size_t x_index = 0;
    const size_t y_index = 1;

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );

    const auto vx_min = table->minValue( x_index );
    const auto vx_max = table->maxValue( x_index );
    const auto vy_min = table->minValue( y_index );
    const auto vy_max = table->maxValue( y_index );
    const auto dvx = vx_max - vx_min;
    const auto dvy = vy_max - vy_min;

    m_p0 = { e->x(), e->y() };
    m_p = m_p0;

    const auto vx = vx_min + dvx * ( m_p0.x() - content.x0() ) / content.width();
    const auto vy = vy_min + dvy * ( content.y1() - m_p0.y() ) / content.height();

    const kvs::Range vx_range( table->minRange( x_index ), table->maxRange( x_index ) );
    const kvs::Range vy_range( table->minRange( y_index ), table->maxRange( y_index ) );

    m_range_setting = true;
    m_range_moving = false;

    if ( vx_range.equals( { vx_min, vx_max } ) && vy_range.equals( { vy_min, vy_max } ) )
    {
        return;
    }

    if ( vx_range.contains( vx ) && vy_range.contains( vy ) )
    {
        m_range_moving = true;
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void Axis2DInteractor::mouseMoveEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

    const size_t x_index = 0;
    const size_t y_index = 1;

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );

    const auto vx_min = table->minValue( x_index );
    const auto vx_max = table->maxValue( x_index );
    const auto vy_min = table->minValue( y_index );
    const auto vy_max = table->maxValue( y_index );
    const auto dvx = vx_max - vx_min;
    const auto dvy = vy_max - vy_min;

    m_p1 = { e->x(), e->y() };
    kvs::Math::Clamp( m_p1.x(), content.x0(), content.x1() );
    kvs::Math::Clamp( m_p1.y(), content.y0(), content.y1() );

    if ( m_range_moving )
    {
        const auto d = m_p1 - m_p;
        const auto vx = dvx * d.x() / content.width();
        const auto vy = dvy * -d.y() / content.height();
        table->moveRange( x_index, vx );
        table->moveRange( y_index, vy );
        m_p = m_p1;
    }
    else
    {
        const auto x0 = kvs::Math::Min( m_p0.x(), m_p1.x() );
        const auto x1 = kvs::Math::Max( m_p0.x(), m_p1.x() );
        const auto y0 = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto y1 = kvs::Math::Max( m_p0.y(), m_p1.y() );

        const auto vx0 = vx_min + dvx * ( x0 - content.x0() ) / content.width();
        const auto vx1 = vx_min + dvx * ( x1 - content.x0() ) / content.width();
        const auto vy0 = vy_min + dvy * ( content.y1() - y1 ) / content.height();
        const auto vy1 = vy_min + dvy * ( content.y1() - y0 ) / content.height();

        table->setMinRange( x_index, vx0 );
        table->setMaxRange( x_index, vx1 );
        table->setMinRange( y_index, vy0 );
        table->setMaxRange( y_index, vy1 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void Axis2DInteractor::mouseReleaseEvent( kvs::MouseEvent* e )
{
    m_range_setting = false;
    m_range_moving = false;
}

/*===========================================================================*/
/**
 *  @brief  Mouse double-click event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void Axis2DInteractor::mouseDoubleClickEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    table->resetRange();

    m_range_setting = false;
    m_range_moving = false;
}

/*===========================================================================*/
/**
 *  @brief  Paint event function.
 */
/*===========================================================================*/
void Axis2DInteractor::paintEvent()
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    if ( !m_range_setting ) { return; }

    // Draw range rectangle for each axis.
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    m_painter.begin( BaseClass::screen() );
    {
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const size_t x_index = 0;
        const size_t y_index = 1;

        const auto dpr = BaseClass::scene()->camera()->devicePixelRatio();
        const auto width = BaseClass::screen()->width();
        const auto height = BaseClass::screen()->height();
        const auto content = m_axis->margins().content( width, height );
        const float cw = content.width();
        const float ch = content.height();

        // Min/max values for each axis.
        const auto vx_min = table->minValue( x_index );
        const auto vx_max = table->maxValue( x_index );
        const auto vy_min = table->minValue( y_index );
        const auto vy_max = table->maxValue( y_index );
        const auto dvx = vx_max - vx_min;
        const auto dvy = vy_max - vy_min;

        // Value ranges for each axis.
        const auto vx0 = table->minRange( x_index );
        const auto vx1 = table->maxRange( x_index );
        const auto vy0 = table->minRange( y_index );
        const auto vy1 = table->maxRange( y_index );

        // Range rectangle.
        const auto x0 = content.x0() + cw * ( vx0 - vx_min ) / dvx;
        const auto x1 = content.x0() + cw * ( vx1 - vx_min ) / dvx;
        const auto y0 = content.y0() + ch * ( vy_max - vy1 ) / dvy;
        const auto y1 = content.y0() + ch * ( vy_max - vy0 ) / dvy;

        // Draw background of the rectangle.
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_range_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();

        // Draw edges of the rectangle.
        kvs::OpenGL::SetLineWidth( m_range_edge_width * dpr );
        kvs::OpenGL::Begin( GL_LINE_LOOP );
        kvs::OpenGL::Color( m_range_edge_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }
    m_painter.end();
}

} // end of namespace kvs
