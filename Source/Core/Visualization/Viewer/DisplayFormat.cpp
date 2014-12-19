/*****************************************************************************/
/**
 *  @file   DisplayFormat.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DisplayFormat.cpp 1422 2013-03-02 06:32:39Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "DisplayFormat.h"


namespace kvs
{

DisplayFormat::DisplayFormat():
    m_double_buffer(true),
    m_color_buffer(true),
    m_depth_buffer(true),
    m_accumulation_buffer(false),
    m_stencil_buffer(false),
    m_stereo_buffer(false),
    m_multisample_buffer(false),
    m_alpha_channel(false)
{
}

bool DisplayFormat::doubleBuffer() const
{
    return m_double_buffer;
}

bool DisplayFormat::colorBuffer() const
{
    return m_color_buffer;
}

bool DisplayFormat::depthBuffer() const
{
    return m_depth_buffer;
}

bool DisplayFormat::accumulationBuffer() const
{
    return m_accumulation_buffer;
}

bool DisplayFormat::stencilBuffer() const
{
    return m_stencil_buffer;
}

bool DisplayFormat::stereoBuffer() const
{
    return m_stereo_buffer;
}

bool DisplayFormat::multisampleBuffer( ) const
{
    return m_multisample_buffer;
}

bool DisplayFormat::alphaChannel() const
{
    return m_alpha_channel;
}

void DisplayFormat::setDoubleBuffer( const bool enable )
{
    m_depth_buffer = enable;
}

void DisplayFormat::setColorBuffer( const bool enable )
{
    m_color_buffer = enable;
}

void DisplayFormat::setDepthBuffer( const bool enable )
{
    m_depth_buffer = enable;
}

void DisplayFormat::setAccumulationBuffer( const bool enable )
{
    m_accumulation_buffer = enable;
}

void DisplayFormat::setStencilBuffer( const bool enable )
{
    m_stencil_buffer = enable;
}

void DisplayFormat::setStereoBuffer( const bool enable )
{
    m_stereo_buffer = enable;
}

void DisplayFormat::setMultisampleBuffer( const bool enable )
{
    m_multisample_buffer = enable;
}

void DisplayFormat::setAlphaChannel( const bool enable )
{
    m_alpha_channel = enable;
}

} // end of namespace kvs
