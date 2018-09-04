/*****************************************************************************/
/**
 *  @file   GL.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "GL.h"
#include <string>
#include <iostream>
#include <kvs/ColorStream>
#include <kvs/IgnoreUnusedVariable>


namespace
{

inline std::string GetErrorString( const GLenum error )
{
    switch ( error )
    {
    case GL_NO_ERROR: return "No error.";
    case GL_INVALID_ENUM: return "Invalid enumerant.";
    case GL_INVALID_VALUE: return "Invalid value.";
    case GL_INVALID_OPERATION: return "Invalid operation.";
#ifndef GL_VERSION_3_0
    case GL_STACK_OVERFLOW: return "Stack overflow.";
    case GL_STACK_UNDERFLOW: return "Stack underflow.";
    case GL_TABLE_TOO_LARGE: return "Table too large";
#endif
    case GL_OUT_OF_MEMORY: return "Out of memory.";
#ifdef GL_EXT_framebuffer_object
    case GL_INVALID_FRAMEBUFFER_OPERATION_EXT: return "Invalid framebuffer operation.";
#endif
    default: break;
    }

    return "Unknown error code.";
}

} // end of namespace


namespace kvs
{

namespace GL
{

namespace detail
{

bool HasError( const char* file, const int line, const char* func, const char* command )
{
#if defined( KVS_ENABLE_OPENGL )
    GLenum error = glGetError();
    if ( error == GL_NO_ERROR ) { return false; }

    // Output message tag.
    std::cerr << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "KVS GL ERROR"
              << kvs::ColorStream::Reset
              << ": ";

    // Output message with an error string.
    const std::string message = ::GetErrorString( error );
    std::cerr << kvs::ColorStream::Underline( kvs::ColorStream::Bold( message ) ) << std::endl;
    std::cerr << "    " << "Func: " << func << std::endl;
    std::cerr << "    " << "File: " << file << ":" << line << std::endl;
    std::cerr << "    " << "GL Command: " << command << std::endl;

    return true;
#else
    kvs::IgnoreUnusedVariable( file );
    kvs::IgnoreUnusedVariable( line );
    kvs::IgnoreUnusedVariable( func );
    kvs::IgnoreUnusedVariable( command );
    return false;
#endif
}

} // end of namespace detail

} // end of namespace GL

} // end of namespace kvs
