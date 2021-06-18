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
    bool m_double_buffer; ///< double buffering (true:double buffering, false:single buffering, default:true)
    bool m_color_buffer; ///< color buffer mode (true:RGBA mode, false:color index mode, default:true)
    bool m_depth_buffer; ///< depth buffer (true:enable, false:disable, default:true)
    bool m_accumulation_buffer; ///< accumulation buffer (true:enable, false:disable, default:false)
    bool m_stencil_buffer; ///< stencil buffer (true:enable, false:disable, default:false)
    bool m_stereo_buffer; ///< stereo buffer (true:enable, false:disable, default:false)
    bool m_multisample_buffer; ///< multisample buffer (true:enable, false:disable, default:false)
    bool m_alpha_channel; ///< alpha channel (true:enable, false:disable, default:false)

public:
    DisplayFormat():
        m_double_buffer( true ),
        m_color_buffer( true ),
        m_depth_buffer( true ),
        m_accumulation_buffer( false ),
        m_stencil_buffer( false ),
        m_stereo_buffer( false ),
        m_multisample_buffer( false ),
        m_alpha_channel( false ) {}

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
