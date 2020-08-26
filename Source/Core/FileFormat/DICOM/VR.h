/****************************************************************************/
/**
 *  @file   VR.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__DCM__VR_H_INCLUDE
#define KVS__DCM__VR_H_INCLUDE

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include "VRType.h"
#include "ElementType.h"
#include "DataType.h"
#include "VRTable.h"


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  DICOM VR (Value Representation) class.
 */
/*===========================================================================*/
class VR
{
protected:

    dcm::VRType m_vr_type; ///< VR type
    dcm::ElementType m_elem_type; ///< element type
    dcm::DataType m_data_type; ///< data type
    unsigned int m_value_length; ///< value length

public:

    VR();
    VR( const dcm::VRType vr_type,
        const dcm::ElementType elem_type,
        const dcm::DataType data_type,
        const unsigned int value_length = 0 );
    VR( const VR& vr );
    VR( const dcm::VRType vr_type );
    VR( std::ifstream& ifs );
    virtual ~VR();

public:

    VR& operator = ( const VR& v );
    friend std::ostream& operator << ( std::ostream& os, const VR& vr );
    friend std::ofstream& operator << ( std::ofstream& ofs, const VR& vr );

public:

    dcm::VRType vrType() const;
    dcm::ElementType elemType() const;
    dcm::DataType dataType() const;
    unsigned int valueLength() const;
    const std::string& vrTypeString() const;
    const std::string& elemTypeString() const;
    const std::string& dataTypeString() const;
    const std::string& vrTypeDescription() const;

public:

    bool read( std::ifstream& ifs, const bool swap = false );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__VR_H_INCLUDE
