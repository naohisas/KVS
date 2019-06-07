#pragma once
#include "../EGL.h"
#include "Display.h"
#include "Context.h"
#include "Config.h"
#include "Surface.h"
#include <kvs/ScreenBase>
#include <kvs/ValueArray>
#include <kvs/ColorImage>


namespace kvs
{

namespace egl
{

class ScreenBase : public kvs::ScreenBase
{
    typedef kvs::ScreenBase BaseClass;

private:
    kvs::egl::Display m_display; ///< EGL display
    kvs::egl::Context m_context; ///< EGL rendering context
    kvs::egl::Config m_config; ///< EGL configulation
    kvs::egl::Surface m_surface; ///< EGL drawing surface

public:
    ScreenBase();
    virtual ~ScreenBase();

    kvs::ValueArray<kvs::UInt8> readbackColorBuffer() const;
    kvs::ValueArray<kvs::Real32> readbackDepthBuffer() const;
    void displayInfo();

    virtual void create();
    virtual void show();
    virtual void redraw();
    virtual void draw();
    virtual kvs::ColorImage capture() const;

    virtual void initializeEvent() {}
    virtual void paintEvent() {}

private:
    void errorMessage( const char* msg );
};

} // end of namespace egl

} // end of namepsace kvs
