/*****************************************************************************/
/**
 *  @file   GlyphBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GlyphBase.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLYPH_BASE_H_INCLUDE
#define KVS__GLYPH_BASE_H_INCLUDE

#include <kvs/RendererBase>
#include <kvs/ValueArray>
#include <kvs/Vector3>
#include <kvs/TransferFunction>


namespace kvs
{

class VolumeObjectBase;
class StructuredVolumeObject;

/*===========================================================================*/
/**
 *  @brief  Glyph renderer base class.
 */
/*===========================================================================*/
class GlyphBase : public kvs::RendererBase
{
    kvsModule( kvs::GlyphBase, Renderer );

public:

    enum SizeMode
    {
        SizeByDefault = 0,
        SizeByMagnitude
    };

    enum DirectionMode
    {
        DirectionByDefault = 0,
        DirectionByNormal
    };

    enum ColorMode
    {
        ColorByDefault = 0,
        ColorByMagnitude
    };

    enum OpacityMode
    {
        OpacityByDefault = 0,
        OpacityByMagnitude
    };

private:

    SizeMode m_size_mode; ///< size mode
    DirectionMode m_direction_mode; ///< direction mode
    ColorMode m_color_mode; ///< color mode
    OpacityMode m_opacity_mode; ///< opacity mode
    kvs::ValueArray<kvs::Real32> m_coords; ///< coordinate value array
    kvs::ValueArray<kvs::Real32> m_sizes; ///< size value array
    kvs::ValueArray<kvs::Real32> m_directions; ///< direction vector array
    kvs::ValueArray<kvs::UInt8> m_colors; ///< color value array
    kvs::ValueArray<kvs::UInt8> m_opacities; ///< opacity value array
    kvs::Vector3f m_scale; ///< scaling vector
    kvs::TransferFunction m_tfunc; ///< transfer function

public:

    GlyphBase();
    virtual ~GlyphBase();

    virtual void exec( kvs::ObjectBase* object, kvs::Camera* camera = NULL, kvs::Light* light = NULL ) = 0;

    void setSizeMode( const SizeMode mode ) { m_size_mode = mode; }
    void setDirectionMode( const DirectionMode mode ) { m_direction_mode = mode; }
    void setColorMode( const ColorMode mode ) { m_color_mode = mode; }
    void setOpacityMode( const OpacityMode mode ) { m_opacity_mode = mode; }
    void setCoords( const kvs::ValueArray<kvs::Real32>& coords ) { m_coords = coords; }
    void setSizes( const kvs::ValueArray<kvs::Real32>& sizes ) { m_sizes = sizes; }
    void setDirections( const kvs::ValueArray<kvs::Real32>& directions ) { m_directions = directions; }
    void setColors( const kvs::ValueArray<kvs::UInt8>& colors ) { m_colors = colors; }
    void setOpacities( const kvs::ValueArray<kvs::UInt8>& opacities ) { m_opacities = opacities; }
    void setScale( const kvs::Real32 scale ) { m_scale = kvs::Vec3::Constant( scale ); }
    void setScale( const kvs::Vec3& scale ) { m_scale = scale; }
    void setTransferFunction( const kvs::TransferFunction& tfunc ) { m_tfunc = tfunc; }
    SizeMode sizeMode() const { return m_size_mode; }
    DirectionMode directionMode() const { return m_direction_mode; }
    ColorMode colorMode() const { return m_color_mode; }
    OpacityMode opacityMode() const { return m_opacity_mode; }
    const kvs::ValueArray<kvs::Real32>& coords() const { return m_coords; }
    const kvs::ValueArray<kvs::Real32>& sizes() const { return m_sizes; }
    const kvs::ValueArray<kvs::Real32>& directions() const { return m_directions; }
    const kvs::ValueArray<kvs::UInt8>& colors() const { return m_colors; }
    const kvs::ValueArray<kvs::UInt8>& opacities() const { return m_opacities; }
    const kvs::Vec3& scale() const { return m_scale; }
    const kvs::TransferFunction& transferFunction() const { return m_tfunc; }

protected:

    void transform( const kvs::Vec3& position, const kvs::Real32 size );
    void transform( const kvs::Vec3& position, const kvs::Vec3& direction, const kvs::Real32 size );
    void calculateCoords( const kvs::VolumeObjectBase* volume );
    void calculateCoords( const kvs::StructuredVolumeObject* volume );
    void calculateUniformCoords( const kvs::StructuredVolumeObject* volume );
    void calculateRectilinearCoords( const kvs::StructuredVolumeObject* volume );
    template <typename T> void calculateSizes( const kvs::VolumeObjectBase* volume );
    template <typename T> void calculateDirections( const kvs::VolumeObjectBase* volume );
    template <typename T> void calculateColors( const kvs::VolumeObjectBase* volume );
    template <typename T> void calculateOpacities( const kvs::VolumeObjectBase* volume );
};

} // end of namespace kvs

#endif // KVS__GLYPH_BASE_H_INCLUDE
