/****************************************************************************/
/**
 *  @file CompilerChecker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CompilerChecker.h 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSCHECK__COMPILER_CHECKER_H_INCLUDE
#define KVSCHECK__COMPILER_CHECKER_H_INCLUDE

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
protected:

    std::string m_name; ///< compiler name
    std::string m_version; ///< compiler version

public:

    CompilerChecker();

    const std::string& name() const;
    const std::string& version() const;
    friend std::ostream& operator << ( std::ostream& os, const CompilerChecker& checker );
};

} // end of namespace kvscheck

#endif // KVSCHECK__COMPILER_CHECKER_H_INCLUDE
