/*****************************************************************************/
/**
 *  @file   Support.h
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
#pragma once
#include <string>
#include <ios>
#include <iomanip>
#include <iostream>
#include <kvs/Program>
#include <kvs/StringList>
#include <kvs/String>
#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif
#if defined( KVS_SUPPORT_OPENCV )
#include <kvs/opencv/OpenCV>
#endif
#if defined( KVS_SUPPORT_CUDA )
#include <kvs/cuda/CUDA>
#endif
#if defined( KVS_SUPPORT_PYTHON )
#include <kvs/python/Python>
#endif
#if defined( KVS_SUPPORT_EGL )
#include <kvs/egl/EGL>
#endif
#if defined( KVS_SUPPORT_OSMESA )
#include <kvs/osmesa/OSMesa>
#endif


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  KVS support library checker class.
 */
/*===========================================================================*/
class Support : public kvs::Program
{
    int exec( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Executes the checker program.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return 0 if the process is done sucessfully
 */
/*===========================================================================*/
inline int Support::exec( int argc, char** argv )
{
    kvs::StringList libraries; // Support library list
    kvs::StringList descriptions; // Support library description list
    kvs::StringList versions; // Support library version list

#if defined( KVS_SUPPORT_GLUT )
    {
        libraries.push_back( "KVS_SUPPORT_GLUT" );
        descriptions.push_back( kvs::glut::Description() );
        versions.push_back( kvs::glut::Version() );
    }
#endif

#if defined( KVS_SUPPORT_QT )
    {
        libraries.push_back( "KVS_SUPPORT_QT" );
        descriptions.push_back( "Qt - Cross-Platform Rich Client Development Framework" );
        versions.push_back( "please type 'qmake -query QT_VERSION'" );
    }
#endif

#if defined( KVS_SUPPORT_OPENCV )
    {
        libraries.push_back( "KVS_SUPPORT_OPENCV" );
        descriptions.push_back( kvs::opencv::Description() );
        versions.push_back( kvs::opencv::Version() );
    }
#endif

#if defined( KVS_SUPPORT_CUDA )
    {
        libraries.push_back( "KVS_SUPPORT_CUDA" );
        descriptions.push_back( kvs::cuda::Description() );
        versions.push_back( kvs::cuda::Version() );
    }
#endif

#if defined( KVS_SUPPORT_PYTHON )
    {
        libraries.push_back( "KVS_SUPPORT_PYTHON" );
        descriptions.push_back( kvs::python::Description() );
        versions.push_back( kvs::python::Version() );
    }
#endif

#if defined( KVS_SUPPORT_EGL )
    {
        libraries.push_back( "KVS_SUPPORT_EGL" );
        descriptions.push_back( kvs::egl::Description() );
        versions.push_back( kvs::egl::Version() );
    }
#endif

#if defined( KVS_SUPPORT_OSMESA )
    {
        libraries.push_back( "KVS_SUPPORT_OSMESA" );
        descriptions.push_back( kvs::osmesa::Description() );
        versions.push_back( kvs::osmesa::Version() );
    }
#endif

    size_t length = libraries[0].size();
    const size_t nlibraries = libraries.size();
    for ( size_t i = 0; i < nlibraries; i++ )
    {
        length = kvs::Math::Max( length, libraries[i].size() );
    }

    for ( size_t i = 0; i < nlibraries; i++ )
    {
        std::cout
            << std::left << std::setw( length + 1 )
            << libraries.at(i) << ": "
            << descriptions.at(i)
            << " (version: "<< versions.at(i) << ")"
            << std::endl;
    }

    return 0;
}

} // end of namespace kvscheck
