/****************************************************************************/
/**
 *  @file ElementType.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ElementType.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__DCM__ELEMENT_TYPE_H_INCLUDE
#define KVS__DCM__ELEMENT_TYPE_H_INCLUDE

#include <string>

namespace kvs
{

namespace dcm
{

enum ElementType
{
    ELEM_UNKNOWN            = 0, ///< unknown type element
    ELEM_IMPLICIT           = 1, ///< implicit type element
    ELEM_EXPLICIT           = 2, ///< explicit type element
    ELEM_EXPLICIT_CUSTOM    = 3, ///< explicit (custom) type element
    NUMBER_OF_ELEMENT_TYPES = 4
};

const std::string ELEMENT_TYPE_TO_STRING[NUMBER_OF_ELEMENT_TYPES] =
{
    "Unknown",
    "Implicit",
    "Explicit",
    "Explicit (custum)"
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__ELEMENT_TYPE_H_INCLUDE
