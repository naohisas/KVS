/****************************************************************************/
/**
 *  @file   TransferFunction.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunction.h 1620 2013-07-21 09:29:00Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TRANSFER_FUNCTION_H_INCLUDE
#define KVS__TRANSFER_FUNCTION_H_INCLUDE

#include <kvs/ColorMap>
#include <kvs/OpacityMap>
#include <kvs/VolumeObjectBase>
#include <kvs/ValueArray>


namespace kvs
{

/*==========================================================================*/
/**
 *  TransferFunction.
 */
/*==========================================================================*/
class TransferFunction
{
private:

    kvs::ColorMap m_color_map; ///< Color map.
    kvs::OpacityMap m_opacity_map; ///< Opacity map.

public:

    explicit TransferFunction( const size_t resolution = 256 );
    TransferFunction( const std::string& filename );
    TransferFunction( const kvs::ColorMap& color_map );
    TransferFunction( const kvs::OpacityMap& opacity_map );
    TransferFunction( const kvs::ColorMap& color_map, const kvs::OpacityMap& opacity_map );
    TransferFunction( const TransferFunction& other );
    virtual ~TransferFunction();

    void setColorMap( const kvs::ColorMap& color_map );
    void setOpacityMap( const kvs::OpacityMap& opacity_map );
    void setRange( const float min_value, const float max_value );
    void setRange( const kvs::VolumeObjectBase* volume );
    void adjustRange( const float min_value, const float max_value );
    void adjustRange( const kvs::VolumeObjectBase* volume );

    bool hasRange() const;
    float minValue() const;
    float maxValue() const;
    const kvs::ColorMap& colorMap() const;
    const kvs::OpacityMap& opacityMap() const;
    size_t resolution() const;
    kvs::ValueArray<kvs::Real32> table() const;

    void create( const size_t resolution );
    bool read( const std::string& filename );
    bool write( const std::string& filename );

    TransferFunction& operator =( const TransferFunction& rhs );
};

} // end of namespace kvs

#endif // KVS__TRANSFER_FUNCTION_H_INCLUDE
