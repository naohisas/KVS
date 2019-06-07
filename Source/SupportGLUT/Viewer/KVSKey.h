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
 *  $Id: KVSKey.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/Key>
#include "../GLUT.h"


namespace kvs
{

namespace glut
{

class KVSKey
{
public:

    static int ASCIICode( const unsigned char key );
    static int SpecialCode( const int key );
    static int Modifier( const int key );
};

inline int KVSKey::ASCIICode( const unsigned char key )
{
    return key;
}

inline int KVSKey::SpecialCode( const int key )
{
    switch ( key )
    {
    case GLUT_KEY_F1:        return kvs::Key::F1;
    case GLUT_KEY_F2:        return kvs::Key::F2;
    case GLUT_KEY_F3:        return kvs::Key::F3;
    case GLUT_KEY_F4:        return kvs::Key::F4;
    case GLUT_KEY_F5:        return kvs::Key::F5;
    case GLUT_KEY_F6:        return kvs::Key::F6;
    case GLUT_KEY_F7:        return kvs::Key::F7;
    case GLUT_KEY_F8:        return kvs::Key::F8;
    case GLUT_KEY_F9:        return kvs::Key::F9;
    case GLUT_KEY_F10:       return kvs::Key::F10;
    case GLUT_KEY_F11:       return kvs::Key::F11;
    case GLUT_KEY_F12:       return kvs::Key::F12;
    case GLUT_KEY_LEFT:      return kvs::Key::Left;
    case GLUT_KEY_RIGHT:     return kvs::Key::Right;
    case GLUT_KEY_UP:        return kvs::Key::Up;
    case GLUT_KEY_DOWN:      return kvs::Key::Down;
    case GLUT_KEY_PAGE_UP:   return kvs::Key::PageUp;
    case GLUT_KEY_PAGE_DOWN: return kvs::Key::PageDown;
    case GLUT_KEY_HOME:      return kvs::Key::Home;
    case GLUT_KEY_END:       return kvs::Key::End;
    case GLUT_KEY_INSERT:    return kvs::Key::Insert;
    default: break;
    }

    return key;
}

inline int KVSKey::Modifier( const int key )
{
    switch ( key )
    {
    case GLUT_ACTIVE_SHIFT: return kvs::Key::ShiftModifier;
    case GLUT_ACTIVE_CTRL:  return kvs::Key::ControlModifier;
    case GLUT_ACTIVE_ALT:   return kvs::Key::AltModifier;
    default: break;
    }

    return key;
}

} // end of namespace glut

} // end of namespace kvs
