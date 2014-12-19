/****************************************************************************/
/**
 *  @file FastTokenizer.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FastTokenizer.cpp 1135 2012-05-07 15:12:32Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#if KVS_ENABLE_DEPRECATED
#include "FastTokenizer.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs a new empty FastTokenizer.
 */
/*==========================================================================*/
FastTokenizer::FastTokenizer()
    : m_ntokens( 0 )
{
}

/*==========================================================================*/
/**
 *  Constructs a new FastTokenizer.
 */
/*==========================================================================*/
FastTokenizer::FastTokenizer( char* const source, const char* const delimiter )
    : m_ntokens( 0 )
{
    this->set( source, delimiter );
}

/*==========================================================================*/
/**
 *  Destroys the FastTokenizer.
 */
/*==========================================================================*/
FastTokenizer::~FastTokenizer()
{
}

} // end of namespace test
#endif
