/*****************************************************************************/
/**
 *  @file   Event.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Event.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef __EVENT_H__
#define __EVENT_H__

#include "Parameter.h"
#include <kvs/InitializeEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/MousePressEventListener>
#include <kvs/MouseMoveEventListener>


namespace Event
{

/*===========================================================================*/
/**
 *  @brief  Initialize event class.
 */
/*===========================================================================*/
class Init : public kvs::InitializeEventListener
{
    Parameter* m_parameter;

public:

    Init( Parameter* parameter ): m_parameter( parameter ){}

    void update( void );
};

/*===========================================================================*/
/**
 *  @brief  Key press event class.
 */
/*===========================================================================*/
class KeyPress : public kvs::KeyPressEventListener
{
    Parameter* m_parameter;

public:

    KeyPress( Parameter* parameter ): m_parameter( parameter ){}

    void update( kvs::KeyEvent* event );
};

/*===========================================================================*/
/**
 *  @brief  Mouse press event class.
 */
/*===========================================================================*/
class MousePress : public kvs::MousePressEventListener
{
    Parameter* m_parameter;

public:

    MousePress( Parameter* parameter ): m_parameter( parameter ){}

    void update( kvs::MouseEvent* event );
};

/*===========================================================================*/
/**
 *  @brief  Mouse move event class.
 */
/*===========================================================================*/
class MouseMove : public kvs::MouseMoveEventListener
{
    Parameter* m_parameter;

public:

    MouseMove( Parameter* parameter ): m_parameter( parameter ){}

    void update( kvs::MouseEvent* event );
};

} // end of namespace Event

#endif // __EVENT_H__
