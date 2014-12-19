/****************************************************************************/
/**
 *  @file Item.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Item.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DCM__ITEM_H_INCLUDE
#define KVS__DCM__ITEM_H_INCLUDE

#include <iostream>
#include <fstream>
#include <string>


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  Item class.
 */
/*===========================================================================*/
class Item
{
protected:

    unsigned int m_data_length; ///< data length
    unsigned int m_item_length; ///< item length
    unsigned short m_item_tag[2]; ///< item tag
    std::string m_values; ///< value string

public:

    Item();
    Item( const unsigned int data_length );
    virtual ~Item();

public:

    unsigned int dataLength() const;
    const std::string& values() const;

public:

    bool read( std::ifstream& ifs, const bool swap = false );

private:

    bool read_item_tag( std::ifstream& ifs, const bool swap = false );
    bool read_item_length( std::ifstream& ifs, const bool swap = false );
    bool seek_item_delimitation( std::ifstream& ifs, const bool swap = false );
    bool read_undefined_length_item( std::ifstream& ifs, const bool swap = false );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__ITEM_H_INCLUDE
