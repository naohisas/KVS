/*****************************************************************************/
/**
 *  @file   InitializeEvent.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__INITIALIZE_EVENT_H_INCLUDE
#define KVS__INITIALIZE_EVENT_H_INCLUDE

#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  InitializeEvent class.
 */
/*===========================================================================*/
class InitializeEvent : public kvs::EventBase
{
public:

    InitializeEvent();
    virtual ~InitializeEvent();

    int type() const;
};

} // end of namespace kvs

#endif // KVS__INITIALIZE_EVENT_H_INCLUDE
