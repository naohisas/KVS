/*****************************************************************************/
/**
 *  @file   Screen.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

/*===========================================================================*/
/**
 *  @typedef Screen
 *  @brief   Type definition of Screen class depends on graphics display API.
 */
/*===========================================================================*/

#if defined( KVS_SUPPORT_GLUT ) && defined( KVS_APP_USE_GLUT )
#include <kvs/glut/Screen>
namespace kvs { using Screen = kvs::glut::Screen; }

#elif defined( KVS_SUPPORT_GLFW ) && defined( KVS_APP_USE_GLFW )
#include <kvs/glfw/Screen>
namespace kvs { using Screen = kvs::glfw::Screen; }

#elif defined( KVS_SUPPORT_QT ) && defined( KVS_APP_USE_QT )
#include <kvs/qt/Screen>
namespace kvs { using Screen = kvs::qt::Screen; }

#else

#if defined( KVS_SUPPORT_GLFW )
#include <kvs/glfw/Screen>
namespace kvs { using Screen = kvs::glfw::Screen; }

#elif defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/Screen>
namespace kvs { using Screen = kvs::glut::Screen; }

#elif defined( KVS_SUPPORT_QT )
#include <kvs/qt/Screen>
namespace kvs { using Screen = kvs::qt::Screen; }

#else
#include "ScreenBase.h"
namespace kvs { using Screen = kvs::ScreenBase; }
#endif

#endif
