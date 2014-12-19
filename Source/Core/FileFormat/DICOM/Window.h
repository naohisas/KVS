/****************************************************************************/
/**
 *  @file Window.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Window.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DCM__WINDOW_H_INCLUDE
#define KVS__DCM__WINDOW_H_INCLUDE


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  DICOM window information class.
 */
/*===========================================================================*/
class Window
{
protected:

    unsigned short m_bits; ///< number of bits
    bool m_sign; ///< true: signed, false: unsigned
    int m_min_value; ///< maximum value of the window's range
    int m_max_value; ///< minimum value of the window's range
    int m_range; ///< range of the window
    int m_level; ///< current window level
    int m_width; ///< current window width

public:

    Window();
    Window( const unsigned short bits, const bool sign );

public:

    Window& operator = ( const Window& w );

public:

    void set( const unsigned short bits, const bool sign );
    void rescale( const double slope, const double intersept );
    void setLevel( const int level );
    void setWidth( const int width );

public:

    int minValue() const;
    int maxValue() const;
    int level() const;
    int width() const;

public:

    int clampLevel( const int level );
    int clampWidth( const int width );

protected:

    void set_range_8bit( const bool sign );
    void set_range_12bit( const bool sign );
    void set_range_16bit( const bool sign );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__WINDOW_H_INCLUDE
