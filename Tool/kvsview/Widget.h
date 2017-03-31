/*****************************************************************************/
/**
 *  @file   Widget.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Widget.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVSVIEW__WIDGET_H_INCLUDE
#define KVSVIEW__WIDGET_H_INCLUDE

#include <string>
#include <kvs/glut/Label>
#include <kvs/glut/LegendBar>
#include <kvs/glut/OrientationAxis>

#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/OrientationAxis>


namespace kvsview
{

namespace Widget
{

/*===========================================================================*/
/**
 *  @brief  Label class to show the frame rate.
 */
/*===========================================================================*/
class FPSLabel : public kvs::Label
{
    std::string m_renderer_name; ///< renderer name

public:

    FPSLabel( kvs::ScreenBase* screen, const std::string renderer_name );

    void screenUpdated( void );
};

/*===========================================================================*/
/**
 *  @brief  Legend bar class.
 */
/*===========================================================================*/
class LegendBar : public kvs::ColorMapBar
{
public:

    LegendBar( kvs::ScreenBase* screen );

    void screenResized( void );
};

/*===========================================================================*/
/**
 *  @brief  Orientation axis class.
 */
/*===========================================================================*/
class OrientationAxis : public kvs::OrientationAxis
{
public:

    OrientationAxis( kvs::glut::Screen* screen );

    void screenResized( void );
};

} // end of namespace Widget

} // end of namespace kvsview

#endif // KVSVIEW__WIDGET_H_INCLUDE
