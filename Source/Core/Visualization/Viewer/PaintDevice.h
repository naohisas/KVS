#pragma once
#include <kvs/NanoVG>
#include <kvs/FontStash>


namespace kvs
{

class PaintDevice
{
private:
    kvs::NanoVG* m_render_engine;
    kvs::FontStash* m_text_engine;

public:
    PaintDevice();
    ~PaintDevice();

    kvs::NanoVG* renderEngine() { return m_render_engine; }
    kvs::FontStash* textEngine() { return m_text_engine; }
    void create();
};

} // end of namespace kvs
