/*****************************************************************************/
/**
 *  @file   TimeEvent.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TimeEvent.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__TIME_EVENT_H_INCLUDE
#define KVS__TIME_EVENT_H_INCLUDE

#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  TimeEvent class.
 */
/*===========================================================================*/
class TimeEvent : public kvs::EventBase
{
public:

    TimeEvent();
    virtual ~TimeEvent();

    int type() const;
};

} // end of namespace kvs

#endif // KVS__TIME_EVENT_H_INCLUDE
