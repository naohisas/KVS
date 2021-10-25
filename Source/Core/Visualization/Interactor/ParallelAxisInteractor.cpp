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
 *  @brief  Returns a position of the point in viewport coordinates (origin: bottom-left)
 *  @param  p [in] position of the point in pixel coordinates (orgin: top-left)
 *  @return position of the point in viewport coordinates (origin: bottom-left)
 */
/*===========================================================================*/
kvs::Vec2i ParallelAxisInteractor::toBottomLeftOrigin( const kvs::Vec2i& p )
{
    return { p.x(), BaseClass::screen()->height() - 1 - p.y() };
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

    m_p0 = this->toBottomLeftOrigin( { e->x(), e->y() } );
    m_p = m_p0;

    // X_min: left side of the plot region
    // X_max: right side of the plot region
    const int X_min = m_axis->leftMargin();
    const int X_max = BaseClass::screen()->width() - m_axis->rightMargin();
    const float stride = float( X_max - X_min ) / ( naxes - 1 );

    // Select the axis and determin the action (moving or setting) for its range bar
    for ( size_t i = 0; i < naxes; ++i )
    {
        const float ex = static_cast<float>( e->x() );
        const float x = m_axis->leftMargin() + stride * i;
        const float dx = m_range_width * 0.5f;
        const float ddx = dx * 0.5f;
        if ( x - ddx < ex && ex < x + ddx )
        {
            // Moving the range
            m_selected_axis = i;
            m_range_moving = true;
        }
        else if ( x - dx < ex && ex < x + dx )
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

    m_p1 = this->toBottomLeftOrigin( { e->x(), e->y() } );

    // Y_min: bottom side of the plot region
    // Y_max: top side of the plot region
    const int Y_min = m_axis->bottomMargin();
    const int Y_max = BaseClass::screen()->height() - m_axis->topMargin();

    // Adjust the mouse moving position.
    if ( m_p1.y() <= Y_min ) { m_p1.y() = Y_min; }
    if ( m_p1.y() >= Y_max ) { m_p1.y() = Y_max; }

    const auto axis = m_selected_axis;
    const auto y_min = table->minValue( axis );
    const auto y_max = table->maxValue( axis );

    if ( m_range_setting )
    {
        const auto Y_rect_min = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto Y_rect_max = kvs::Math::Max( m_p0.y(), m_p1.y() );
        const auto y_rect_min = y_min + ( y_max - y_min ) * ( Y_rect_min - Y_min ) / ( Y_max - Y_min );
        const auto y_rect_max = y_min + ( y_max - y_min ) * ( Y_rect_max - Y_min ) / ( Y_max - Y_min );
        table->setMinRange( axis, y_rect_min );
        table->setMaxRange( axis, y_rect_max );
    }

    if ( m_range_moving )
    {
        const auto Y_delta = m_p1.y() - m_p.y();
        const auto y_delta = ( y_max - y_min ) * Y_delta / ( Y_max - Y_min );
        table->moveRange( axis, y_delta );
        m_p = m_p1;
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

    const auto X_min = m_axis->leftMargin();
    const auto X_max = BaseClass::screen()->width() - m_axis->rightMargin();

    const auto stride = float( X_max - X_min ) / ( naxes - 1 );
    for ( size_t i = 0; i < naxes; i++ )
    {
        const auto ex = static_cast<float>( e->x() );
        const auto x = m_axis->leftMargin() + stride * i;
        if ( x - 5 < ex && ex < x + 5 ) { table->resetRange(i); return; }
    }

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

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    m_painter.begin( BaseClass::screen() );
    {
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        // Draw range bars for each axis.
        const auto dpr = BaseClass::scene()->camera()->devicePixelRatio();
        const auto X_min = m_axis->leftMargin();
        const auto X_max = BaseClass::screen()->width() - m_axis->rightMargin();
        const auto Y_min = m_axis->topMargin();
        const auto Y_max = BaseClass::screen()->height() - m_axis->bottomMargin();
        const auto stride = float( X_max - X_min ) / ( naxes - 1 );
        for ( size_t i = 0; i < naxes; ++i )
        {
            const auto X = m_axis->leftMargin() + stride * i;
            const auto y_min_range = table->minRange(i);
            const auto y_max_range = table->maxRange(i);
            const auto y_min_value = table->minValue(i);
            const auto y_max_value = table->maxValue(i);
            const auto Y_min_range = Y_max - ( Y_max - Y_min ) * ( y_max_range - y_min_value ) / ( y_max_value - y_min_value );
            const auto Y_max_range = Y_max - ( Y_max - Y_min ) * ( y_min_range - y_min_value ) / ( y_max_value - y_min_value );

            // Range box
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Color( m_range_color );
            kvs::OpenGL::Vertex( kvs::Vec2( X - 10, Y_min_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X + 10, Y_min_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X + 10, Y_max_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X - 10, Y_max_range ) * dpr );
            kvs::OpenGL::End();

            // Range edge
            kvs::OpenGL::SetLineWidth( m_range_edge_width * dpr );
            kvs::OpenGL::Begin( GL_LINE_LOOP );
            kvs::OpenGL::Color( m_range_edge_color );
            kvs::OpenGL::Vertex( kvs::Vec2( X - 10, Y_min_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X + 10, Y_min_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X + 10, Y_max_range ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( X - 10, Y_max_range ) * dpr );
            kvs::OpenGL::End();
        }
    }
    m_painter.end();
}

} // end of namespace kvs
