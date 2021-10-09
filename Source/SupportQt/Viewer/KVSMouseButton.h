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
public:
#if ( KVS_QT_VERSION >= 4 )
    static int Button( Qt::MouseButton button );
#else
    static int Button( Qt::ButtonState button );
#endif
};

/*==========================================================================*/
/**
 *  Convert the Qt mouse button code to the KVS mouse button code.
 *  @param button [in] Qt mouse button code
 *  @return KVS mouse button code
 */
/*==========================================================================*/
#if ( KVS_QT_VERSION >= 4 )
inline int KVSMouseButton::Button( Qt::MouseButton button )
#else
inline int KVSMouseButton::Button( Qt::ButtonState button )
#endif
{
    switch( button )
    {
    case Qt::LeftButton: return( kvs::MouseButton::Left );
#if ( KVS_QT_VERSION >= 6 )
    case Qt::MiddleButton: return( kvs::MouseButton::Middle );
#else
    case Qt::MidButton: return( kvs::MouseButton::Middle );
#endif
    case Qt::RightButton: return( kvs::MouseButton::Right );
    default: break;
    }

    return( button );
}

} // end of namespace qt

} // end of namespace kvs
