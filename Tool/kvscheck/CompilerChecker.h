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


namespace kvscheck
{

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
