/****************************************************************************/
/**
 *  @file VRTable.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VRTable.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DCM__VR_TABLE_H_INCLUDE
#define KVS__DCM__VR_TABLE_H_INCLUDE

#include "VRType.h"
#include <string>
#include "ElementType.h"
#include "DataType.h"


namespace kvs
{

namespace dcm
{

struct VRTable
{
    std::string vr_string;
    dcm::VRType vr_type;
    dcm::ElementType elem_type;
    dcm::DataType data_type;
};

const VRTable VR_TABLE[] =
{
    { "",   dcm::VR_NONE, dcm::ELEM_UNKNOWN,         dcm::DATA_STRING },
    { "AE", dcm::VR_AE,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "AS", dcm::VR_AS,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "AT", dcm::VR_AT,   dcm::ELEM_EXPLICIT,        dcm::DATA_UINT   },
    { "CS", dcm::VR_CS,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "DA", dcm::VR_DA,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "DS", dcm::VR_DS,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "DT", dcm::VR_DT,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "FL", dcm::VR_FL,   dcm::ELEM_EXPLICIT,        dcm::DATA_FLOAT  },
    { "FD", dcm::VR_FD,   dcm::ELEM_EXPLICIT,        dcm::DATA_DOUBLE },
    { "IS", dcm::VR_IS,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "LO", dcm::VR_LO,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "LT", dcm::VR_LT,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "OB", dcm::VR_OB,   dcm::ELEM_EXPLICIT_CUSTOM, dcm::DATA_OTHER  },
    { "OW", dcm::VR_OW,   dcm::ELEM_EXPLICIT_CUSTOM, dcm::DATA_OTHER  },
    { "PN", dcm::VR_PN,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "SH", dcm::VR_SH,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "SL", dcm::VR_SL,   dcm::ELEM_EXPLICIT,        dcm::DATA_INT    },
    { "SQ", dcm::VR_SQ,   dcm::ELEM_EXPLICIT_CUSTOM, dcm::DATA_OTHER  },
    { "SS", dcm::VR_SS,   dcm::ELEM_EXPLICIT,        dcm::DATA_SHORT  },
    { "ST", dcm::VR_ST,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "TM", dcm::VR_TM,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "UI", dcm::VR_UI,   dcm::ELEM_EXPLICIT,        dcm::DATA_STRING },
    { "UL", dcm::VR_UL,   dcm::ELEM_EXPLICIT,        dcm::DATA_UINT   },
    { "US", dcm::VR_US,   dcm::ELEM_EXPLICIT,        dcm::DATA_USHORT },
    { "UN", dcm::VR_UN,   dcm::ELEM_EXPLICIT_CUSTOM, dcm::DATA_OTHER  },
};

const int VR_TABLE_SIZE = sizeof( VR_TABLE ) / sizeof( VRTable );

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM_VR_TABLE_H_INCLUDE
