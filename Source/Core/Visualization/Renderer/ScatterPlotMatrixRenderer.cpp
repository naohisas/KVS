/*****************************************************************************/
/**
 *  @file   ScatterPlotMatrixRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ScatterPlotMatrixRenderer.h"
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
void ScatterPlotMatrixRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    BaseClass::startTimer();

    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    BaseClass::painter().begin( screen() );
    {
        const float dpr = camera->devicePixelRatio();
        const int width = camera->windowWidth();
        const int height = camera->windowHeight();
        const kvs::Rectangle content = BaseClass::margins().content( width, height );
        const size_t M = table->numberOfColumns(); // number of dimensions
        const float Lx = float( content.width() - m_padding * ( M - 1 ) ) / M; // length for each x axis
        const float Ly = float( content.height() - m_padding * ( M - 1 ) ) / M; // length for each y axis

        for ( size_t j = 0; j < M; ++j )
        {
            for ( size_t i = 0; i < M; ++i )
            {
                const int x0 = content.x0() + ( Lx + m_padding ) * i;
                const int y0 = content.y0() + ( Ly + m_padding ) * j;
                const kvs::Rectangle rect( x0, y0, Lx, Ly );

                BaseClass::drawBackground( rect, dpr );

                const size_t x_index = i;
                const size_t y_index = j;
                // if ( x_index == y_index ) { continue; } // diagonal region

                kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
                engine->beginFrame( screen()->width(), screen()->height(), dpr );
                {
                    BaseClass::drawPolyline( rect, table, x_index, y_index );
                    BaseClass::drawPoint( rect, table, x_index, y_index, false );
                }
                engine->endFrame();
            }
        }
    }
    BaseClass::painter().end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
