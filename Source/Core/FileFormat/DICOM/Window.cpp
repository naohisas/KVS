/****************************************************************************/
/**
 *  @file Window.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Window.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Window.h"
#include <kvs/Math>
#include <kvs/Binary>


namespace
{

const int UInt8Min  =  kvsBinary8( 0000, 0000 );
const int UInt8Max  =  kvsBinary8( 1111, 1111 );

const int UInt12Min =  kvsBinary12( 0000, 0000, 0000 );
const int UInt12Max =  kvsBinary12( 1111, 1111, 1111 );

const int UInt16Min =  kvsBinary16( 0000, 0000, 0000, 0000 );
const int UInt16Max =  kvsBinary16( 1111, 1111, 1111, 1111 );

const int Int8Min   = -kvsBinary8( 1000, 0000 );
const int Int8Max   =  kvsBinary8( 0111, 1111 );

const int Int12Min  = -kvsBinary12( 1000, 0000, 0000 );
const int Int12Max  =  kvsBinary12( 0111, 1111, 1111 );

const int Int16Min  = -kvsBinary16( 1000, 0000, 0000, 0000 );
const int Int16Max  =  kvsBinary16( 0111, 1111, 1111, 1111 );

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
Window::Window()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructor.
 *  @param  bits [in] number of bits
 *  @param  sign [in] signed flag
 */
/*===========================================================================*/
Window::Window( const unsigned short bits, const bool sign ):
    m_bits( bits ),
    m_sign( sign ),
    m_level( 0 ),
    m_width( 0 )
{
    this->set( bits, sign );
}

/*===========================================================================*/
/**
 *  @brief  '=' operator.
 *  @param  w [in] window
 */
/*===========================================================================*/
Window& Window::operator = ( const Window& w )
{
    m_bits      = w.m_bits;
    m_sign      = w.m_sign;
    m_min_value = w.m_min_value;
    m_max_value = w.m_max_value;
    m_range     = w.m_range;
    m_level     = w.m_level;
    m_width     = w.m_width;

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Set min-max range.
 *
 *  @param  bits [in] number of bits
 *  @param  sign [in] signed flag
 */
/*===========================================================================*/
void Window::set( const unsigned short bits, const bool sign )
{
    switch( bits )
    {
    case 8:  this->set_range_8bit( sign );  break;
    case 12: this->set_range_12bit( sign ); break;
    case 16: this->set_range_16bit( sign ); break;
    default: break;
    }

    m_range = m_max_value - m_min_value + 1;
}

/*===========================================================================*/
/**
 *  @brief  Rescaling.
 *  @param  slope     [in] rescale slope
 *  @param  intersept [in] rescale intersept
 */
/*===========================================================================*/
void Window::rescale( const double slope, const double intersept )
{
    const double min_value = m_min_value * slope + intersept;
    const double max_value = m_max_value * slope + intersept;

    m_min_value = static_cast<int>( min_value );
    m_max_value = static_cast<int>( max_value );
}

void Window::setLevel( const int level )
{
    m_level = level;
}

void Window::setWidth( const int width )
{
    m_width = width;
}

/*===========================================================================*/
/**
 *  @brief  Get the minimum value.
 *  @retval minimum range
 */
/*===========================================================================*/
int Window::minValue() const
{
    return m_min_value;
}

/*===========================================================================*/
/**
 *  @brief  Get the maximum value.
 *  @retval maximum range
 */
/*===========================================================================*/
int Window::maxValue() const
{
    return m_max_value;
}

int Window::level() const
{
    return m_level;
}

int Window::width() const
{
    return m_width;
}

/*===========================================================================*/
/**
 *  @brief  Get a clamped window level.
 *  @param  level [in] window level
 *  @return clamped window level
 */
/*===========================================================================*/
int Window::clampLevel( const int level )
{
    return kvs::Math::Clamp( level, m_min_value, m_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Get a clamped window width.
 *  @param  width [in] window width
 *  @return clamped width
 */
/*===========================================================================*/
int Window::clampWidth( const int width )
{
    return kvs::Math::Clamp( width, 0, m_range );
}

/*===========================================================================*/
/**
 *  @brief  Set the ranges for 8 bit.
 *  @param  sign [in] signed flag
 */
/*===========================================================================*/
void Window::set_range_8bit( const bool sign )
{
    if( sign )
    {
        m_min_value = ::Int8Min;
        m_max_value = ::Int8Max;
    }
    else
    {
        m_min_value = ::UInt8Min;
        m_max_value = ::UInt8Max;
    }
}

/*===========================================================================*/
/**
 *  @brief  Set the ranges for 12 bit.
 *  @param  sign [in] signed flag
 */
/*===========================================================================*/
void Window::set_range_12bit( const bool sign )
{
    if( sign )
    {
        m_min_value = ::Int12Min;
        m_max_value = ::Int12Max;
    }
    else
    {
        m_min_value = ::UInt12Min;
        m_max_value = ::UInt12Max;
    }
}

/*===========================================================================*/
/**
 *  @brief  Set the ranges for 16 bit.
 *  @param  sign [in] signed flag
 */
/*===========================================================================*/
void Window::set_range_16bit( const bool sign )
{
    if( sign )
    {
        m_min_value = ::Int16Min;
        m_max_value = ::Int16Max;
    }
    else
    {
        m_min_value = ::UInt16Min;
        m_max_value = ::UInt16Max;
    }
}

} // end of namespace dcm

} // end of namespace kvs
