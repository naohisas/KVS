/****************************************************************************/
/**
 *  @file XMLDeclaration.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLDeclaration.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "XMLDeclaration.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
XMLDeclaration::XMLDeclaration()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param version [in] version
 *  @param encoding [in] encoding
 *  @param standalone [in] stand alone
 */
/*==========================================================================*/
XMLDeclaration::XMLDeclaration(
    const std::string& version,
    const std::string& encoding,
    const std::string& standalone ):
    TiXmlDeclaration( version, encoding, standalone )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
XMLDeclaration::~XMLDeclaration()
{
}

} // end of namespace kvs
