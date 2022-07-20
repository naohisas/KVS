/*****************************************************************************/
/**
 *  @file   GlyphBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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

private:
    static kvs::Real32 m_default_size;
    static kvs::Vec3 m_default_direction;
    static kvs::RGBColor m_default_color;
    static kvs::UInt8 m_default_opacity;
public:
    static kvs::Real32 DefaultSize() { return m_default_size; }
    static kvs::Vec3 DefaultDirection() { return m_default_direction; }
    static kvs::RGBColor DefaultColor() { return m_default_color; }
    static kvs::UInt8 DefaultOpacity() { return m_default_opacity; }
    static void SetDefaultSize( const kvs::Real32 size ) { m_default_size = size; }
    static void SetDefaultDirection( const kvs::Vec3& dir ) { m_default_direction = dir; }
    static void SetDefaultColor( const kvs::RGBColor& color ) { m_default_color = color; }
    static void SetDefaultOpacity( const kvs::UInt8 opacity ) { m_default_opacity = opacity; }

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
    SizeMode m_size_mode = SizeMode::SizeByDefault; ///< size mode
    DirectionMode m_direction_mode = DirectionMode::DirectionByDefault; ///< direction mode
    ColorMode m_color_mode = ColorMode::ColorByMagnitude; ///< color mode
    OpacityMode m_opacity_mode = OpacityMode::OpacityByDefault; ///< opacity mode
    kvs::ValueArray<kvs::Real32> m_coords{}; ///< coordinate value array
    kvs::ValueArray<kvs::Real32> m_sizes{}; ///< size value array
    kvs::ValueArray<kvs::Real32> m_directions{}; ///< direction vector array
    kvs::ValueArray<kvs::UInt8> m_colors{}; ///< color value array
    kvs::ValueArray<kvs::UInt8> m_opacities{}; ///< opacity value array
    kvs::Vec3 m_scale{ 1.0f, 1.0f, 1.0f }; ///< scaling vector
    kvs::TransferFunction m_tfunc{}; ///< transfer function

public:
    GlyphBase() = default;
    virtual ~GlyphBase() = default;

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
