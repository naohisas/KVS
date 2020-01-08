/*****************************************************************************/
/**
 *  @file   CompilerChecker.cpp
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
