/*****************************************************************************/
/**
 *  @file   RGBFormulae.h
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
#ifndef KVS__RGB_FORMULAE_H_INCLUDE
#define KVS__RGB_FORMULAE_H_INCLUDE

#include <kvs/ColorMap>


namespace kvs
{

namespace RGBFormulae
{

kvs::ColorMap Create( const int index0, const int index1, const int index2, const size_t resolution );

/*===========================================================================*/
/**
 *  @brief  Returns PM3D colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap PM3D( const size_t resolution )
{
    return Create( 7, 5, 15, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns GreenRedViolet colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap GreenRedViolet( const size_t resolution )
{
    return Create( 3, 11, 6, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns Ocean colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap Ocean( const size_t resolution )
{
    return Create( 23, 28, 3, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns Hot colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap Hot( const size_t resolution )
{
    return Create( 21, 22, 23, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorPrintable colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap ColorPrintable( const size_t resolution )
{
    return Create( 30, 31, 32, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns Rainbow colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap Rainbow( const size_t resolution )
{
    return Create( 33, 13, 10, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns AFMHot colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap AFMHot( const size_t resolution )
{
    return Create( 34, 35, 36, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns Jet colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap Jet( const size_t resolution )
{
    return Create( 37, 38, 39, resolution );
}

/*===========================================================================*/
/**
 *  @brief  Returns Bone colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap Bone( const size_t resolution )
{
    return Create( 40, 41, 42, resolution );
}

} // end of namespace RGBFormulae

} // end of namespace kvs

#endif // KVS__RGB_FORMULAE_H_INCLUDE
