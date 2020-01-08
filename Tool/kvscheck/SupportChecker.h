/*****************************************************************************/
/**
 *  @file   SupportChecker.h
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
#include <kvs/StringList>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  KVS support library checker class.
 */
/*==========================================================================*/
class SupportChecker
{
private:
    kvs::StringList m_descriptions; ///< KVS support library description list
    kvs::StringList m_versions; ///< KVS support library version list

public:
    SupportChecker();
    const kvs::StringList& descriptionList() const { return m_descriptions; }
    const kvs::StringList& versionList() const { return m_versions; }
    friend std::ostream& operator << ( std::ostream& os, const SupportChecker& checker );
};

} // end of namespace kvscheck
