/*****************************************************************************/
/**
 *  @file   TransferFunctionEditor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

/*===========================================================================*/
/**
 *  @typedef TransferFunctionEditor
 *  @brief   Type definition of TransferFunctionEditor class depends on graphics display API.
 */
/*===========================================================================*/

#if defined( KVS_SUPPORT_GLUT ) && defined( KVS_APP_USE_GLUT )
#include <kvs/glut/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::glut::TransferFunctionEditor; }

#elif defined( KVS_SUPPORT_GLFW ) && defined( KVS_APP_USE_GLFW )
#include <kvs/glfw/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::glfw::TransferFunctionEditor; }

#elif defined( KVS_SUPPORT_QT ) && defined( KVS_APP_USE_QT )
#include <kvs/qt/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::qt::TransferFunctionEditor; }

#else

#if defined( KVS_SUPPORT_GLFW )
#include <kvs/glfw/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::glfw::TransferFunctionEditor; }

#elif defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::glut::TransferFunctionEditor; }

#elif defined( KVS_SUPPORT_QT )
#include <kvs/qt/TransferFunctionEditor>
namespace kvs { using TransferFunctionEditor = kvs::qt::TransferFunctionEditor; }

#else
#include "TransferFunctionEditorBase.h"
namespace kvs { using TransferFunctionEditor = kvs::TransferFunctionEditorBase; }
#endif

#endif
