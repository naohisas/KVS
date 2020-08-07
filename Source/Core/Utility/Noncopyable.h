/*****************************************************************************/
/**
 *  @file   Noncopyable.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

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
