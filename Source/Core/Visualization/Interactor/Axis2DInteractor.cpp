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
 *  @brief  Returns a position of the point in viewport coordinates (origin: bottom-left)
 *  @param  p [in] position of the point in pixel coordinates (orgin: top-left)
 *  @return position of the point in viewport coordinates (origin: bottom-left)
 */
/*===========================================================================*/
kvs::Vec2i Axis2DInteractor::toBottomLeftOrigin( const kvs::Vec2i& p )
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

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    m_p0 = this->toBottomLeftOrigin( { e->x(), e->y() } );
    m_p = m_p0;

    // X_min: left side of the plot region
    // X_max: right side of the plot region
    // Y_min: bottom side of the plot region
    // Y_max: top side of the plot region
    const auto X_min = m_axis->margins().left();
    const auto X_max = BaseClass::screen()->width() - m_axis->margins().right();
    const auto Y_min = m_axis->margins().top();
    const auto Y_max = BaseClass::screen()->height() - m_axis->margins().bottom();

    const auto x_min_value = table->minValue(0);
    const auto x_max_value = table->maxValue(0);
    const auto y_min_value = table->minValue(1);
    const auto y_max_value = table->maxValue(1);

    const auto x_value_width = x_max_value - x_min_value;
    const auto y_value_width = y_max_value - y_min_value;

    const auto x_value = x_min_value + x_value_width * ( m_p0.x() - X_min ) / ( X_max - X_min );
    const auto y_value = y_min_value + y_value_width * ( m_p0.y() - Y_min ) / ( Y_max - Y_min );

    const auto x_min_range = table->minRange(0);
    const auto x_max_range = table->maxRange(0);
    const auto y_min_range = table->minRange(1);
    const auto y_max_range = table->maxRange(1);

    m_range_setting = true;
    m_range_moving = false;
    if ( kvs::Math::Equal( x_min_range, x_min_value ) &&
         kvs::Math::Equal( x_max_range, x_max_value ) &&
         kvs::Math::Equal( y_min_range, y_min_value ) &&
         kvs::Math::Equal( y_max_range, y_max_value ) )
    {
        return;
    }
    else
    {
        if ( x_min_range <= x_value && x_value <= x_max_range &&
             y_min_range <= y_value && y_value <= y_max_range )
        {
            m_range_moving = true;
        }
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

    const auto naxes = table->numberOfColumns();
    if ( naxes < 2 ) { return; }

    m_p1 = this->toBottomLeftOrigin( { e->x(), e->y() } );

    // X_min: left side of the plot region
    // X_max: right side of the plot region
    // Y_min: bottom side of the plot region
    // Y_max: top side of the plot region
    const auto X_min = m_axis->margins().left();
    const auto X_max = BaseClass::screen()->width() - m_axis->margins().right();
    const auto Y_min = m_axis->margins().top();
    const auto Y_max = BaseClass::screen()->height() - m_axis->margins().bottom();

    // Adjust the mouse moving position.
    if ( m_p1.x() <= X_min ) { m_p1.x() = X_min; }
    if ( m_p1.x() >= X_max ) { m_p1.x() = X_max; }
    if ( m_p1.y() <= Y_min ) { m_p1.y() = Y_min; }
    if ( m_p1.y() >= Y_max ) { m_p1.y() = Y_max; }

    const auto x_min = table->minValue(0);
    const auto x_max = table->maxValue(0);
    const auto y_min = table->minValue(1);
    const auto y_max = table->maxValue(1);

    if ( m_range_moving )
    {
        const auto X_delta = m_p1.x() - m_p.x();
        const auto Y_delta = m_p1.y() - m_p.y();
        const auto x_delta = ( x_max - x_min ) * X_delta / ( X_max - X_min );
        const auto y_delta = ( y_max - y_min ) * Y_delta / ( Y_max - Y_min );
        table->moveRange( 0, x_delta );
        table->moveRange( 1, y_delta );
        m_p = m_p1;
    }
    else
    {
        const auto X_rect_min = kvs::Math::Min( m_p0.x(), m_p1.x() );
        const auto X_rect_max = kvs::Math::Max( m_p0.x(), m_p1.x() );
        const auto Y_rect_min = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto Y_rect_max = kvs::Math::Max( m_p0.y(), m_p1.y() );
        const auto x_rect_min = x_min + ( x_max - x_min ) * ( X_rect_min - X_min ) / ( X_max - X_min );
        const auto x_rect_max = x_min + ( x_max - x_min ) * ( X_rect_max - X_min ) / ( X_max - X_min );
        const auto y_rect_min = y_min + ( y_max - y_min ) * ( Y_rect_min - Y_min ) / ( Y_max - Y_min );
        const auto y_rect_max = y_min + ( y_max - y_min ) * ( Y_rect_max - Y_min ) / ( Y_max - Y_min );
        table->setMinRange( 0, x_rect_min );
        table->setMaxRange( 0, x_rect_max );
        table->setMinRange( 1, y_rect_min );
        table->setMaxRange( 1, y_rect_max );
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
        const auto X_min = m_axis->margins().left();
        const auto X_max = BaseClass::screen()->width() - m_axis->margins().right();
        const auto Y_min = m_axis->margins().top();
        const auto Y_max = BaseClass::screen()->height() - m_axis->margins().bottom();

        const auto x_min_range = table->minRange(0);
        const auto x_max_range = table->maxRange(0);
        const auto y_min_range = table->minRange(1);
        const auto y_max_range = table->maxRange(1);

        const auto x_min_value = table->minValue(0);
        const auto x_max_value = table->maxValue(0);
        const auto y_min_value = table->minValue(1);
        const auto y_max_value = table->maxValue(1);
        const auto X_min_range = X_min + ( X_max - X_min ) * ( x_max_range - x_min_value ) / ( x_max_value - x_min_value );
        const auto X_max_range = X_min + ( X_max - X_min ) * ( x_min_range - x_min_value ) / ( x_max_value - x_min_value );
        const auto Y_min_range = Y_max - ( Y_max - Y_min ) * ( y_max_range - y_min_value ) / ( y_max_value - y_min_value );
        const auto Y_max_range = Y_max - ( Y_max - Y_min ) * ( y_min_range - y_min_value ) / ( y_max_value - y_min_value );

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
