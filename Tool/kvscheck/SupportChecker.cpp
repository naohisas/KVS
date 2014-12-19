/****************************************************************************/
/**
 *  @file SupportChecker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SupportChecker.cpp 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#if defined( KVS_SUPPORT_GLEW )
#include <kvs/glew/GLEW>
#endif

#include "SupportChecker.h"
#include <kvs/String>

#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif

#if defined( KVS_SUPPORT_SAGE )
#include <kvs/sage/SAGE>
#endif

#if defined( KVS_SUPPORT_OPENCV )
#include <kvs/opencv/OpenCV>
#endif

#if defined( KVS_SUPPORT_CUDA )
#include <kvs/cuda/CUDA>
#endif


namespace kvscheck
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new SupportChecker class.
 */
/*==========================================================================*/
SupportChecker::SupportChecker()
{
#if defined( KVS_SUPPORT_GLUT )
    {
        m_descriptions.push_back( kvs::glut::Description() );
        m_versions.push_back( kvs::glut::Version() );
    }
#endif

#if defined( KVS_SUPPORT_GLEW )
    {
        m_descriptions.push_back( kvs::glew::Description() );
        m_versions.push_back( kvs::glew::Version() );
    }
#endif

#if defined( KVS_SUPPORT_SAGE )
    {
        m_descriptions.push_back( kvs::sage::Description() );
        m_versions.push_back( kvs::sage::Version() );
    }
#endif

#if defined( KVS_SUPPORT_QT )
    {
        m_descriptions.push_back( "Qt - Cross-Platform Rich Client Development Framework" );
        m_versions.push_back( "please type 'qmake -v'" );
    }
#endif

#if defined( KVS_SUPPORT_OPENCV )
    {
        m_descriptions.push_back( kvs::opencv::Description() );
        m_versions.push_back( kvs::opencv::Version() );
    }
#endif

#if defined( KVS_SUPPORT_CUDA )
    {
        m_descriptions.push_back( kvs::cuda::Description() );
        m_versions.push_back( kvs::cuda::Version() );
    }
#endif
}

/*==========================================================================*/
/**
 *  @brief  Returns a description list of the supported libraries.
 *  @return description list of the supported libraries
 */
/*==========================================================================*/
const kvs::StringList& SupportChecker::descriptionList() const
{
    return m_descriptions;
}

/*==========================================================================*/
/**
 *  @brief  Returns a version list of the supported libraries.
 *  @return version list of the supported libraries
 */
/*==========================================================================*/
const kvs::StringList& SupportChecker::versionList() const
{
    return m_versions;
}

/*==========================================================================*/
/**
 *  @brief  Outputs supported libraries information.
 *  @param  os [in] output stream
 *  @param  checker [in] supported library checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const SupportChecker& checker )
{
    os << "Supported libraries :" << std::endl;

    const size_t nlibraries = checker.descriptionList().size();
    for ( size_t i = 0; i < nlibraries; i++ )
    {
        os << checker.descriptionList().at(i)
           << " (version: "<< checker.versionList().at(i) << ")";

        if ( i != nlibraries - 1 ) std::cout << std::endl;
    }

    return os;
}

} // end of namespace kvscheck
