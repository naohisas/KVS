/****************************************************************************/
/**
 *  @file   ElementType.h
 *  @author Naohisa Sakamoto
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
