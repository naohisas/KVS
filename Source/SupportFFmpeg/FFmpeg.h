#pragma once
#include <SupportFFmpeg/AvCpp/ffmpeg.h>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Returns FFmpeg description
 *  @return description
 */
/*===========================================================================*/
inline const std::string Description()
{
    const std::string description( "FFmpeg - Multimedia framework for video and audio files." );
    return description;
}

/*===========================================================================*/
/**
 *  @brief  Returns FFmpeg version.
 *  @return FFmpeg version
 */
/*===========================================================================*/
inline const std::string Version()
{
    const std::string version( av_version_info() );
    return version;
}

} // end of namespace ffmpeg

} // end of namespace kvs
