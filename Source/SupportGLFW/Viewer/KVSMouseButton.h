/*****************************************************************************/
/**
 *  @file   KVSMouseButton.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/MouseButton>


namespace kvs
{

namespace glfw
{

class KVSMouseButton
{
public:
    static int Button( int button );
    static int State( int action );
};

inline int KVSMouseButton::Button( int button )
{
    switch ( button )
    {
    case GLFW_MOUSE_BUTTON_LEFT: return kvs::MouseButton::Left;
    case GLFW_MOUSE_BUTTON_MIDDLE: return kvs::MouseButton::Middle;
    case GLFW_MOUSE_BUTTON_RIGHT: return kvs::MouseButton::Right;
    default: break;
    }

    return button;
}

inline int KVSMouseButton::State( int action )
{
    switch ( action )
    {
    case GLFW_RELEASE: return kvs::MouseButton::Up;
    case GLFW_PRESS: return kvs::MouseButton::Down;
    default: break;
    }

    return action;
}

} // end of namespace glfw

} // end of namespace kvs
