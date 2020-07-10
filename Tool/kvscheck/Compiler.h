/*****************************************************************************/
/**
 *  @file   Compiler.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <kvs/Program>
#include <kvs/Compiler>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Complier checker class.
 */
/*===========================================================================*/
class Compiler : public kvs::Program
{
    int exec( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Executes the checker program.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return 0 if the process is done sucessfully
 */
/*===========================================================================*/
inline int Compiler::exec( int argc, char** argv )
{
    const std::string name = kvs::Compiler::Name();
    const std::string version = kvs::Compiler::Version();
    std::cout << "Compiler : " << name << " (version: " << version << ")" << std::endl;
    return 0;
}

} // end of namespace kvscheck
