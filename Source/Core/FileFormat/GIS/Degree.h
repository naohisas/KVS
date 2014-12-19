/*****************************************************************************/
/**
 *  @file   Degree.h
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
#ifndef KVS__GIS__DEGREE_H_INCLUDE
#define KVS__GIS__DEGREE_H_INCLUDE


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Degree class.
 */
/*===========================================================================*/
class Degree
{
public:

    enum Type
    {
        Decimal = 0,
        Nexadecimal = 1
    };

private:

    int m_degree; ///< degree
    int m_minute; ///< minute
    float m_second; ///< second

public:

    Degree();
    Degree( const float degree, const Type type = Decimal );
    Degree( const int degree, const int minute, const float second );
    Degree( const int degree );

public:

    void setDecimal( const float degree );
    void setNexadecimal( const float degree );
    float decimal() const;
    float nexadecimal() const;
    int degree() const;
    int minute() const;
    float second() const;
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__DEGREE_H_INCLUDE
