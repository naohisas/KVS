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
#include <kvs/Deprecated>


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
    /*
    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    */

    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)
    size_t m_active_axis; ///< index of active axis
    kvs::UInt8 m_line_opacity; ///< line opacity
    kvs::Real32 m_line_width; ///< line width
    kvs::ColorMap m_color_map; ///< color map

public:
    ParallelCoordinatesRenderer();

    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }
    void setLineOpacity( const kvs::UInt8 opacity ) { m_line_opacity = opacity; }
    void setLineWidth( const kvs::Real32 width ) { m_line_width = width; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map; }
    void selectAxis( const size_t index ) { m_active_axis = index; }

    const kvs::Margins& margins() const { return m_margins; }
    size_t activeAxis() const { return m_active_axis; }
    kvs::UInt8 lineOpacity() const { return m_line_opacity; }
    kvs::Real32 lineWidth() const { return m_line_width; }
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

public:
    KVS_DEPRECATED( void setTopMargin( const int margin ) ) { m_margins.setTop( margin ); }
    KVS_DEPRECATED( void setBottomMargin( const int margin ) ) { m_margins.setBottom( margin ); }
    KVS_DEPRECATED( void setLeftMargin( const int margin ) ) { m_margins.setLeft( margin ); }
    KVS_DEPRECATED( void setRightMargin( const int margin ) ) { m_margins.setRight( margin ); }
    KVS_DEPRECATED( int topMargin() const ) { return m_margins.top(); }
    KVS_DEPRECATED( int bottomMargin() const ) { return m_margins.bottom(); }
    KVS_DEPRECATED( int leftMargin() const ) { return m_margins.left(); }
    KVS_DEPRECATED( int rightMargin() const ) { return m_margins.right(); }
};

} // end of namespace kvs
