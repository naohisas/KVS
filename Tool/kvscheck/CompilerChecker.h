/*****************************************************************************/
/**
 *  @file   CompilerChecker.h
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
#include <iostream>
#include <kvs/Program>
#include <kvs/Compiler>


namespace kvscheck
{

class Compiler : public kvs::Program
{
public:
    int exec( int argc, char** argv );
};

inline int Compiler::exec( int argc, char** argv )
{
    const std::string name = kvs::Compiler::Name();
    const std::string version = kvs::Compiler::Version();
    std::cout << "Compiler : " << name << " (version: " << version << ")" << std::endl;
    return 0;
}

/*==========================================================================*/
/**
 *  Compiler checker class.
 */
/*==========================================================================*/
class CompilerChecker
{
private:
    std::string m_name; ///< compiler name
    std::string m_version; ///< compiler version

public:
    CompilerChecker();
    const std::string& name() const { return m_name; }
    const std::string& version() const { return m_version; }
    friend std::ostream& operator << ( std::ostream& os, const CompilerChecker& checker );
};

} // end of namespace kvscheck
