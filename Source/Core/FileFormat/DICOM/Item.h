/****************************************************************************/
/**
 *  @file   Item.h
 *  @author Naohisa Sakamoto
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
