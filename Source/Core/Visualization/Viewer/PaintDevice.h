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
    kvs::NanoVG* m_render_engine; ///< render engin based on NanoVG
    kvs::FontStash* m_text_engine; ///< text rendering engin

public:
    PaintDevice();
    ~PaintDevice();

    kvs::NanoVG* renderEngine() { return m_render_engine; }
    kvs::FontStash* textEngine() { return m_text_engine; }
    void create();
};

} // end of namespace kvs
