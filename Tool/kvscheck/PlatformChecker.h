/*****************************************************************************/
/**
 *  @file   PlatformChecker.h
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
 *  Platform checker class.
 */
/*==========================================================================*/
class PlatformChecker
{
private:
    std::string m_name; ///< platform name
    std::string m_cpu; ///< CPU name

public:
    PlatformChecker();
    const std::string& name() const { return m_name; }
    const std::string& cpu() const { return m_cpu; }
    friend std::ostream& operator << ( std::ostream& os, const PlatformChecker& checker );
};

} // end of namespace kvscheck
