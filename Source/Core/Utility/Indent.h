/*****************************************************************************/
/**
 *  @file   Indent.h
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
#ifndef KVS__INDENT_H_INCLUDE
#define KVS__INDENT_H_INCLUDE

#include <iostream>


namespace kvs
{

class Indent
{
private:

    size_t m_width; ///< indent width

public:

    explicit Indent( const size_t width = 0 );

    Indent nextIndent() const;

    friend std::ostream& operator << ( std::ostream& os, const kvs::Indent& indent );
};

} // end of namespace kvs

#endif // KVS__INDENT_H_INCLUDE
