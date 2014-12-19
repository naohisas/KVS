/****************************************************************************/
/**
 *  @file KeyEvent.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KeyEvent.cpp 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "KeyEvent.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyEvent class.
 */
/*===========================================================================*/
KeyEvent::KeyEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyEvent class.
 *  @param  event [in] key event
 */
/*===========================================================================*/
KeyEvent::KeyEvent( const KeyEvent& event ):
    m_key( event.m_key ),
    m_x( event.m_x ),
    m_y( event.m_y )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyEvent class.
 *  @param  key [in] key code
 *  @param  x [in] mouse cursor position of X
 *  @param  y [in] mouse cursor position of Y
 */
/*===========================================================================*/
KeyEvent::KeyEvent( int key, int x, int y ):
    m_key( key ),
    m_x( x ),
    m_y( y )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the KeyEvent class.
 */
/*===========================================================================*/
KeyEvent::~KeyEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the key code.
 *  @return key code
 */
/*===========================================================================*/
int KeyEvent::key() const
{
    return m_key;
}

/*===========================================================================*/
/**
 *  @brief  Returns mouse cursor position of x.
 *  @return mouse cursor position of x
 */
/*===========================================================================*/
int KeyEvent::x() const
{
    return m_x;
}

/*===========================================================================*/
/**
 *  @brief  Returns mouse cursor position of y.
 *  @return mouse cursor position of y
 */
/*===========================================================================*/
int KeyEvent::y() const
{
    return m_y;
}

/*===========================================================================*/
/**
 *  @brief  Returns key type.
 *  @return key type
 */
/*===========================================================================*/
int KeyEvent::type() const
{
    return EventBase::KeyPressEvent;
}

/*===========================================================================*/
/**
 *  @brief  Sets a key code.
 *  @param  key [in] key code
 */
/*===========================================================================*/
void KeyEvent::setKey( int key )
{
    m_key = key;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse cursor position
 *  @param  x [in] mouse cursor position of x
 *  @param  y [in] mouse cursor position of y
 */
/*===========================================================================*/
void KeyEvent::setPosition( int x, int y )
{
    m_x = x;
    m_y = y;
}

} // end of namespace kvs
