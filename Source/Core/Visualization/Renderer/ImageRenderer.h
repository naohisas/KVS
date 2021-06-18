/****************************************************************************/
/**
 *  @file   ImageRenderer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include "RendererBase.h"
#include <kvs/Texture2D>
#include <kvs/ImageObject>
#include <kvs/Module>
#include <kvs/Deprecated>

namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Image renderer class.
 */
/*==========================================================================*/
class ImageRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ImageRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    bool m_enable_centering; ///< enable center alignment
    kvs::Texture2D m_texture; ///< texture image

public:
    ImageRenderer();

    bool isCenteringEnabled() const { return m_enable_centering; }
    void setCenteringEnabled( const bool enable = true ) { m_enable_centering = enable; }
    void enableCentering() { this->setCenteringEnabled( true ); }
    void disableCentering() { this->setCenteringEnabled( false ); }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Texture2D& texture() { return m_texture; }
    void createTexture( const kvs::ImageObject* image );
    void alignCenter( const kvs::Camera* camera );
    void textureMapping();

public:
    KVS_DEPRECATED( bool isEnabledCentering() const ) { return this->isCenteringEnabled(); }
    KVS_DEPRECATED( void setEnabledCentering( const bool enable ) ) { this->setCenteringEnabled( enable ); }
};

} // end of namespace kvs
