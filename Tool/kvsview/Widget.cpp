/*****************************************************************************/
/**
 *  @file   Widget.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Widget.cpp 1517 2013-04-09 11:55:49Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Widget.h"
#include <kvs/RendererBase>
#include <kvs/RendererManager>
#include <sstream>


namespace kvsview
{

namespace Widget
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new FPSLabel.
 *  @param  screen [in] pointer to the screen
 *  @param  renderer_name [in] renderer name
 */
/*===========================================================================*/
FPSLabel::FPSLabel( kvs::ScreenBase* screen, const std::string renderer_name ):
    kvs::Label( screen ),
    m_renderer_name( renderer_name )
{
    setMargin( 10 );
}

/*===========================================================================*/
/**
 *  @brief  Update FPS string.
 */
/*===========================================================================*/
void FPSLabel::screenUpdated()
{
    kvs::Scene* scene = static_cast<kvs::glut::Screen*>( screen() )->scene();
    const kvs::RendererBase* renderer = scene->renderer( m_renderer_name );

    std::stringstream fps;
    fps << std::setprecision(4) << renderer->timer().fps();
    setText( std::string( "FPS: " + fps.str() ).c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ColorMapBar.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
ColorMapBar::ColorMapBar( kvs::ScreenBase* screen ):
    kvs::ColorMapBar( screen )
{
    setWidth( 150 );
    setHeight( 60 );
}

/*===========================================================================*/
/**
 *  @brief  Anchor the ColorMapBar to the left-bottom on the screen.
 */
/*===========================================================================*/
void ColorMapBar::screenResized()
{
    setX( screen()->width() - width() );
    setY( screen()->height() - height() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new OrientationAxis.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
OrientationAxis::OrientationAxis( kvs::glut::Screen* screen ):
    kvs::OrientationAxis( screen, screen->scene() )
{
    setMargin( 0 );
    setSize( 100 );
    setAxisLength( 3.2f );
    setBoxType( kvs::OrientationAxis::SolidBox );
    enableAntiAliasing();
}

/*===========================================================================*/
/**
 *  @brief  Anchor the OrientationAxis to the left-bottom on the screen.
 */
/*===========================================================================*/
void OrientationAxis::screenResized()
{
    setY( screen()->height() - height() );
}

} // end of namespace Widget

} // end of namespace kvsview
