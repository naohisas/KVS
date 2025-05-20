/****************************************************************************/
/**
 *  @file   KVSMouseButton.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include "../Qt.h"
#include <kvs/MouseButton>


namespace kvs
{

namespace qt
{

/*==========================================================================*/
/**
 *  Mouse button conversion class.
 */
/*==========================================================================*/
class KVSMouseButton
{
#if ( KVS_QT_VERSION >= 4 )
    using State = Qt::MouseButton;
#else
    using State = Qt::ButtonState;
#endif

public:
    static int Button( KVSMouseButton::State button );
};

/*==========================================================================*/
/**
 *  Convert the Qt mouse button code to the KVS mouse button code.
 *  @param button [in] Qt mouse button code
 *  @return KVS mouse button code
 */
/*==========================================================================*/
inline int KVSMouseButton::Button( KVSMouseButton::State button )
{
    switch( button )
    {
    case Qt::LeftButton: return kvs::MouseButton::Left;
#if ( KVS_QT_VERSION >= 6 )
    case Qt::MiddleButton: return kvs::MouseButton::Middle;
#else
    case Qt::MidButton: return kvs::MouseButton::Middle;
#endif
    case Qt::RightButton: return kvs::MouseButton::Right;
    default: break;
    }
    return button;
}

} // end of namespace qt

} // end of namespace kvs
