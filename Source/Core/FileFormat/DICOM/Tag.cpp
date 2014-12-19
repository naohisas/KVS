/****************************************************************************/
/**
 *  @file Tag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tag.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Tag.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "TagDictionary.h"
#include "StreamReader.h"

namespace
{
kvs::dcm::TagDictionary Dictionary;
} // end of namespace


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  Constructor.
 */
/*===========================================================================*/
Tag::Tag():
    m_group_id(0),
    m_element_id(0),
    m_vr_type( dcm::VR_NONE ),
    m_name("")
{
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 *  @param  group_id   [i] group ID
 *  @param  element_id [i] element ID
 *  @param  vr_type    [i] VR type
 *  @param  name       [i] element name
 */
/*===========================================================================*/
Tag::Tag(
    const unsigned short group_id,
    const unsigned short element_id,
    const dcm::VRType    vr_type,
    const std::string    name ) :
    m_group_id( group_id ),
    m_element_id( element_id ),
    m_vr_type( vr_type ),
    m_name( name )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 *  @param  tag [i] element tag
 */
/*===========================================================================*/
Tag::Tag( const Tag& tag ):
    m_group_id( tag.m_group_id ),
    m_element_id( tag.m_element_id ),
    m_vr_type( tag.m_vr_type ),
    m_name( tag.m_name )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] swap flag
 */
/*===========================================================================*/
Tag::Tag( std::ifstream& ifs, bool swap )
{
    this->read( ifs, swap );
}

/*===========================================================================*/
/**
 *  @brief  Destructor.
 */
/*===========================================================================*/
Tag::~Tag()
{
}

/*===========================================================================*/
/**
 *  @brief  '=' operator.
 *  @param  t [i] element tag
 */
/*===========================================================================*/
Tag& Tag::operator = ( const Tag& t )
{
    m_group_id   = t.m_group_id;
    m_element_id = t.m_element_id;
    m_vr_type    = t.m_vr_type;
    m_name       = t.m_name;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  '==' operator.
 *  @param  a [i] element tag
 *  @param  b [i] element tag
 *  @return true if the group ID and the element ID of a is equal to b's
 */
/*===========================================================================*/
bool operator == ( const Tag& a, const Tag& b )
{
    return
        ( a.m_group_id   == b.m_group_id ) &&
        ( a.m_element_id == b.m_element_id );
}

/*===========================================================================*/
/**
 *  @brief  '!=' operator.
 *  @param  a [i] element tag
 *  @param  b [i] element tag
 *  @return true if a is not equal to b
 */
/*===========================================================================*/
bool operator != ( const Tag& a, const Tag& b )
{
    return !(a == b);
}

/*===========================================================================*/
/**
 *  @brief  '<' operator.
 *  @param  a [i] element tag
 *  @param  b [i] element tag
 *  @return true if the group ID and the element ID of a are less than b's
 */
/*===========================================================================*/
bool operator < ( const Tag& a, const Tag& b )
{
    return
        ( a.m_group_id   < b.m_group_id ) &&
        ( a.m_element_id < b.m_element_id );
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator.
 *  @param  os [i] ouput stream
 *  @param  t  [i] element tag
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const Tag& t )
{
    os << "Group ID:       0x" << std::hex << std::setw(4) << std::setfill('0')
       << t.m_group_id << std::endl;
    os << "Element ID:     0x" << std::hex << std::setw(4) << std::setfill('0')
       << t.m_element_id << std::endl;
    os << std::dec;
    os << "Element name:   " << t.m_name;
    return os;
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator.
 *  @param  os [i] ouput stream
 *  @param  t  [i] element tag
 */
/*===========================================================================*/
std::ofstream& operator << ( std::ofstream& os, const Tag& t )
{
    os << "Group ID:       0x" << std::hex << std::setw(4) << std::setfill('0')
       << t.m_group_id << std::endl;
    os << "Element ID:     0x" << std::hex << std::setw(4) << std::setfill('0')
       << t.m_element_id << std::endl;
    os << std::dec;
    os << "Element name:   " << t.m_name;
    return os;
}

/*===========================================================================*/
/**
 *  @brief  Return the group ID.
 */
/*===========================================================================*/
unsigned short Tag::groupID() const
{
    return m_group_id;
}

/*===========================================================================*/
/**
 *  @brief  Return the element ID.
 */
/*===========================================================================*/
unsigned short Tag::elementID() const
{
    return m_element_id;
}

/*===========================================================================*/
/**
 *  @brief  Get the VR type.
 */
/*===========================================================================*/
dcm::VRType Tag::vrType() const
{
    return m_vr_type;
}

/*===========================================================================*/
/**
 *  @brief  Get the element name.
 */
/*===========================================================================*/
const std::string& Tag::name() const
{
    return m_name;
}

/*===========================================================================*/
/**
 *  @brief  Read the tag of the data element.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] swap flag
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool dcm::Tag::read( std::ifstream& ifs, const bool swap )
{
    const unsigned short group_id   = kvs::dcm::StreamReader::Get<unsigned short>( ifs, swap );
    const unsigned short element_id = kvs::dcm::StreamReader::Get<unsigned short>( ifs, swap );

    *this = ::Dictionary[ kvs::dcm::Tag( group_id, element_id ) ];

    return true;
}

} // end of namespace dcm

} // end of namespace kvs
