/****************************************************************************/
/**
 *  @file KVSKey.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSKey.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__QT__KVS_KEY_H_INCLUDE
#define KVS__QT__KVS_KEY_H_INCLUDE

#include "../Qt.h"
#include <kvs/Key>


namespace kvs
{

namespace qt
{

class KVSKey
{
public:

    static int Code( const int key );

#if ( KVS_QT_VERSION >= 4 )
    static int Modifier( const Qt::KeyboardModifiers modifier );
#else
    static int Modifier( Qt::ButtonState modifier );
#endif
};

inline int KVSKey::Code( const int key )
{
    switch( key )
    {
    case Qt::Key_Escape:     return( kvs::Key::Escape );
    case Qt::Key_Backspace:  return( kvs::Key::BackSpace );
    case Qt::Key_Tab:        return( kvs::Key::Tab );
    case Qt::Key_Space:      return( kvs::Key::Space );
    case Qt::Key_Exclam:     return( kvs::Key::Exclamation );
    case Qt::Key_QuoteDbl:   return( kvs::Key::DoubleQuote );
    case Qt::Key_NumberSign: return( kvs::Key::NumberSign );
    case Qt::Key_Dollar:     return( kvs::Key::Dollar );
    case Qt::Key_Percent:    return( kvs::Key::Percent );
    case Qt::Key_Ampersand:  return( kvs::Key::Ampersand );
    case Qt::Key_Apostrophe: return( kvs::Key::Apostrophe );
    case Qt::Key_ParenLeft:  return( kvs::Key::ParenthesisLeft );
    case Qt::Key_ParenRight: return( kvs::Key::ParenthesisRight );
    case Qt::Key_Asterisk:   return( kvs::Key::Asterisk );
    case Qt::Key_Plus:       return( kvs::Key::Plus );
    case Qt::Key_Comma:      return( kvs::Key::Comma );
    case Qt::Key_Minus:      return( kvs::Key::Minus );
    case Qt::Key_Period:     return( kvs::Key::Period );
    case Qt::Key_Slash:      return( kvs::Key::Slash );
    case Qt::Key_Colon:      return( kvs::Key::Colon );
    case Qt::Key_Semicolon:  return( kvs::Key::Semicolon );
    case Qt::Key_Less:       return( kvs::Key::Less );
    case Qt::Key_Equal:      return( kvs::Key::Equal );
    case Qt::Key_Greater:    return( kvs::Key::Greater );
    case Qt::Key_Question:   return( kvs::Key::Question );
    case Qt::Key_At:         return( kvs::Key::At );
    case Qt::Key_0:          return( kvs::Key::Zero );
    case Qt::Key_1:          return( kvs::Key::One );
    case Qt::Key_2:          return( kvs::Key::Two );
    case Qt::Key_3:          return( kvs::Key::Three );
    case Qt::Key_4:          return( kvs::Key::Four );
    case Qt::Key_5:          return( kvs::Key::Five );
    case Qt::Key_6:          return( kvs::Key::Six );
    case Qt::Key_7:          return( kvs::Key::Seven );
    case Qt::Key_8:          return( kvs::Key::Eight );
    case Qt::Key_9:          return( kvs::Key::Nine );
    case Qt::Key_A:          return( kvs::Key::a );
    case Qt::Key_B:          return( kvs::Key::b );
    case Qt::Key_C:          return( kvs::Key::c );
    case Qt::Key_D:          return( kvs::Key::d );
    case Qt::Key_E:          return( kvs::Key::e );
    case Qt::Key_F:          return( kvs::Key::f );
    case Qt::Key_G:          return( kvs::Key::g );
    case Qt::Key_H:          return( kvs::Key::h );
    case Qt::Key_I:          return( kvs::Key::i );
    case Qt::Key_J:          return( kvs::Key::j );
    case Qt::Key_K:          return( kvs::Key::k );
    case Qt::Key_L:          return( kvs::Key::l );
    case Qt::Key_M:          return( kvs::Key::m );
    case Qt::Key_N:          return( kvs::Key::n );
    case Qt::Key_O:          return( kvs::Key::o );
    case Qt::Key_P:          return( kvs::Key::p );
    case Qt::Key_Q:          return( kvs::Key::q );
    case Qt::Key_R:          return( kvs::Key::r );
    case Qt::Key_S:          return( kvs::Key::s );
    case Qt::Key_T:          return( kvs::Key::t );
    case Qt::Key_U:          return( kvs::Key::u );
    case Qt::Key_V:          return( kvs::Key::v );
    case Qt::Key_W:          return( kvs::Key::w );
    case Qt::Key_X:          return( kvs::Key::x );
    case Qt::Key_Y:          return( kvs::Key::y );
    case Qt::Key_Z:          return( kvs::Key::z );
    case Qt::Key_F1:         return( kvs::Key::F1 );
    case Qt::Key_F2:         return( kvs::Key::F2 );
    case Qt::Key_F3:         return( kvs::Key::F3 );
    case Qt::Key_F4:         return( kvs::Key::F4 );
    case Qt::Key_F5:         return( kvs::Key::F5 );
    case Qt::Key_F6:         return( kvs::Key::F6 );
    case Qt::Key_F7:         return( kvs::Key::F7 );
    case Qt::Key_F8:         return( kvs::Key::F8 );
    case Qt::Key_F9:         return( kvs::Key::F9 );
    case Qt::Key_F10:        return( kvs::Key::F10 );
    case Qt::Key_F11:        return( kvs::Key::F11 );
    case Qt::Key_F12:        return( kvs::Key::F12 );
    case Qt::Key_Left:       return( kvs::Key::Left );
    case Qt::Key_Right:      return( kvs::Key::Right );
    case Qt::Key_Up:         return( kvs::Key::Up );
    case Qt::Key_Down:       return( kvs::Key::Down );
    case Qt::Key_PageUp:     return( kvs::Key::PageUp );
    case Qt::Key_PageDown:   return( kvs::Key::PageDown );
    case Qt::Key_Home:       return( kvs::Key::Home );
    case Qt::Key_End:        return( kvs::Key::End );
    case Qt::Key_Insert:     return( kvs::Key::Insert );
    default:                 break;
    }

    return( key );
}

#if ( KVS_QT_VERSION >= 4 )
inline int KVSKey::Modifier( const Qt::KeyboardModifiers modifier )
{
    switch( modifier )
    {
    case Qt::ShiftModifier:   return( kvs::Key::ShiftModifier   );
    case Qt::ControlModifier: return( kvs::Key::ControlModifier );
    case Qt::AltModifier:     return( kvs::Key::AltModifier     );
    default:                  break;
    }

    return( 0 );
}
#else
inline int KVSKey::Modifier( Qt::ButtonState modifier )
{
    switch( modifier )
    {
    case Qt::ShiftButton:   return( kvs::Key::ShiftModifier   );
    case Qt::ControlButton: return( kvs::Key::ControlModifier );
    case Qt::AltButton:     return( kvs::Key::AltModifier     );
    default: break;
    }

    return( 0 );
}
#endif

} // end of namespace qt

} // end of namespace kvs

#endif // KVS__QT__KVS_KEY_H_INCLUDE
