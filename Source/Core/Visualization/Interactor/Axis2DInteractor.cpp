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
 *  @brief  Returns a position of the point in window coordinates (origin: bottom-left)
 *  @param  p [in] position of the point in pixel coordinates (orgin: top-left)
 *  @return position of the point in window coordinates (origin: bottom-left)
 */
/*===========================================================================*/
kvs::Vec2i Axis2DInteractor::toWindowCoordinates( const kvs::Vec2i& p )
{
    return { p.x(), BaseClass::screen()->height() - 1 - p.y() };
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

    m_p0 = { e->x(), e->y() };
    m_p = m_p0;

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );

    const size_t x_index = 0;
    const size_t y_index = 1;

    const auto x_min = table->minValue( x_index );
    const auto x_max = table->maxValue( x_index );
    const auto y_min = table->minValue( y_index );
    const auto y_max = table->maxValue( y_index );
    const auto dx = x_max - x_min;
    const auto dy = y_max - y_min;

    const auto x0 = x_min + dx * ( m_p0.x() - content.x0() ) / content.width();
    const auto y0 = y_min + dy * ( content.y1() - m_p0.y() ) / content.height();

    const kvs::Range x_range( table->minRange( x_index ), table->maxRange( x_index ) );
    const kvs::Range y_range( table->minRange( y_index ), table->maxRange( y_index ) );

    m_range_setting = true;
    m_range_moving = false;

    if ( x_range.equals( { x_min, x_max } ) && y_range.equals( { y_min, y_max } ) )
    {
        return;
    }

    if ( x_range.contains( x0 ) && y_range.contains( y0 ) )
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

    m_p1 = { e->x(), e->y() };

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );
    kvs::Math::Clamp( m_p1.x(), content.x0(), content.x1() );
    kvs::Math::Clamp( m_p1.y(), content.y0(), content.y1() );

    const size_t x_index = 0;
    const size_t y_index = 1;

    const auto x_min = table->minValue( x_index );
    const auto x_max = table->maxValue( x_index );
    const auto y_min = table->minValue( y_index );
    const auto y_max = table->maxValue( y_index );
    const auto dx = x_max - x_min;
    const auto dy = y_max - y_min;

    if ( m_range_moving )
    {
        const auto D = m_p1 - m_p;
        const auto x_delta = dx * D.x() / content.width();
        const auto y_delta = dy * -D.y() / content.height();
        table->moveRange( x_index, x_delta );
        table->moveRange( y_index, y_delta );
        m_p = m_p1;
    }
    else
    {
        const auto X_rect_min = kvs::Math::Min( m_p0.x(), m_p1.x() );
        const auto X_rect_max = kvs::Math::Max( m_p0.x(), m_p1.x() );
        const auto Y_rect_min = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto Y_rect_max = kvs::Math::Max( m_p0.y(), m_p1.y() );

        const auto x_rect_min = x_min + dx * ( X_rect_min - content.x0() ) / content.width();
        const auto x_rect_max = x_min + dx * ( X_rect_max - content.x0() ) / content.width();
        const auto y_rect_min = y_min + dy * ( content.y1() - Y_rect_max ) / content.height();
        const auto y_rect_max = y_min + dy * ( content.y1() - Y_rect_min ) / content.height();

        table->setMinRange( x_index, x_rect_min );
        table->setMaxRange( x_index, x_rect_max );
        table->setMinRange( y_index, y_rect_min );
        table->setMaxRange( y_index, y_rect_max );
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

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    m_painter.begin( BaseClass::screen() );
    {
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // Draw range bars for each axis.
        const auto dpr = BaseClass::scene()->camera()->devicePixelRatio();
        const auto width = BaseClass::screen()->width();
        const auto height = BaseClass::screen()->height();
        const auto content = m_axis->margins().content( width, height );

        const size_t x_index = 0;
        const size_t y_index = 1;

        const auto x_min_range = table->minRange( x_index );
        const auto x_max_range = table->maxRange( x_index );
        const auto y_min_range = table->minRange( y_index );
        const auto y_max_range = table->maxRange( y_index );

        const auto x_min = table->minValue( x_index );
        const auto x_max = table->maxValue( x_index );
        const auto y_min = table->minValue( y_index );
        const auto y_max = table->maxValue( y_index );
        const auto dx = x_max - x_min;
        const auto dy = y_max - y_min;

        const auto X_min_range = content.x0() + content.width() * ( x_max_range - x_min ) / dx;
        const auto X_max_range = content.x0() + content.width() * ( x_min_range - x_min ) / dx;
        const auto Y_min_range = content.y0() + content.height() * ( y_max - y_max_range ) / dy;
        const auto Y_max_range = content.y0() + content.height() * ( y_max - y_min_range ) / dy;

        // Range box
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_range_color );
        kvs::OpenGL::Vertex( kvs::Vec2( X_min_range, Y_min_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_max_range, Y_min_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_max_range, Y_max_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_min_range, Y_max_range ) * dpr );
        kvs::OpenGL::End();

        // Range edge
        kvs::OpenGL::SetLineWidth( m_range_edge_width * dpr );
        kvs::OpenGL::Begin( GL_LINE_LOOP );
        kvs::OpenGL::Color( m_range_edge_color );
        kvs::OpenGL::Vertex( kvs::Vec2( X_min_range, Y_min_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_max_range, Y_min_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_max_range, Y_max_range ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( X_min_range, Y_max_range ) * dpr );
        kvs::OpenGL::End();
    }
    m_painter.end();
}

} // end of namespace kvs
