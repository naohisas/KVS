/*****************************************************************************/
/**
 *  @file   OpenCV.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpenCV.h 1309 2012-09-18 04:29:34Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__OPENCV__OPENCV_H_INCLUDE
#define KVS__OPENCV__OPENCV_H_INCLUDE

#include <cv.h>
#include <highgui.h>
#include <kvs/String>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Returns OpenCV description
 *  @return description
 */
/*===========================================================================*/
inline const std::string Description()
{
    const std::string description( "OpenCV - Intel Open Source Computer Vision Library" );
    return description;
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenCV version.
 *  @return OpenCV version
 */
/*===========================================================================*/
inline const std::string Version()
{
    const std::string version = kvs::String::ToString( CV_VERSION );
    return version;
}

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__OPENCV_H_INCLUDE
