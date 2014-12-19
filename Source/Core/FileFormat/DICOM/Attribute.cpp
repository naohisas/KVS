/****************************************************************************/
/**
 *  @file Attribute.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Attribute.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Attribute.h"
#include <cstring>


namespace
{

const char*  Part10Code     = "DICM";
const size_t Part10CodeSize = 4;
const size_t Part10CodeSeek = 128;

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
Attribute::Attribute()
{
}

/*===========================================================================*/
/**
 *  @brief  Destructor.
 */
/*===========================================================================*/
Attribute::~Attribute()
{
}

/*===========================================================================*/
/**
 *  @brief  Get the state of part10 flag.
 *  @return the state of part10 flag.
 */
/*===========================================================================*/
bool Attribute::part10() const
{
    return m_part10;
}

/*===========================================================================*/
/**
 *  @brief  Get the state of swap flag.
 *  @return the state of swap flag.
 */
/*===========================================================================*/
bool Attribute::swap() const
{
    return m_swap;
}

/*===========================================================================*/
/**
 *  @brief  '=' operator.
 *  @param  a [i] attribute
 */
/*===========================================================================*/
Attribute& Attribute::operator = ( const Attribute& a )
{
    m_part10 = a.m_part10;
    m_swap   = a.m_swap;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Check the attribute.
 *  @param  ifs [i] input file stream
 *  @return true if check successful, false if not.
 */
/*===========================================================================*/
bool Attribute::check( std::ifstream& ifs )
{
    ifs.seekg( ::Part10CodeSeek, std::ios::beg );

    char part10[ ::Part10CodeSize ];
    ifs.read( part10, ::Part10CodeSize );

    if( strncmp( part10, ::Part10Code, ::Part10CodeSize ) == 0 )
    {
        m_part10 = true;
    }
    else
    {
        m_part10 = false;
        ifs.seekg( 0, std::ios::beg );
    }

    short temp;
    ifs.read( reinterpret_cast<char*>(&temp), sizeof(short) );
    m_swap = ( temp > 0xff );

    int size = sizeof(short);
    ifs.seekg( -size, std::ios::cur );

    return true;
}

} // end of namespace dcm

} // end of namespace kvs
