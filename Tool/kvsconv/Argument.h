/*****************************************************************************/
/**
 *  @file   Argument.h
 *  @author Naohisa Sakamoto
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
    void set_options();
};

} // end of namespace kvsconv
