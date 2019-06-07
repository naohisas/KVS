#pragma once
#include "../OSMesa.h"
#include "Context.h"
#include "Surface.h"
#include <kvs/ScreenBase>
#include <kvs/ValueArray>
#include <kvs/ColorImage>


namespace kvs
{

namespace osmesa
{

class ScreenBase : public kvs::ScreenBase
{
    typedef kvs::ScreenBase BaseClass;

private:
    kvs::osmesa::Context m_context; ///< OSMesa rendering context
    kvs::osmesa::Surface m_surface; ///< OSMesa drawing surface

public:
    ScreenBase();
    virtual ~ScreenBase();

    const kvs::ValueArray<kvs::UInt8>& buffer() const { return m_surface.buffer(); }
    kvs::ValueArray<kvs::UInt8> readbackColorBuffer() const;
    kvs::ValueArray<kvs::Real32> readbackDepthBuffer() const;

    virtual void create();
    virtual void show();
    virtual void redraw();
    virtual void draw();
    virtual kvs::ColorImage capture() const;

    virtual void initializeEvent() {}
    virtual void paintEvent() {}
};

} // end of namespace osmesa

} // end of namepsace kvs
