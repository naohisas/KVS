/****************************************************************************/
/**
 *  @file VRType.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VRType.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__DCM__VR_TYPE_H_INCLUDE
#define KVS__DCM__VR_TYPE_H_INCLUDE

#include <string>


namespace kvs
{

namespace dcm
{

enum VRType
{
    VR_NONE =  0, ///< None
    VR_AE   =  1, ///< Application Entity    (16 bytes max.)
    VR_AS   =  2, ///< Age String            (4 bytes fixed)
    VR_AT   =  3, ///< Attribute Tag         (4 bytes fixed)
    VR_CS   =  4, ///< Code String           (16 bytes max.)
    VR_DA   =  5, ///< Data                  (8 bytes fixed)
    VR_DS   =  6, ///< Decimal String        (16 bytes max.)
    VR_DT   =  7, ///< Date Time             (26 bytes max.)
    VR_FL   =  8, ///< Floating Point Single (4 bytes fixed)
    VR_FD   =  9, ///< Floating Point Double (8 bytes fixed)
    VR_IS   = 10, ///< Integer String        (12 bytes max.)
    VR_LO   = 11, ///< Long String           (62 chars max.)
    VR_LT   = 12, ///< Long Text             (10240 chars max.)
    VR_OB   = 13, ///< Other Byte String     (*)
    VR_OW   = 14, ///< Other Word String     (*)
    VR_PN   = 15, ///< Person Name           (64 chars max. per component group)
    VR_SH   = 16, ///< Short String          (16 chars max.)
    VR_SL   = 17, ///< Singled Long          (4 bytes fixed)
    VR_SQ   = 18, ///< Sequence of Items     (*)
    VR_SS   = 19, ///< Signed Short          (2 bytes fixed)
    VR_ST   = 20, ///< Short Text            (1024 chars max.)
    VR_TM   = 21, ///< Time                  (16 bytes max.)
    VR_UI   = 22, ///< Unique Identifier     (64 bytes max.)
    VR_UL   = 23, ///< Unsigned Long         (4 bytes fixed)
    VR_US   = 24, ///< Unsigned short        (2 bytes fixed)
    VR_UN   = 25, ///< Unknown               (*)
    VR_UT   = 26, ///< Unlimited Text        (*)
    NUMBER_OF_VR_TYPES = 27
};

const std::string VR_TYPE_TO_STRING[] =
{
    "",
    "AE",
    "AS",
    "AT",
    "CS",
    "DA",
    "DS",
    "DT",
    "FL",
    "FD",
    "IS",
    "LO",
    "LT",
    "OB",
    "OW",
    "PN",
    "SH",
    "SL",
    "SQ",
    "SS",
    "ST",
    "TM",
    "UI",
    "UL",
    "US"
};

const std::string VR_TYPE_TO_DESCRIPTION[] =
{
    "None",
    "Application Entity (16 bytes max.)",
    "Age String (4 bytes fixed)",
    "Attribute Tag (4 bytes fixed)",
    "Code String (16 bytes max.)",
    "Data (8 bytes fixed)",
    "Decimal String (16 bytes max.)",
    "Date Time (26 bytes max.)",
    "Floating Point Single (4 bytes fixed)",
    "Floating Point Double (8 bytes fixed)",
    "Integer String (12 bytes max.)",
    "Long String (62 chars max.)",
    "Long Text (10240 chars max.)",
    "Other Byte String (*)",
    "Other Word String (*)",
    "Person Name (64 chars max. per component group)",
    "Short String (16 chars max.)",
    "Singled Long (4 bytes fixed)",
    "Sequence of Items (*)",
    "Signed Short (2 bytes fixed)",
    "Short Text (1024 chars max.)",
    "Time (16 bytes max.)",
    "Unique Identifier (64 bytes max.)",
    "Unsigned Long (4 bytes fixed)",
    "Unsigned short (2 bytes fixed)",
    "Unknown (*)",
    "Unlimited Text (*)"
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__VR_TYPE_H_INCLUDE
