/*****************************************************************************/
/**
 *  @file   Program.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <functional>
#include <kvs/Deprecated>
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
    using ExecFunc = std::function<int()>;

private:
    ExecFunc m_exec_func;

public:
    Program() = default;
    Program( ExecFunc func ): m_exec_func( func ) {}

    void exec( ExecFunc func ) { m_exec_func = func; }
    virtual int exec() { return m_exec_func(); }
    int run();

public:
    KVS_DEPRECATED( int start( int argc, char** argv ) );
private:
    KVS_DEPRECATED( virtual int exec( int argc, char** argv ) ) { return 0; }
};

} // end of namespace kvs
