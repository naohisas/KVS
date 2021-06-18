/****************************************************************************/
/**
 *  @file   Tag.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__DCM__TAG_H_INCLUDE
#define KVS__DCM__TAG_H_INCLUDE

#include "VRType.h"


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  Dicom tag class.
 */
/*===========================================================================*/
class Tag
{
protected:

    unsigned short m_group_id; ///< group ID
    unsigned short m_element_id; ///< element ID
    dcm::VRType m_vr_type; ///< VR type (initial value)
    std::string m_name; ///< element name

public:

    Tag();
    Tag( const unsigned short group_id,
         const unsigned short element_id,
         const dcm::VRType vr_type = dcm::VR_NONE,
         const std::string name = "" );
    Tag( const Tag& tag );
    Tag( std::ifstream& ifs, const bool swap = false );
    virtual ~Tag();

public:

    Tag& operator = ( const Tag& t );
    friend bool operator == ( const Tag& a, const Tag& b );
    friend bool operator != ( const Tag& a, const Tag& b );
    friend bool operator < ( const Tag& a, const Tag& b );
    friend std::ostream& operator << ( std::ostream& os, const Tag& t );
    friend std::ofstream& operator << ( std::ofstream& ofs, const Tag& t );

public:

    unsigned short groupID() const;
    unsigned short elementID() const;
    dcm::VRType vrType() const;
    const std::string& name() const;

public:

    bool read( std::ifstream& ifs, bool swap );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM_TAG_H_INCLUDE
