/*****************************************************************************/
/**
 *  @file   Parameter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Parameter.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef __PARAPMETER_H__
#define __PARAPMETER_H__

#include "Argument.h"
#include <kvs/DicomList>
#include <kvs/Vector2>


/*===========================================================================*/
/**
 *  @brief  Global parameter set.
 */
/*===========================================================================*/
struct Parameter
{
    kvs::DicomList dicom_list; ///< DICOM list
    int width; ///< image width
    int height; ///< image height
    int index; ///< current data index
    int window_level; ///< window level
    int window_width; ///< window width
    int value; ///< pointed value
    bool enable_show_information; ///< enable showing information
    kvs::Vector2i mouse; ///< windowing mouse

    Parameter( void );

    const bool read( const Argument& argument );

private:

    const bool read_from_file( const std::string& name );

    const bool read_from_directory( const std::string& name, const bool extension_check );
};

#endif // __PARAPMETER_H__
