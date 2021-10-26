#include "Axis2DMatrixInteractor.h"
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectManager>
#include <kvs/Camera>
#include <kvs/Range>
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis2DMatrixInteractor class.
 *  @param  axis [in] axis 2D axis
 */
/*===========================================================================*/
Axis2DMatrixInteractor::Axis2DMatrixInteractor( const kvs::Axis2DMatrix* axis ):
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
kvs::TableObject* Axis2DMatrixInteractor::table()
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
void Axis2DMatrixInteractor::mousePressEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

    m_p0 = { e->x(), e->y() };
    m_p = m_p0;

    m_range_setting = true;

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );

    const auto padding = m_axis->padding();
    const auto X_stride = float( content.width() - padding * ( ncols - 1 ) ) / ncols;
    const auto Y_stride = float( content.height() - padding * ( ncols - 1 ) ) / ncols;
    float Y_min = content.y0();
    for ( size_t j = 0; j < ncols; ++j, Y_min += Y_stride + padding )
    {
        float X_min = content.x0();
        for ( size_t i = 0; i < ncols; ++i, X_min += X_stride + padding )
        {
            const float X_max = X_min + X_stride;
            const float Y_max = Y_min + Y_stride;

            if ( m_p0.x() < X_min || X_max < m_p0.x() ||
                 m_p0.y() < Y_min || Y_max < m_p0.y() ) { continue; }

            const size_t x_index = i;
            const size_t y_index = j;
            m_i.x() = x_index;
            m_i.y() = y_index;
            //if ( x_index == y_index ) renderer->selectAxis( x_index );

            const auto x_min = table->minValue( x_index );
            const auto x_max = table->maxValue( x_index );
            const auto y_min = table->minValue( y_index );
            const auto y_max = table->maxValue( y_index );

            const auto dx = x_max - x_min;
            const auto dy = y_max - y_min;

            const auto x0 = x_min + dx * ( m_p0.x() - X_min ) / ( X_max - X_min );
            const auto y0 = y_min + dy * ( Y_max - m_p0.y() ) / ( Y_max - Y_min );

            const kvs::Range x_range( table->minRange( x_index ), table->maxRange( x_index ) );
            const kvs::Range y_range( table->minRange( y_index ), table->maxRange( y_index ) );

            m_range_moving = false;
            if ( x_range.equals( { x_min, x_max } ) && y_range.equals( { y_min, y_max } ) )
            {
                return;
            }
            if ( x_range.contains( x0 ) && y_range.contains( y0 ) )
            {
                m_range_moving = true;
                break;
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void Axis2DMatrixInteractor::mouseMoveEvent( kvs::MouseEvent* e )
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

    m_p1 = { e->x(), e->y() };

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );

    const auto padding = m_axis->padding();
    const auto X_stride = float( content.width() - padding * ( ncols - 1 ) ) / ncols;
    const auto Y_stride = float( content.height() - padding * ( ncols - 1 ) ) / ncols;

    const auto X_min = content.x0() + ( X_stride + padding ) * m_i.x();
    const auto Y_min = content.y0() + ( Y_stride + padding ) * m_i.y();
    const auto X_max = X_min + X_stride;
    const auto Y_max = Y_min + Y_stride;

    // Adjust the mouse moving position.
    kvs::Math::Clamp( m_p1.x(), int( X_min ), int( X_max ) );
    kvs::Math::Clamp( m_p1.y(), int( Y_min ), int( Y_max ) );

    const auto x_min = table->minValue( m_i.x() );
    const auto x_max = table->maxValue( m_i.x() );
    const auto y_min = table->minValue( m_i.y() );
    const auto y_max = table->maxValue( m_i.y() );
    const auto dx = x_max - x_min;
    const auto dy = y_max - y_min;

    if ( m_range_moving )
    {
        const auto D = m_p1 - m_p;
        const auto x_delta = dx * D.x() / ( X_max - X_min );
        const auto y_delta = dy * -D.y() / ( Y_max - Y_min );
        table->moveRange( m_i.x(), x_delta );
        table->moveRange( m_i.y(), y_delta );
        m_p = m_p1;
    }
    else
    {
        const auto X_rect_min = kvs::Math::Min( m_p0.x(), m_p1.x() );
        const auto X_rect_max = kvs::Math::Max( m_p0.x(), m_p1.x() );
        const auto Y_rect_min = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto Y_rect_max = kvs::Math::Max( m_p0.y(), m_p1.y() );

        const auto x_rect_min = x_min + dx * ( X_rect_min - X_min ) / ( X_max - X_min );
        const auto x_rect_max = x_min + dx * ( X_rect_max - X_min ) / ( X_max - X_min );
        const auto y_rect_min = y_min + dy * ( Y_max - Y_rect_max ) / ( Y_max - Y_min );
        const auto y_rect_max = y_min + dy * ( Y_max - Y_rect_min ) / ( Y_max - Y_min );

        table->setMinRange( m_i.x(), x_rect_min );
        table->setMaxRange( m_i.x(), x_rect_max );
        table->setMinRange( m_i.y(), y_rect_min );
        table->setMaxRange( m_i.y(), y_rect_max );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event function.
 *  @param  e [in] pointer to the mouse event
 */
/*===========================================================================*/
void Axis2DMatrixInteractor::mouseReleaseEvent( kvs::MouseEvent* e )
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
void Axis2DMatrixInteractor::mouseDoubleClickEvent( kvs::MouseEvent* e )
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
void Axis2DMatrixInteractor::paintEvent()
{
    auto* table = this->table();
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

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

        const auto padding = m_axis->padding();
        const auto X_stride = float( content.width() - padding * ( ncols - 1 ) ) / ncols;
        const auto Y_stride = float( content.height() - padding * ( ncols - 1 ) ) / ncols;
        float Y_min = content.y0();
        for ( size_t j = 0; j < ncols; ++j, Y_min += Y_stride + padding )
        {
            float X_min = content.x0();
            for ( size_t i = 0; i < ncols; ++i, X_min += X_stride + padding )
            {
                const float X_max = X_min + X_stride;
                const float Y_max = Y_min + Y_stride;

                const auto x_index = i;
                const auto y_index = j;
                if ( x_index == y_index ) { continue; }

                const auto x_min = table->minValue( x_index );
                const auto x_max = table->maxValue( x_index );
                const auto y_min = table->minValue( y_index );
                const auto y_max = table->maxValue( y_index );
                const auto dx = x_max - x_min;
                const auto dy = y_max - y_min;

                const auto x_min_range = table->minRange( x_index );
                const auto x_max_range = table->maxRange( x_index );
                const auto y_min_range = table->minRange( y_index );
                const auto y_max_range = table->maxRange( y_index );

                const auto X_min_range = X_min + ( X_max - X_min ) * ( x_min_range - x_min ) / dx;
                const auto X_max_range = X_min + ( X_max - X_min ) * ( x_max_range - x_min ) / dx;
                const auto Y_min_range = Y_min + ( Y_max - Y_min ) * ( y_max - y_max_range ) / dy;
                const auto Y_max_range = Y_min + ( Y_max - Y_min ) * ( y_max - y_min_range ) / dy;

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
        }
    }
    m_painter.end();
}

} // end of namespace kvs
