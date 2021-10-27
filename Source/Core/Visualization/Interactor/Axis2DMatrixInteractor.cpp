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
    const float cw = content.width();
    const float ch = content.height();

    const auto padding = m_axis->padding();
    const auto dx = float( cw - padding * ( ncols - 1 ) ) / ncols;
    const auto dy = float( ch - padding * ( ncols - 1 ) ) / ncols;
    float y_min = content.y0();
    for ( size_t j = 0; j < ncols; ++j, y_min += dy + padding )
    {
        float x_min = content.x0();
        for ( size_t i = 0; i < ncols; ++i, x_min += dx + padding )
        {
            const auto x_max = x_min + dx;
            const auto y_max = y_min + dy;

            if ( m_p0.x() < x_min || x_max < m_p0.x() ||
                 m_p0.y() < y_min || y_max < m_p0.y() ) { continue; }

            const size_t x_index = i;
            const size_t y_index = j;
            m_indices.x() = x_index;
            m_indices.y() = y_index;
            //if ( x_index == y_index ) renderer->selectAxis( x_index );

            const auto vx_min = table->minValue( x_index );
            const auto vx_max = table->maxValue( x_index );
            const auto vy_min = table->minValue( y_index );
            const auto vy_max = table->maxValue( y_index );

            const auto dvx = vx_max - vx_min;
            const auto dvy = vy_max - vy_min;

            const auto vx = vx_min + dvx * ( m_p0.x() - x_min ) / dx;
            const auto vy = vy_min + dvy * ( y_max - m_p0.y() ) / dy;

            const kvs::Range vx_range( table->minRange( x_index ), table->maxRange( x_index ) );
            const kvs::Range vy_range( table->minRange( y_index ), table->maxRange( y_index ) );

            m_range_moving = false;
            if ( vx_range.equals( { vx_min, vx_max } ) && vy_range.equals( { vy_min, vy_max } ) )
            {
                return;
            }
            if ( vx_range.contains( vx ) && vy_range.contains( vy ) )
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

    const auto width = BaseClass::screen()->width();
    const auto height = BaseClass::screen()->height();
    const auto content = m_axis->margins().content( width, height );
    const float cw = content.width();
    const float ch = content.height();

    const auto padding = m_axis->padding();
    const auto dx = float( cw - padding * ( ncols - 1 ) ) / ncols;
    const auto dy = float( ch - padding * ( ncols - 1 ) ) / ncols;

    const auto x_min = content.x0() + ( dx + padding ) * m_indices.x();
    const auto y_min = content.y0() + ( dy + padding ) * m_indices.y();
    const auto x_max = x_min + dx;
    const auto y_max = y_min + dy;

    const auto vx_min = table->minValue( m_indices.x() );
    const auto vx_max = table->maxValue( m_indices.x() );
    const auto vy_min = table->minValue( m_indices.y() );
    const auto vy_max = table->maxValue( m_indices.y() );
    const auto dvx = vx_max - vx_min;
    const auto dvy = vy_max - vy_min;

    m_p1 = { e->x(), e->y() };
    kvs::Math::Clamp( m_p1.x(), int( x_min ), int( x_max ) );
    kvs::Math::Clamp( m_p1.y(), int( y_min ), int( y_max ) );

    if ( m_range_moving )
    {
        const auto d = m_p1 - m_p;
        const auto vx = dvx * d.x() / ( x_max - x_min );
        const auto vy = dvy * -d.y() / ( y_max - y_min );
        table->moveRange( m_indices.x(), vx );
        table->moveRange( m_indices.y(), vy );
        m_p = m_p1;
    }
    else
    {
        const auto x0 = kvs::Math::Min( m_p0.x(), m_p1.x() );
        const auto x1 = kvs::Math::Max( m_p0.x(), m_p1.x() );
        const auto y0 = kvs::Math::Min( m_p0.y(), m_p1.y() );
        const auto y1 = kvs::Math::Max( m_p0.y(), m_p1.y() );

        const auto vx0 = vx_min + dvx * ( x0 - x_min ) / dx;
        const auto vx1 = vx_min + dvx * ( x1 - x_min ) / dx;
        const auto vy0 = vy_min + dvy * ( y_max - y1 ) / dy;
        const auto vy1 = vy_min + dvy * ( y_max - y0 ) / dy;

        table->setMinRange( m_indices.x(), vx0 );
        table->setMaxRange( m_indices.x(), vx1 );
        table->setMinRange( m_indices.y(), vy0 );
        table->setMaxRange( m_indices.y(), vy1 );
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

    // Draw range rectables for each axis.
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    m_painter.begin( BaseClass::screen() );
    {
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const auto dpr = BaseClass::scene()->camera()->devicePixelRatio();
        const auto width = BaseClass::screen()->width();
        const auto height = BaseClass::screen()->height();
        const auto content = m_axis->margins().content( width, height );
        const float cw = content.width();
        const float ch = content.height();

        const auto padding = m_axis->padding();
        const auto dx = float( cw - padding * ( ncols - 1 ) ) / ncols;
        const auto dy = float( ch - padding * ( ncols - 1 ) ) / ncols;

        float y_min = content.y0();
        for ( size_t j = 0; j < ncols; ++j, y_min += dy + padding )
        {
            float x_min = content.x0();
            for ( size_t i = 0; i < ncols; ++i, x_min += dx + padding )
            {
                const auto x_index = i;
                const auto y_index = j;
                if ( x_index == y_index ) { continue; }

                const float x_max = x_min + dx;
                const float y_max = y_min + dy;

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
                const auto x0 = x_min + dx * ( vx0 - vx_min ) / dvx;
                const auto x1 = x_min + dx * ( vx1 - vx_min ) / dvx;
                const auto y0 = y_min + dy * ( vy_max - vy1 ) / dvy;
                const auto y1 = y_min + dy * ( vy_max - vy0 ) / dvy;

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
        }
    }
    m_painter.end();
}

} // end of namespace kvs
