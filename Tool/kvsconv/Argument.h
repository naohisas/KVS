/*****************************************************************************/
/**
 *  @file   Argument.h
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
#pragma once
#include <string>
#include <kvs/CommandLine>


namespace kvsconv
{

/*===========================================================================*/
/**
 *  Argument class.
 */
/*===========================================================================*/
class Argument : public kvs::CommandLine
{
public:
    class Common;

public:
    Argument( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Common argument class.
 */
/*===========================================================================*/
class Argument::Common : public kvs::CommandLine
{
public:
    Common( int argc, char** argv );
    Common( int argc, char** argv, const std::string& converter );

private:
    void set_options( void );
};

} // end of namespace kvsconv
