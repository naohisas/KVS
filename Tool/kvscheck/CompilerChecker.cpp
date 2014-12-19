/****************************************************************************/
/**
 *  @file CompilerChecker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CompilerChecker.cpp 1439 2013-03-18 01:51:51Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "CompilerChecker.h"
#include <kvs/Compiler>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CompilerChecker class.
 */
/*===========================================================================*/
CompilerChecker::CompilerChecker()
{
    m_name = kvs::Compiler::Name();
    m_version = kvs::Compiler::Version();
}

/*===========================================================================*/
/**
 *  @brief  Returns a compiler name.
 *  @return compiler name
 */
/*===========================================================================*/
const std::string& CompilerChecker::name() const
{
    return m_name;
}

/*===========================================================================*/
/**
 *  @brief  Returns a compiler version.
 *  @return compiler version
 */
/*===========================================================================*/
const std::string& CompilerChecker::version() const
{
    return m_version;
}

/*==========================================================================*/
/**
 *  @brief Outputs version information.
 *  @param os [in] output stream
 *  @param checker [in] compiler checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const CompilerChecker& checker )
{
    os << "Compiler : " << checker.name() << " (" << checker.version() << ")";
    return os;
}

} // end of namespace kvscheck
