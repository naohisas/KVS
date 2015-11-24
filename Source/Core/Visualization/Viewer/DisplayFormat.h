/*****************************************************************************/
/**
 *  @file   DisplayFormat.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DisplayFormat.h 1514 2013-04-09 07:14:05Z naohisa.sakamoto@gmail.com $
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

    DisplayFormat();

    bool doubleBuffer() const;
    bool colorBuffer() const;
    bool depthBuffer() const;
    bool accumulationBuffer() const;
    bool stencilBuffer() const;
    bool stereoBuffer() const;
    bool multisampleBuffer() const;
    bool alphaChannel() const;

    void setDoubleBuffer( const bool enable );
    void setColorBuffer( const bool enable );
    void setDepthBuffer( const bool enable );
    void setAccumulationBuffer( const bool enable );
    void setStencilBuffer( const bool enable );
    void setStereoBuffer( const bool enable );
    void setMultisampleBuffer( const bool enable );
    void setAlphaChannel( const bool enable );
};

} // end of namespace kvs
