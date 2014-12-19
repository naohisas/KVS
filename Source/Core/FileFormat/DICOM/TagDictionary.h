/****************************************************************************/
/**
 *  @file TagDictionary.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagDictionary.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DCM__TAG_DICTIONARY_H_INCLUDE
#define KVS__DCM__TAG_DICTIONARY_H_INCLUDE

#include <vector>
#include <algorithm>
#include "Tag.h"
#include "TagTable.h"


#define PUSH_BACK_DICOM_TAG_TO_CONTAINER( GroupID )                      \
    for( int i = 0; i < dcm::TAG_TABLE_##GroupID##_SIZE; i++ )             \
    {                                                                    \
        dcm::Tag val = dcm::Tag( dcm::TAG_TABLE_##GroupID[i].group_id,   \
                                 dcm::TAG_TABLE_##GroupID[i].element_id, \
                                 dcm::TAG_TABLE_##GroupID[i].vr_type,    \
                                 dcm::TAG_TABLE_##GroupID[i].name );     \
        m_container.push_back( val );                                    \
    }


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  DICOM element tag dictionary class
 */
/*===========================================================================*/
class TagDictionary
{
public:

    typedef std::vector<dcm::Tag> Container;
    typedef std::vector<dcm::Tag>::iterator ContainerIterator;

protected:

    Container m_container; ///< container

public:

    TagDictionary();
    virtual ~TagDictionary();

public:

    dcm::Tag operator [] ( const dcm::Tag& key );

protected:

    void create();
    void clear();
};

/*===========================================================================*/
/**
 *  @brief  Constructor.
 */
/*===========================================================================*/
inline TagDictionary::TagDictionary()
{
    this->create();
}

/*===========================================================================*/
/**
 *  @brief  Destructor.
 */
/*===========================================================================*/
inline TagDictionary::~TagDictionary()
{
    this->clear();
}

/*===========================================================================*/
/**
 *  @brief  '[]' operator.
 *  @param   key [i] search key
 */
/*===========================================================================*/
inline dcm::Tag TagDictionary::operator [] ( const dcm::Tag& key )
{
    ContainerIterator i = std::find( m_container.begin(), m_container.end(), key );

    return
        i == m_container.end() ?
        dcm::Tag( key.groupID(), key.elementID() ) :
        *i;
}

/*===========================================================================*/
/**
 *  @brief  Create the container.
 */
/*===========================================================================*/
inline void TagDictionary::create()
{
    m_container.reserve( dcm::TAG_TABLE_SIZE );
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0008);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0010);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0018);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0020);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0028);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0032);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0038);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(003A);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0040);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0050);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0054);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0060);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0070);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0088);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0100);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0400);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2000);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2010);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2020);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2030);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2040);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2050);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2100);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2110);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2120);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(2130);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(3002);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(3004);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(3006);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(3008);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(300A);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(300C);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(300E);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(4000);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(4008);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(4FFE);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(5200);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(5400);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(7FE0);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(FFFA);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(FFFC);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(FFFE);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0002);
    PUSH_BACK_DICOM_TAG_TO_CONTAINER(0004);
/*
    m_container.reserve( dcm::TAG_TABLE_SIZE );
    for( int i = 0; i < dcm::TAG_TABLE_SIZE; i++ )
    {
        dcm::Tag val = dcm::Tag( dcm::TAG_TABLE[i].group_id,
        dcm::TAG_TABLE[i].element_id,
        dcm::TAG_TABLE[i].vr_type,
        dcm::TAG_TABLE[i].name );

        m_container.push_back( val );
    }
*/
}

/*===========================================================================*/
/**
 *  @brief  Clear the container.
 */
/*===========================================================================*/
inline void TagDictionary::clear()
{
    m_container.clear();
}

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__TAG_DICTIONARY_H_INCLUDE
