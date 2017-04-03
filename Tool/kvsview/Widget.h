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
#pragma once
#include <string>
#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/OrientationAxis>
#include <kvs/glut/Screen>


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
    void screenUpdated();
};

/*===========================================================================*/
/**
 *  @brief  Color map bar class.
 */
/*===========================================================================*/
class ColorMapBar : public kvs::ColorMapBar
{
public:
    ColorMapBar( kvs::ScreenBase* screen );
    void screenResized();
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
    void screenResized();
};

} // end of namespace Widget

} // end of namespace kvsview
