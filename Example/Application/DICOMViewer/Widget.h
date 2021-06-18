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
#pragma once
#include "Parameter.h"
#include <kvs/Label>
#include <kvs/Screen>


namespace Widget
{

/*===========================================================================*/
/**
 *  @brief  Counter label class.
 */
/*===========================================================================*/
class CounterLabel : public kvs::Label
{
    Parameter* m_parameter; ///< parameters

public:
    CounterLabel( Parameter* parameter, kvs::Screen* screen );
    void screenUpdated();
    void screenResized();
};

/*===========================================================================*/
/**
 *  @brief  Modality label class.
 */
/*===========================================================================*/
class ModalityLabel : public kvs::Label
{
    Parameter* m_parameter; ///< parameters

public:
    ModalityLabel( Parameter* parameter, kvs::Screen* screen );
    void screenUpdated();
    void screenResized();
};

/*===========================================================================*/
/**
 *  @brief  Image information label class.
 */
/*===========================================================================*/
class ImageInfoLabel : public kvs::Label
{
    Parameter* m_parameter; ///< parameters

public:
    ImageInfoLabel( Parameter* parameter, kvs::Screen* screen );
    void screenUpdated();
    void screenResized();
};

} // end of namespace Widget
