/****************************************************************************/
/**
 *  @file VR.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VR.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "VR.h"
#include <iostream>
#include <fstream>
#include "VRType.h"
#include "ElementType.h"
#include "DataType.h"
#include "VRDictionary.h"
#include "StreamReader.h"


namespace kvs
{

namespace dcm
{

const int VR_STRING_WIDTH = 2;
dcm::VRDictionary VR_DICTIONARY;

/*===========================================================================*/
/**
 *  @brief  Construct a new class.
 */
/*===========================================================================*/
VR::VR() :
    m_vr_type( dcm::VR_NONE ),
    m_elem_type( dcm::ELEM_IMPLICIT ),
    m_data_type( dcm::DATA_STRING ),
    m_value_length( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new class.
 *  @param  vr_type      [in] VR type
 *  @param  elem_type    [in] element type
 *  @param  data_type    [in] data type
 *  @param  value_length [in] value length
 */
/*===========================================================================*/
VR::VR(
    const dcm::VRType vr_type,
    const dcm::ElementType elem_type,
    const dcm::DataType data_type,
    const unsigned int value_length ) :
    m_vr_type( vr_type ),
    m_elem_type( elem_type ),
    m_data_type( data_type ),
    m_value_length( value_length )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a copied calss.
 *  @param  vr [in] Value representation
 */
/*===========================================================================*/
VR::VR( const VR& vr ):
    m_vr_type( vr.m_vr_type ),
    m_elem_type( vr.m_elem_type ),
    m_data_type( vr.m_data_type ),
    m_value_length( vr.m_value_length )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new class by specifying the VR type.
 *  @param  vr_type [in] VR type
 */
/*===========================================================================*/
VR::VR( const dcm::VRType vr_type ):
    m_vr_type( vr_type ),
    m_elem_type( dcm::VR_TABLE[ vr_type ].elem_type ),
    m_data_type( dcm::VR_TABLE[ vr_type ].data_type ),
    m_value_length( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constuct a new class by reading the file.
 *  @param  ifs [in] input file sream
 */
/*===========================================================================*/
VR::VR( std::ifstream& ifs )
{
    this->read( ifs );
}

/*===========================================================================*/
/**
 *  @brief  Destruct this class.
 */
/*===========================================================================*/
VR::~VR()
{
}

/*===========================================================================*/
/**
 *  @brief  Copy operator.
 *  @param  v [in] value representation
 */
/*===========================================================================*/
VR& VR::operator = ( const VR& v )
{
    m_vr_type      = v.m_vr_type;
    m_elem_type    = v.m_elem_type;
    m_data_type    = v.m_data_type;
    m_value_length = v.m_value_length;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Output operator.
 *  @param  os [out] output stream
 *  @param  vr [in]  value representation
 *  @return output stream
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const VR& vr )
{
    os << "VR type:        " << vr.vrTypeString() << std::endl;
    os << "VR description: " << vr.vrTypeDescription() << std::endl;
    os << "Element type:   " << vr.elemTypeString() << std::endl;
    os << "Data type:      " << vr.dataTypeString() << std::endl;
    os << "Value length:   " << vr.valueLength();
    os << " (" << std::hex << std::setw(8) << std::setfill('0') << vr.valueLength() << "H)";
    os << std::dec;
    return os;
}

/*===========================================================================*/
/**
 *  @brief  Output operator.
 *  @param  os [out] output stream
 *  @param  vr [in]  value representation
 *  @return output stream
 */
/*===========================================================================*/
std::ofstream& operator << ( std::ofstream& os, const VR& vr )
{
    os << "VR type:        " << vr.vrTypeString() << std::endl;
    os << "VR description: " << vr.vrTypeDescription() << std::endl;
    os << "Element type:   " << vr.elemTypeString() << std::endl;
    os << "Data type:      " << vr.dataTypeString() << std::endl;
    os << "Value length:   " << vr.valueLength();
    os << " (" << std::hex << std::setw(8) << std::setfill('0') << vr.valueLength() << "H)";
    os << std::dec;
    return os;
}

/*===========================================================================*/
/**
 *  @brief  Return VR type.
 *  @return VR type
 */
/*===========================================================================*/
dcm::VRType VR::vrType() const
{
    return m_vr_type;
}

/*===========================================================================*/
/**
 *  @brief  Return element type.
 *  @return element type
 */
/*===========================================================================*/
dcm::ElementType VR::elemType() const
{
    return m_elem_type;
}

/*===========================================================================*/
/**
 *  @brief  Return data type.
 *  @return data type
 */
/*===========================================================================*/
dcm::DataType VR::dataType() const
{
    return m_data_type;
}

/*===========================================================================*/
/**
 *  @brief  Return value length.
 *  @return value length
 */
/*===========================================================================*/
unsigned int VR::valueLength() const
{
    return m_value_length;
}

/*===========================================================================*/
/**
 *  @brief  Return VR type as string.
 *  @return VR type
 */
/*===========================================================================*/
const std::string& VR::vrTypeString() const
{
    return dcm::VR_TABLE[ m_vr_type ].vr_string;
}

/*===========================================================================*/
/**
 *  @brief  Return element type as string.
 *  @return element type
 */
/*===========================================================================*/
const std::string& VR::elemTypeString() const
{
    return dcm::ELEMENT_TYPE_TO_STRING[ m_elem_type ];
}

/*===========================================================================*/
/**
 *  @brief  Return data type as string.
 *  @return data type
 */
/*===========================================================================*/
const std::string& VR::dataTypeString() const
{
    return dcm::DATA_TYPE_TO_STRING[ m_data_type ];
}

/*===========================================================================*/
/**
 *  @brief  Return VR type description.
 *  @return VR type description
 */
/*===========================================================================*/
const std::string& VR::vrTypeDescription() const
{
    return dcm::VR_TYPE_TO_DESCRIPTION[ m_vr_type ];
}

/*===========================================================================*/
/**
 *  @brief  Read the VR code.
 *  @param  ifs  [in] input file stream
 *  @param  swap [in] swap flag
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool dcm::VR::read( std::ifstream& ifs, const bool swap )
{
    // Read the VR string.
    char vr[ VR_STRING_WIDTH + 1 ];
    ifs.read( vr, VR_STRING_WIDTH );
    if( ifs.bad() )
    {
        kvsMessageError("Cannot read the VR code.");
        return false;
    }
    vr[ VR_STRING_WIDTH ] = '\0';

    // Find the new VR code in the factory class.
    dcm::VR old_vr = *this;
    dcm::VR new_vr = VR_DICTIONARY[ std::string( vr ) ];
    if( new_vr.vrType() == dcm::VR_NONE )
    {
        m_vr_type   = old_vr.vrType();
        m_elem_type = dcm::ELEM_IMPLICIT;
        m_data_type = old_vr.dataType();
    }
    else
    {
        m_vr_type   = new_vr.vrType();
        m_elem_type = new_vr.elemType();
        m_data_type = old_vr.dataType();
    }

    // Read the value length.
    switch( m_elem_type )
    {
    case dcm::ELEM_IMPLICIT:
    {
        ifs.clear();
        ifs.seekg( -VR_STRING_WIDTH, std::ios_base::cur ); // go back
        m_value_length = dcm::StreamReader::Get<unsigned int>( ifs, swap );
        break;
    }
    case dcm::ELEM_EXPLICIT:
    {
        m_value_length = dcm::StreamReader::Get<short>( ifs, swap );
        break;
    }
    case dcm::ELEM_EXPLICIT_CUSTOM:
    {
        ifs.clear();
        ifs.seekg( VR_STRING_WIDTH, std::ios_base::cur );
        m_value_length = dcm::StreamReader::Get<unsigned int>( ifs, swap );
        break;
    }
    default:
    {
        kvsMessageError("Unknown element type.");
        return false;
    }
    }

    return true;
}

} // end of namespace dcm

} // end of namespace kvs
