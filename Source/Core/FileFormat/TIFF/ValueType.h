/****************************************************************************/
/**
 *  @file   ValueType.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ValueType.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#pragma once

#include <string>


namespace kvs
{

namespace tiff
{

enum ValueType
{
    NoType    = 0,  ///< No type
    Byte      = 1,  ///< 1 byte unsigned integer
    Ascii     = 2,  ///< 1 byte ascii character
    Short     = 3,  ///< 2 byte unsigned integer
    Long      = 4,  ///< 4 byte unsigned integer
    Rational  = 5,  ///< 8 byte unsigned fraction = 4 byte uint / 4 byte uint
    SByte     = 6,  ///< 1 byte signed integer
    Undefined = 7,  ///< 1 byte untyped data
    SShort    = 8,  ///< 2 byte signed integer
    SLong     = 9,  ///< 4 byte signed integer
    SRational = 10, ///< 8 byte signed fraction = 4 byte int / 4 byte int
    Float     = 11, ///< 4 byte floating point
    Double    = 12, ///< 8 byte double precision floating point
    NumberOfValueTypes = 13
};

const std::string ValueTypeName[ kvs::tiff::NumberOfValueTypes ] =
{
    "NoType",
    "Byte",
    "Ascii",
    "Short",
    "Long",
    "Rational",
    "SByte",
    "Undefined",
    "SShort",
    "SLong",
    "SRational",
    "Float",
    "Double"
};

const size_t ValueTypeSize[ kvs::tiff::NumberOfValueTypes ] =
{
    0,
    1,
    1,
    2,
    4,
    8,
    1,
    1,
    2,
    4,
    8,
    4,
    8
};

} // end of namespace tiff

} // end of namespace kvs
