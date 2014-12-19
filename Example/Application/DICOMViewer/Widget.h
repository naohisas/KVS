/*****************************************************************************/
/**
 *  @file   Widget.h
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Widget.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Parameter.h"
#include <kvs/glut/Label>
#include <kvs/glut/Screen>


namespace Widget
{

/*===========================================================================*/
/**
 *  @brief  Counter label class.
 */
/*===========================================================================*/
class CounterLabel : public kvs::glut::Label
{
    Parameter* m_parameter; ///< parameters

public:

    CounterLabel( Parameter* parameter, kvs::glut::Screen* screen );

    void screenUpdated( void );

    void screenResized( void );
};

/*===========================================================================*/
/**
 *  @brief  Modality label class.
 */
/*===========================================================================*/
class ModalityLabel : public kvs::glut::Label
{
    Parameter* m_parameter; ///< parameters

public:

    ModalityLabel( Parameter* parameter, kvs::glut::Screen* screen );

    void screenUpdated( void );

    void screenResized( void );
};

/*===========================================================================*/
/**
 *  @brief  Image information label class.
 */
/*===========================================================================*/
class ImageInfoLabel : public kvs::glut::Label
{
    Parameter* m_parameter; ///< parameters

public:

    ImageInfoLabel( Parameter* parameter, kvs::glut::Screen* screen );

    void screenUpdated( void );

    void screenResized( void );
};

} // end of namespace Widget

#endif // __WIDGET_H__
