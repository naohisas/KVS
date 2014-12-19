/*****************************************************************************/
/**
 *  @file   PaintEvent.h
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
#ifndef KVS__PAINT_EVENT_H_INCLUDE
#define KVS__PAINT_EVENT_H_INCLUDE

#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PaintEvent class.
 */
/*===========================================================================*/
class PaintEvent : public kvs::EventBase
{
public:

    PaintEvent();
    virtual ~PaintEvent();

    int type() const;
};

} // end of namespace kvs

#endif // KVS__PAINT_EVENT_H_INCLUDE
