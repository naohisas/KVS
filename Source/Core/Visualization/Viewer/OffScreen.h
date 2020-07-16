/*****************************************************************************/
/**
 *  @file   OffScreen.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

/*===========================================================================*/
/**
 *  @typedef OffScreen
 *  @brief   Type definition of Screen class for off-screen rendering.
 */
/*===========================================================================*/

#if defined( KVS_SUPPORT_OSMESA ) && defined( KVS_APP_USE_OSMESA )
#include <kvs/osmesa/Screen>
namespace kvs { using OffScreen = kvs::osmesa::Screen; }

#elif defined( KVS_SUPPORT_EGL ) && defined( KVS_APP_USE_EGL )
#include <kvs/egl/Screen>
namespace kvs { using OffScreen = kvs::egl::Screen; }

#else

#if defined( KVS_SUPPORT_OSMESA )
#include <kvs/osmesa/Screen>
namespace kvs { using OffScreen = kvs::osmesa::Screen; }

#elif defined( KVS_SUPPORT_EGL )
#include <kvs/egl/Screen>
namespace kvs { using OffScreen = kvs::egl::Screen; }

#else
#include "ScreenBase.h"
namespace kvs { using OffScreen = kvs::ScreenBase; }
#endif

#endif
