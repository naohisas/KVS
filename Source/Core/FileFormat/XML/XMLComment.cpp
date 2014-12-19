/****************************************************************************/
/**
 *  @file XMLComment.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLComment.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "XMLComment.h"

namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
XMLComment::XMLComment()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param comment [in] comments
 */
/*==========================================================================*/
XMLComment::XMLComment( const char* comment ):
    TiXmlComment()
{
    this->SetValue( comment );
}

/*==========================================================================*/
/**
 *  Desturctor.
 */
/*==========================================================================*/
XMLComment::~XMLComment()
{
}

} // end of namespace kvs
