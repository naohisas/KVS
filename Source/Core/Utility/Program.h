/*****************************************************************************/
/**
 *  @file   Program.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "Noncopyable.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Program class.
 */
/*===========================================================================*/
class Program : private kvs::Noncopyable
{
public:
    int start( int argc, char** argv );

private:
    virtual int exec( int argc, char** argv ) = 0;
};

} // end of namespace kvs
