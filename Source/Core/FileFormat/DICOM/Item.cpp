/****************************************************************************/
/**
 *  @file Item.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Item.cpp 1758 2014-05-04 13:19:24Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Item.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "StreamReader.h"


namespace
{

const unsigned int ItemTagSize     = 4;
const unsigned int ItemLengthSize  = 4;
//const unsigned int ItemEmptyLength = 4;

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
Item::Item()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 *  @param  data_length [i] data length [byte]
 */
/*===========================================================================*/
Item::Item( const unsigned int data_length ) :
    m_data_length( data_length )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructor.
 */
/*===========================================================================*/
Item::~Item()
{
}

/*===========================================================================*/
/**
 *  @brief  Return the data length.
 */
/*===========================================================================*/
unsigned int Item::dataLength() const
{
    return m_data_length;
}

/*===========================================================================*/
/**
 *  @brief  Return the item value string.
 */
/*===========================================================================*/
const std::string& Item::values() const
{
    return m_values;
}

/*===========================================================================*/
/**
 *  @brief  Read the item data.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] swap flag
 *  @return true if reading successful, false if not.
 */
/*===========================================================================*/
bool dcm::Item::read( std::ifstream& ifs, const bool swap )
{
    // Check the sequence delimitation item.
    if( m_data_length == 0xffffffff )
    {
        return this->read_undefined_length_item( ifs, swap );
    }

    unsigned int length = 0;
    while( m_data_length < length )
    {
        this->read_item_tag( ifs, swap );
        this->read_item_length( ifs, swap );

        m_values += dcm::StreamReader::Get( ifs, m_item_length, swap ) + " ";
        length += ::ItemTagSize + ::ItemLengthSize + m_item_length;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reading the item tag.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] byte swap flag
 *  @return true if reading the item tag successful.
 */
/*===========================================================================*/
bool dcm::Item::read_item_tag( std::ifstream& ifs, const bool swap )
{
    m_item_tag[0] = dcm::StreamReader::Get<unsigned short>( ifs, swap );
    m_item_tag[1] = dcm::StreamReader::Get<unsigned short>( ifs, swap );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read the item length.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] byte swap flag
 *  @return true if reading the item length successful.
 */
/*===========================================================================*/
bool dcm::Item::read_item_length( std::ifstream& ifs, const bool swap )
{
    m_item_length = dcm::StreamReader::Get<unsigned int>( ifs, swap );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Seek the item delimitation tag.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] swap flag
 *  @return true if the item delimitation tag is found, false if not.
 */
/*===========================================================================*/
bool dcm::Item::seek_item_delimitation( std::ifstream& ifs, const bool swap )
{
    while( !ifs.eof() )
    {
        m_item_tag[0] = dcm::StreamReader::Get<unsigned short>( ifs, swap );
        if( m_item_tag[0] == 0xfffe )
        {
            m_item_tag[1] = dcm::StreamReader::Get<unsigned short>( ifs, swap );
            if( m_item_tag[1] == 0xe00d )
            {
                return true;
            }
        }
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Read the undefined length item.
 *  @param  ifs  [i] input file stream
 *  @param  swap [i] byte swap flag
 *  @return true if reading the item length successful, false if not.
 */
/*===========================================================================*/
bool dcm::Item::read_undefined_length_item( std::ifstream& ifs, const bool swap )
{
    this->read_item_tag( ifs, swap );
    this->read_item_length( ifs, swap );

    // Check the item delimitation item.
    if( m_item_length == 0xffffffff )
    {
        this->seek_item_delimitation( ifs, swap );
        this->read_item_length( ifs, swap );

        /* After executing the seek_sequence_delimitation() and
         * read_item_length(), sequence delimitation values are set to the
         * member parameters as follows:
         *
         *     m_item_tag[0] == 0xfffe
         *     m_item_tag[1] == 0xe00d
         *     m_item_length == 00000000H
         */

        return this->read_undefined_length_item( ifs, swap );
    }

    // Check the sequence delimitation.
    if( m_item_tag[0] == 0xfffe && m_item_tag[1] == 0xe0dd ) return true;

    if( m_item_length > 0 )
    {
        m_values += dcm::StreamReader::Get( ifs, m_item_length, swap ) + " ";
    }

    return this->read_undefined_length_item( ifs, swap );
}

} // end of namespace dcm

} // end of namespcae kvs
