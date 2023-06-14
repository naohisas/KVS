/*****************************************************************************/
/**
 *  @file   PaintDevice.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/NanoVG>
#include <kvs/FontStash>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Paint device class.
 */
/*===========================================================================*/
class PaintDevice
{
private:
    kvs::NanoVG* m_render_engine = nullptr; ///< render engin based on NanoVG
    kvs::FontStash* m_text_engine = nullptr; ///< text rendering engin

public:
    PaintDevice() = default;
    ~PaintDevice();

    kvs::NanoVG* renderEngine() { return m_render_engine; }
    kvs::FontStash* textEngine() { return m_text_engine; }
    void create();
};

} // end of namespace kvs
