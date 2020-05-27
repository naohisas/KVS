/*****************************************************************************/
/**
 *  @file   Axis2DMatrix.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Axis2DMatrix.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/TableObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void Axis2DMatrix::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    BaseClass::startTimer();

    kvs::TableObject* table = kvs::TableObject::DownCast( object );
    this->setup_axes( table );

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    BaseClass::painter().begin( screen() );
    {
        BaseClass::updateAxes( table );

        const float dpr = camera->devicePixelRatio();
        const float X0 = BaseClass::leftMargin();
        const float X1 = camera->windowWidth() - BaseClass::rightMargin();
        const float Y0 = BaseClass::topMargin();
        const float Y1 = camera->windowHeight() - BaseClass::bottomMargin();

        const size_t M = table->numberOfColumns(); // number of dimensions
        const float Lx = float( X1 - X0 - m_padding * ( M - 1 ) ) / M; // length for each x axis
        const float Ly = float( Y1 - Y0 - m_padding * ( M - 1 ) ) / M; // length for each y axis

        for ( size_t j = 0; j < M; ++j )
        {
            for ( size_t i = 0; i < M; ++i )
            {
                const float x0 = X0 + ( Lx + m_padding ) * i;
                const float x1 = x0 + Lx;
                const float y0 = Y0 + ( Ly + m_padding ) * j;
                const float y1 = y0 + Ly;
                const kvs::Vec4 rect( x0, x1, y0, y1 );

                BaseClass::drawBackground( rect, dpr );
                BaseClass::drawGridlines( rect, dpr );
                BaseClass::drawBorder( rect, dpr );
                this->draw_axes( rect, M, i, j );
            }
        }
    }
    BaseClass::painter().end();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Setups the axes.
 *  @param  table [in] pointer to the table object
 */
/*===========================================================================*/
void Axis2DMatrix::setup_axes( const kvs::TableObject* table )
{
    if ( BaseClass::axes().size() < table->numberOfColumns() )
    {
        const size_t n = table->numberOfColumns() - BaseClass::axes().size();
        for ( size_t i = 0; i < n; ++i )
        {
            BaseClass::axes().push_back( new kvs::ValueAxis() );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draws the axes.
 *  @param  rect [in] plotting region rectangle
 *  @param  dim [in] dimensions of the data
 *  @param  x_index [in] x index of the plotting region
 *  @param  y_index [in] y index of the plotting region
 */
/*===========================================================================*/
void Axis2DMatrix::draw_axes(
    const kvs::Vec4& rect,
    const size_t dim,
    const size_t x_index,
    const size_t y_index )
{
    auto x_axis = BaseClass::axis( x_index );
    x_axis.setAlignToBottom();
    x_axis.setLabelVisible( y_index == dim - 1 );
    x_axis.setTickLabelVisible( y_index == dim - 1 );

    auto y_axis = BaseClass::axis( y_index );
    y_axis.setAlignToLeft();
    y_axis.setLabelVisible( x_index == 0 );
    y_axis.setTickLabelVisible( x_index == 0 );

    BaseClass::drawAxis( rect, &x_axis );
    BaseClass::drawAxis( rect, &y_axis );
}

} // end of namespace kvs
