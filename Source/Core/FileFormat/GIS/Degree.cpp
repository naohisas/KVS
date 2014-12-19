/*****************************************************************************/
/**
 *  @file   Degree.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "Degree.h"
#include <kvs/Math>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Degree class.
 */
/*===========================================================================*/
Degree::Degree():
    m_degree( 0 ),
    m_minute( 0 ),
    m_second( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Degree class.
 *  @param  degree [in] degree
 *  @param  type [in] representation type
 */
/*===========================================================================*/
Degree::Degree( const float degree, const Degree::Type type )
{
    switch ( type )
    {
    case Degree::Decimal: this->setDecimal( degree ); break;
    case Degree::Nexadecimal: this->setNexadecimal( degree ); break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Degree class.
 *  @param  degree [in] degree
 *  @param  minute [in] minute
 *  @param  second [in] second
 */
/*===========================================================================*/
Degree::Degree( const int degree, const int minute, const float second ) :
    m_degree( degree ),
    m_minute( minute ),
    m_second( second )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Degree class.
 *  @param  degree [in] degree
 */
/*===========================================================================*/
Degree::Degree( const int degree )
{
    m_degree = static_cast<int>( degree / 10000 );

    const int tmp = degree - m_degree * 10000;
    m_minute = static_cast<int>( tmp / 100 );

    const int s = tmp - m_minute * 100;
    m_second = static_cast<float>( s );
}

/*===========================================================================*/
/**
 *  @brief  Sets a degree as decimal number.
 *  @param  degree [in] degree
 */
/*===========================================================================*/
void Degree::setDecimal( const float degree )
{
    m_degree = static_cast<int>( degree );
    m_minute = static_cast<int>( ( degree - static_cast<float>( m_degree ) ) * 60 );
    m_second = ( degree - static_cast<float>( m_degree ) - ( static_cast<float>( m_minute ) / 60 ) ) * 3600;
    if ( kvs::Math::Equal( m_second, 60.0f ) )
    {
        m_second = 0;
        m_minute += 1;

        if ( m_minute == 60 )
        {
            m_minute = 0;
            m_degree += 1;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a degree as nexadecimal number.
 *  @param  degree [in] degree
 */
/*===========================================================================*/
void Degree::setNexadecimal( const float degree )
{
    m_degree = static_cast<int>( degree / 10000 );
    m_minute = static_cast<int>( ( degree - static_cast<float>( m_degree * 10000 ) ) / 100 );
    m_second = degree - static_cast<float>( m_degree * 10000 ) - static_cast<float>( m_minute * 100 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the decimal value.
 *  @return decimal value
 */
/*===========================================================================*/
float Degree::decimal() const
{
    const float ret = static_cast<float>( m_degree ) + static_cast<float>( m_minute ) / 60 + m_second / 3600;
    return ret;
}

/*===========================================================================*/
/**
 *  @brief  Returns the nexadecimal number.
 *  @return Nexadecimal number
 */
/*===========================================================================*/
float Degree::nexadecimal() const
{
    const float ret = static_cast<float>( m_degree * 10000 ) + static_cast<float>( m_minute * 100 ) + m_second;
    return ret;
}

/*===========================================================================*/
/**
 *  @brief  Returns the degree.
 *  @return degree
 */
/*===========================================================================*/
int Degree::degree() const
{
    return m_degree;
}

/*===========================================================================*/
/**
 *  @brief  Returns the minute.
 *  @return minute
 */
/*===========================================================================*/
int Degree::minute() const
{
    return m_minute;
}

/*===========================================================================*/
/**
 *  @brief  Returns the second.
 *  @return second
 */
/*===========================================================================*/
float Degree::second() const
{
    return m_second;
}

} // end of namespace gis

} // end of namespace kvs
