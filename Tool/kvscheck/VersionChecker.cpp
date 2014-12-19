/****************************************************************************/
/**
 *  @file   VersionChecker.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VersionChecker.cpp 1438 2013-03-18 01:49:19Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "VersionChecker.h"
#include <kvs/Version>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new VersionChecker class.
 */
/*==========================================================================*/
VersionChecker::VersionChecker()
{
    m_name = kvs::Version::Name();
}

/*==========================================================================*/
/**
 *  @brief  Returns a KVS version name.
 */
/*==========================================================================*/
const std::string& VersionChecker::name() const
{
    return m_name;
}

/*==========================================================================*/
/**
 *  @brief  Outputs version information.
 *  @param  os [in] output stream
 *  @param  checker [in] version checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const VersionChecker& checker )
{
    os << "KVS version : " << checker.name();
    return os;
}

} // end of namespace kvscheck
