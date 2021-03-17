/*****************************************************************************/
/**
 *  @file   SizeOf.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <kvs/Program>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Type size checker class
 */
/*===========================================================================*/
class SizeOf : public kvs::Program
{
    int exec()
    {
        std::cout << "sizeof(char)   : " << sizeof(char) << " bytes" << std::endl;
        std::cout << "sizeof(short)  : " << sizeof(short) << " bytes" << std::endl;
        std::cout << "sizeof(int)    : " << sizeof(int) << " bytes" << std::endl;
        std::cout << "sizeof(long)   : " << sizeof(long) << " bytes" << std::endl;
        std::cout << "sizeof(float)  : " << sizeof(float) << " bytes" << std::endl;
        std::cout << "sizeof(double) : " << sizeof(double) << " bytes" << std::endl;
        std::cout << "sizeof(size_t) : " << sizeof(size_t) << " bytes" << std::endl;
        std::cout << "sizeof(void*)  : " << sizeof(void*) << " bytes" << std::endl;
        return 0;
    }
};

} // end of namespace kvscheck
