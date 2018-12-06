/****************************************************************************/
/**
 *  @file KVSMouseButton.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMouseButton.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/MouseButton>
#include "../GLUT.h"

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#endif
#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 4
#endif


namespace kvs
{

namespace glut
{

class KVSMouseButton
{
public:

    static int Button( int button );
    static int State( int state );
};

inline int KVSMouseButton::Button( int button )
{
    switch ( button )
    {
    case GLUT_LEFT_BUTTON:   return kvs::MouseButton::Left;
    case GLUT_MIDDLE_BUTTON: return kvs::MouseButton::Middle;
    case GLUT_RIGHT_BUTTON:  return kvs::MouseButton::Right;
    case GLUT_WHEEL_UP:      return kvs::MouseButton::WheelUp;
    case GLUT_WHEEL_DOWN:    return kvs::MouseButton::WheelDown;
    default: break;
    }

    return button;
}

inline int KVSMouseButton::State( int state )
{
    switch ( state )
    {
    case GLUT_UP:   return kvs::MouseButton::Up;
    case GLUT_DOWN: return kvs::MouseButton::Down;
    default: break;
    }

    return state;
}

} // end of namespace glut

} // end of namespace kvs
