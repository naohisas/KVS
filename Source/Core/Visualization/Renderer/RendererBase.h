/****************************************************************************/
/**
 *  @file   RendererBase.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/Timer>
#include <kvs/Module>
#include <kvs/Deprecated>
#include <kvs/ScreenBase>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Renderer base class.
 */
/*==========================================================================*/
class RendererBase
{
    kvsModuleBase( kvs::RendererBase );

private:
    kvs::ScreenBase* m_screen = nullptr; ///< pointer to the screen
    std::string m_name = "unknown"; ///< renderer name
    kvs::Timer m_timer{}; ///< timer
    mutable bool m_enable_shading = true; ///< shading flag

public:
    RendererBase() = default;
    virtual ~RendererBase()  = default;

    const std::string& name() const { return m_name; }
    const kvs::Timer& timer() const { return m_timer; }

    void setScreen( kvs::ScreenBase* screen ) { m_screen = screen; }
    void setName( const std::string& name ) { m_name = name; }
    void setShadingEnabled( const bool enable = true ) { m_enable_shading = enable; }

    bool isShadingEnabled() const { return m_enable_shading; }
    void enableShading() const { m_enable_shading = true; }
    void disableShading() const { m_enable_shading = false; }

    virtual void exec( kvs::ObjectBase* object, kvs::Camera* camera = nullptr, kvs::Light* light  = nullptr ) = 0;

protected:
    kvs::ScreenBase* screen() { return m_screen; }
    void startTimer() { m_timer.start(); }
    void stopTimer() { m_timer.stop(); }

public:
    KVS_DEPRECATED( void setEnabledShading( const bool enable ) ) { this->setShadingEnabled( enable ); }
    KVS_DEPRECATED( bool isEnabledShading() const ) { return this->isShadingEnabled(); }
};

} // end of namespace kvs
