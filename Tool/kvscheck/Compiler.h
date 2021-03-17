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
    int exec()
    {
        const auto name = kvs::Compiler::Name();
        const auto version = kvs::Compiler::Version();
        std::cout << "Compiler : " << name
                  << " (version: " << version << ")" << std::endl;
        return 0;
    }
};

} // end of namespace kvscheck
