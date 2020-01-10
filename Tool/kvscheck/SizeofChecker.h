/*****************************************************************************/
/**
 *  @file   SizeofChecker.h
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
#include <iostream>
#include <kvs/Program>


namespace kvscheck
{

class SizeOf : public kvs::Program
{
public:
    int exec( int argc, char** argv );
};

inline int SizeOf::exec( int argc, char** argv )
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

/*==========================================================================*/
/**
 *  'sizeof' checker class.
 */
/*==========================================================================*/
class SizeofChecker
{
public:
    template <typename T> size_t sizeOf() const { return sizeof(T); }
    friend std::ostream& operator << ( std::ostream& os, const SizeofChecker& checker );
};

} // end of namespace kvscheck
