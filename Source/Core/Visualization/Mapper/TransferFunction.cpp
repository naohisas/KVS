/****************************************************************************/
/**
 *  @file   TransferFunction.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunction.cpp 1620 2013-07-21 09:29:00Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "TransferFunction.h"

#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/KVSMLTransferFunction>


namespace
{

float Interpolate(
    const float s,
    const float s0,
    const float s1,
    const float S0,
    const float S1 )
{
    const float w = ( s - s0 ) / ( s1 - s0 );
    return kvs::Math::Mix( S0, S1, w );
}

}

namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new TransferFunction.
 *  @param  resolution [in] Resolution.
 */
/*==========================================================================*/
TransferFunction::TransferFunction( const size_t resolution ):
    m_color_map( resolution ),
    m_opacity_map( resolution )
{
    m_color_map.create();
    m_opacity_map.create();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new TransferFunction.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
TransferFunction::TransferFunction( const std::string& filename )
{
    this->read( filename );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new TransferFunction.
 *  @param  color_map [in] Color map.
 */
/*==========================================================================*/
TransferFunction::TransferFunction( const kvs::ColorMap& color_map ):
    m_color_map( color_map ),
    m_opacity_map( color_map.resolution() )
{
    m_opacity_map.create();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new TransferFunction.
 *  @param  opacity_map [in] Opacity map.
 */
/*==========================================================================*/
TransferFunction::TransferFunction( const kvs::OpacityMap& opacity_map ):
    m_color_map( opacity_map.resolution() ),
    m_opacity_map( opacity_map )
{
    m_color_map.create();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new TransferFunction.
 *  @param  color_map [in] Color map.
 *  @param  opacity_map [in] Opacity map.
 */
/*==========================================================================*/
TransferFunction::TransferFunction(
    const kvs::ColorMap& color_map,
    const kvs::OpacityMap& opacity_map ):
    m_color_map( color_map ),
    m_opacity_map( opacity_map )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a copy of other.
 *  @param  other [in] Transfer function.
 */
/*==========================================================================*/
TransferFunction::TransferFunction( const TransferFunction& other ):
    m_color_map( other.m_color_map ),
    m_opacity_map( other.m_opacity_map )
{
}

/*==========================================================================*/
/**
 *  @brief  Destroys TransferFunction.
 */
/*==========================================================================*/
TransferFunction::~TransferFunction()
{
}

/*==========================================================================*/
/**
 *  @brief  Set color map.
 *  @param  color_map [in] color map
 */
/*==========================================================================*/
void TransferFunction::setColorMap( const kvs::ColorMap& color_map )
{
    m_color_map = color_map;
}

/*==========================================================================*/
/**
 *  @brief  Set opacity map.
 *  @param  opacity_map [in] opacity map
 */
/*==========================================================================*/
void TransferFunction::setOpacityMap( const kvs::OpacityMap& opacity_map )
{
    m_opacity_map = opacity_map;
}

/*===========================================================================*/
/**
 *  @brief  Sets min. and max. values.
 *  @param  min_value [in] min. value
 *  @param  max_value [in] max. value
 */
/*===========================================================================*/
void TransferFunction::setRange( const float min_value, const float max_value )
{
    m_color_map.setRange( min_value, max_value );
    m_opacity_map.setRange( min_value, max_value );
}

/*===========================================================================*/
/**
 *  @brief  Sets min. and max. values.
 *  @param  volume [in] pointer to volume object
 */
/*===========================================================================*/
void TransferFunction::setRange( const kvs::VolumeObjectBase* volume )
{
    const float min_value = static_cast<float>( volume->minValue() );
    const float max_value = static_cast<float>( volume->maxValue() );
    this->setRange( min_value, max_value );
}

/*===========================================================================*/
/**
 *  @brief  Adjusts min. and max. values.
 *  @param  min_value [in] min. value
 *  @param  max_value [in] max. value
 */
/*===========================================================================*/
void TransferFunction::adjustRange( const float min_value, const float max_value )
{
    if ( this->hasRange() )
    {
        // Original range.
        const float s0 = this->minValue();
        const float s1 = this->maxValue();

        // Adjusted range.
        const float S0 = min_value;
        const float S1 = max_value;

        // Adjust color map.
        {
            kvs::ColorMap::Points::const_iterator point = m_color_map.points().begin();
            kvs::ColorMap::Points::const_iterator last = m_color_map.points().end();
            while ( point != last )
            {
                const float s = point->first;
                const float S = ::Interpolate( s, s0, s1, S0, S1 );
                const kvs::RGBColor c = point->second;

                m_color_map.removePoint( s );
                m_color_map.addPoint( S, c );

                point++;
            }
        }

        // Adjust opacity map.
        {
            kvs::OpacityMap::Points::const_iterator point = m_opacity_map.points().begin();
            kvs::OpacityMap::Points::const_iterator last = m_opacity_map.points().end();
            while ( point != last )
            {
                const float s = point->first;
                const float S = ::Interpolate( s, s0, s1, S0, S1 );
                const float a = point->second;

                m_opacity_map.removePoint( s );
                m_opacity_map.addPoint( S, a );

                point++;
            }
        }
    }

    this->setRange( min_value, max_value );
}

/*===========================================================================*/
/**
 *  @brief  Adjusts min. and max. values.
 *  @param  volume [in] pointer to volume object
 */
/*===========================================================================*/
void TransferFunction::adjustRange( const kvs::VolumeObjectBase* volume )
{
    const float min_value = static_cast<float>( volume->minValue() );
    const float max_value = static_cast<float>( volume->maxValue() );
    this->adjustRange( min_value, max_value );
}

/*===========================================================================*/
/**
 *  @brief  Return true if the range is specified.
 *  @return true if the range is specified
 */
/*===========================================================================*/
bool TransferFunction::hasRange() const
{
    return m_color_map.hasRange() && m_opacity_map.hasRange();
}

/*===========================================================================*/
/**
 *  @brief  Return min. value.
 *  @return min. value
 */
/*===========================================================================*/
float TransferFunction::minValue() const
{
    KVS_ASSERT( m_color_map.minValue() == m_opacity_map.minValue() );
    return m_color_map.minValue();
}

/*===========================================================================*/
/**
 *  @brief  Return max. value.
 *  @return max. value
 */
/*===========================================================================*/
float TransferFunction::maxValue() const
{
    KVS_ASSERT( m_color_map.maxValue() == m_opacity_map.maxValue() );
    return m_color_map.maxValue();
}

/*==========================================================================*/
/**
 *  @brief  Returns the color map.
 *  @retunr color map
 */
/*==========================================================================*/
const kvs::ColorMap& TransferFunction::colorMap() const
{
    return m_color_map;
}

/*==========================================================================*/
/**
 *  @brief  Returns the opacity map.
 *  @retunr opacity map
 */
/*==========================================================================*/
const kvs::OpacityMap& TransferFunction::opacityMap() const
{
    return m_opacity_map;
}

/*==========================================================================*/
/**
 *  @brief  Returns the resolution.
 *  @return resolution
 */
/*==========================================================================*/
size_t TransferFunction::resolution() const
{
    KVS_ASSERT( m_opacity_map.resolution() == m_color_map.resolution() );
    return m_opacity_map.resolution();
}

/*===========================================================================*/
/**
 *  @brief  Returns RGBA table as value array.
 *  @return RGBA table
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> TransferFunction::table() const
{
    const size_t width = this->resolution();
    kvs::ValueArray<kvs::Real32> table( width * 4 );
    float* data = table.data();

    const kvs::ColorMap& cmap = this->colorMap();
    const kvs::OpacityMap& omap = this->opacityMap();
    for ( size_t i = 0; i < width; i++ )
    {
        *(data++) = static_cast<float>( cmap[i].r() ) / 255.0f;
        *(data++) = static_cast<float>( cmap[i].g() ) / 255.0f;
        *(data++) = static_cast<float>( cmap[i].b() ) / 255.0f;
        *(data++) = omap[i];
    }

    return table;
}

/*==========================================================================*/
/**
 *  @brief  Create the alpha map.
 *  @param  resolution [in] resolution
 */
/*==========================================================================*/
void TransferFunction::create( const size_t resolution )
{
    m_opacity_map.setResolution( resolution );
    m_opacity_map.create();

    m_color_map.setResolution( resolution );
    m_color_map.create();
}

/*==========================================================================*/
/**
 *  @brief  Read a trasfer function data from the given file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool TransferFunction::read( const std::string& filename )
{
    kvs::KVSMLTransferFunction transfer_function( filename );
    if ( transfer_function.isFailure() ) return false;

    const float min_value = transfer_function.minValue();
    const float max_value = transfer_function.maxValue();
    const bool has_range = !( kvs::Math::Equal( min_value, max_value ) );
    if ( has_range )
    {
        m_color_map.setRange( min_value, max_value );
        m_opacity_map.setRange( min_value, max_value );
    }

    m_color_map.setResolution( transfer_function.resolution() );
    if ( transfer_function.colorPointList().size() > 0 )
    {
        typedef kvs::KVSMLTransferFunction::ColorPointList::const_iterator Iter;
        Iter point = transfer_function.colorPointList().begin();
        Iter last = transfer_function.colorPointList().end();
        while ( point != last )
        {
            const float value = point->first;
            const kvs::RGBColor color = point->second;
            m_color_map.addPoint( value, color );
            point++;
        }

        m_color_map.create();
    }
    else
    {
        m_color_map = kvs::ColorMap( transfer_function.colors() );
    }

    m_opacity_map.setResolution( transfer_function.resolution() );
    if ( transfer_function.opacityPointList().size() > 0 )
    {
        typedef kvs::KVSMLTransferFunction::OpacityPointList::const_iterator Iter;
        Iter point = transfer_function.opacityPointList().begin();
        Iter last = transfer_function.opacityPointList().end();
        while ( point != last )
        {
            const float value = point->first;
            const kvs::Real32 opacity = point->second;
            m_opacity_map.addPoint( value, opacity );
            point++;
        }

        m_opacity_map.create();
    }
    else
    {
        m_opacity_map = kvs::OpacityMap( transfer_function.opacities() );
    }

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Writes the trasfer function data.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool TransferFunction::write( const std::string& filename )
{
    kvs::KVSMLTransferFunction transfer_function;
    transfer_function.setResolution( this->resolution() );
    transfer_function.setWritingDataType( kvs::KVSMLTransferFunction::Ascii );

    if ( m_color_map.points().size() > 0 )
    {
        typedef kvs::ColorMap::Points::const_iterator Iter;
        Iter point = m_color_map.points().begin();
        Iter last = m_color_map.points().end();
        while ( point != last )
        {
            transfer_function.addColorPoint( point->first, point->second );
            point++;
        }
    }
    else
    {
        transfer_function.setColors( m_color_map.table() );
    }

    if ( m_opacity_map.points().size() > 0 )
    {
        typedef kvs::OpacityMap::Points::const_iterator Iter;
        Iter point = m_opacity_map.points().begin();
        Iter last = m_opacity_map.points().end();
        while ( point != last )
        {
            transfer_function.addOpacityPoint( point->first, point->second );
            point++;
        }
    }
    else
    {
        transfer_function.setOpacities( m_opacity_map.table() );
    }

    return transfer_function.write( filename );
}

/*==========================================================================*/
/**
 *  @brief  Substitution operator =.
 *  @param  trunsfer_function [in] transfer function
 *  @retval transfer function
 */
/*==========================================================================*/
TransferFunction& TransferFunction::operator =( const TransferFunction& rhs )
{
    m_color_map = rhs.m_color_map;
    m_opacity_map = rhs.m_opacity_map;

    return *this;
}

} // end of namespace kvs
