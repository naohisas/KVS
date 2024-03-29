/*****************************************************************************/
/**
 *  @file   ParallelCoordinatesRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ColorMap>
#include <kvs/Margins>
#include <kvs/TableObject>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Parallel coordinates renderer class.
 */
/*===========================================================================*/
class ParallelCoordinatesRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ParallelCoordinatesRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    kvs::Margins m_margins{ 30 };
    mutable bool m_enable_anti_aliasing = false; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing = false; ///< flag for multisample anti-aliasing (MSAA)
    size_t m_active_axis = 0; ///< index of active axis
    kvs::Real32 m_line_opacity = 1.0f; ///< line opacity
    kvs::Real32 m_line_width = 1.0f; ///< line width
    kvs::ColorMap m_color_map{ 256 }; ///< color map

public:
    ParallelCoordinatesRenderer() { m_color_map.create(); }

    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }
    void setLineOpacity( const kvs::Real32 opacity ) { m_line_opacity = opacity; }
    void setLineWidth( const kvs::Real32 width ) { m_line_width = width; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map; }
    void selectAxis( const size_t index ) { m_active_axis = index; }

    const kvs::Margins& margins() const { return m_margins; }
    const kvs::ColorMap& colorMap() const { return m_color_map; }
    size_t activeAxis() const { return m_active_axis; }
    kvs::Real32 lineOpacity() const { return m_line_opacity; }
    kvs::Real32 lineWidth() const { return m_line_width; }
    void setAntiAliasingEnabled( const bool aa = true, const bool msaa = false ) const;
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    void updateColorMapRange( const kvs::TableObject* table );
    void updateAntiAliasing();
};

} // end of namespace kvs
