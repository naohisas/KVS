/****************************************************************************/
/**
 *  @file SupportChecker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SupportChecker.h 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSCHECK__SUPPORT_CHECKER_H_INCLUDE
#define KVSCHECK__SUPPORT_CHECKER_H_INCLUDE

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
protected:

    kvs::StringList m_descriptions; ///< KVS support library description list
    kvs::StringList m_versions; ///< KVS support library version list

public:

    SupportChecker();

    const kvs::StringList& descriptionList() const;
    const kvs::StringList& versionList() const;
    friend std::ostream& operator << ( std::ostream& os, const SupportChecker& checker );
};

} // end of namespace kvscheck

#endif // KVSCHECK__SUPPORT_CHECKER_H_INCLUDE
