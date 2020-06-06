/*****************************************************************************/
/**
 *  @file   Application.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

/*===========================================================================*/
/**
 *  @typedef Application
 *  @brief   Type definition of Application class depends on graphics display API.
 */
/*===========================================================================*/

#if defined( KVS_SUPPORT_GLUT ) && defined( KVS_APP_USE_GLUT )
#include <kvs/glut/Application>
namespace kvs { using Application = kvs::glut::Application; }

#elif defined( KVS_SUPPORT_GLFW ) && defined( KVS_APP_USE_GLFW )
#include <kvs/glfw/Application>
namespace kvs { using Application = kvs::glfw::Application; }

#elif defined( KVS_SUPPORT_QT ) && defined( KVS_APP_USE_QT )
#include <kvs/qt/Application>
namespace kvs { using Application = kvs::qt::Application; }

#else

#if defined( KVS_SUPPORT_GLFW )
#include <kvs/glfw/Application>
namespace kvs { using Application = kvs::glfw::Application; }

#elif defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/Application>
namespace kvs { using Application = kvs::glut::Application; }

#elif defined( KVS_SUPPORT_QT )
#include <kvs/glut/Application>
namespace kvs { using Application = kvs::glut::Application; }

#else
#include "ApplicationBase.h"
namespace kvs { using Application = kvs::ApplicationBase; }
#endif

#endif
