/*****************************************************************************/
/**
 *  @file   DisplayFormat.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Display format class.
 */
/*===========================================================================*/
class DisplayFormat
{
private:
    bool m_double_buffer = true; ///< double buffering (true:double buffering, false:single buffering)
    bool m_color_buffer = true; ///< color buffer mode (true:RGBA mode, false:color index mode)
    bool m_depth_buffer = true; ///< depth buffer (true:enable, false:disable)
    bool m_accumulation_buffer = false; ///< accumulation buffer (true:enable, false:disable)
    bool m_stencil_buffer = false; ///< stencil buffer (true:enable, false:disable)
    bool m_stereo_buffer = false; ///< stereo buffer (true:enable, false:disable)
    bool m_multisample_buffer = false; ///< multisample buffer (true:enable, false:disable)
    bool m_alpha_channel = false; ///< alpha channel (true:enable, false:disable)

public:
    DisplayFormat() = default;

    bool doubleBuffer() const { return m_double_buffer; }
    bool colorBuffer() const { return m_color_buffer; }
    bool depthBuffer() const { return m_depth_buffer; }
    bool accumulationBuffer() const { return m_accumulation_buffer; }
    bool stencilBuffer() const { return m_stencil_buffer; }
    bool stereoBuffer() const { return m_stereo_buffer; }
    bool multisampleBuffer() const { return m_multisample_buffer; }
    bool alphaChannel() const { return m_alpha_channel; }

    void setDoubleBuffer( const bool enable ) { m_depth_buffer = enable; }
    void setColorBuffer( const bool enable ) { m_color_buffer = enable; }
    void setDepthBuffer( const bool enable ) { m_depth_buffer = enable; }
    void setAccumulationBuffer( const bool enable ) { m_accumulation_buffer = enable; }
    void setStencilBuffer( const bool enable ) { m_stencil_buffer = enable; }
    void setStereoBuffer( const bool enable ) { m_stereo_buffer = enable; }
    void setMultisampleBuffer( const bool enable ) { m_multisample_buffer = enable; }
    void setAlphaChannel( const bool enable ) { m_alpha_channel = enable; }
};

} // end of namespace kvs
