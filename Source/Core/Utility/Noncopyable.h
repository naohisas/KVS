/*****************************************************************************/
/**
 *  @file   Noncopyable.h
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
#ifndef KVS__NONCOPYABLE_H_INCLUDE
#define KVS__NONCOPYABLE_H_INCLUDE


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Non-copyable class.
 */
/*===========================================================================*/
class Noncopyable
{
public:
    Noncopyable() {}
    ~Noncopyable() {}

private:
    Noncopyable( const Noncopyable& );
    Noncopyable& operator =( const Noncopyable );
};

} // end of namespace kvs

#endif // KVS__NONCOPYABLE_H_INCLUDE
