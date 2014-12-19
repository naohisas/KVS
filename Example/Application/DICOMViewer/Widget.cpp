/*****************************************************************************/
/**
 *  @file   Widget.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Widget.cpp 1518 2013-04-10 01:13:11Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Widget.h"
#include <kvs/RGBColor>
#include <kvs/Dicom>
#include <kvs/Camera>


namespace Widget
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CounterLabel class.
 *  @param  parameter [in] pointer to the parameters
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
CounterLabel::CounterLabel( Parameter* parameter, kvs::glut::Screen* screen ):
    kvs::glut::Label( screen ),
    m_parameter( parameter )
{
    setTextColor( kvs::RGBColor( 255, 255, 255 ) );
}

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is updated.
 */
/*===========================================================================*/
void CounterLabel::screenUpdated( void )
{
    if ( m_parameter->enable_show_information ) show();
    else hide();

    const int counter = m_parameter->index + 1;
    const int counter_max = m_parameter->dicom_list.size();
    setText("[%04d/%04d]", counter, counter_max );
}

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is resized.
 */
/*===========================================================================*/
void CounterLabel::screenResized( void )
{
    // Anchor at the top-right on the screen.
    const size_t width = static_cast<kvs::glut::Screen*>(screen())->scene()->camera()->windowWidth();
    setX( width - 100 );
    setY( 10 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ModalityLabel class.
 *  @param  parameter [in] pointer to the parameters
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
ModalityLabel::ModalityLabel( Parameter* parameter, kvs::glut::Screen* screen ):
    kvs::glut::Label( screen ),
    m_parameter( parameter )
{
    setTextColor( kvs::RGBColor( 255, 255, 255 ) );
}

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is updated.
 */
/*===========================================================================*/
void ModalityLabel::screenUpdated( void )
{
    if ( m_parameter->enable_show_information ) show();
    else hide();

    const kvs::Dicom* dicom = m_parameter->dicom_list[ m_parameter->index ];
    const std::string manufacturer = dicom->manufacturer();
    const std::string modality = dicom->modality();
    setText("Manufacturer: %s", manufacturer.c_str() );
    addText("Modality:     %s", modality.c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is resized.
 */
/*===========================================================================*/
void ModalityLabel::screenResized( void )
{
    // Anchor at the top-left on the screen.
    setX( 10 );
    setY( 10 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ImageInfoLabel class.
 */
/*===========================================================================*/
ImageInfoLabel::ImageInfoLabel( Parameter* parameter, kvs::glut::Screen* screen ):
    kvs::glut::Label( screen ),
    m_parameter( parameter )
{
    setTextColor( kvs::RGBColor( 255, 255, 255 ) );
};

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is updated.
 */
/*===========================================================================*/
void ImageInfoLabel::screenUpdated( void )
{
    if ( m_parameter->enable_show_information ) show();
    else hide();

    const kvs::Dicom* dicom = m_parameter->dicom_list[ m_parameter->index ];

    setText("Slice Thickness:   %.3lf", dicom->sliceThickness() );
    addText("Slice Spacing:     %.3lf", dicom->sliceSpacing() );
    addText("Series Number:     %d",    dicom->seriesNumber() );
    addText("Image Number:      %d",    dicom->imageNumber() );
    addText("Slice Location:    %.3lf", dicom->sliceLocation() );
    addText("Row:               %d",    dicom->row() );
    addText("Column:            %d",    dicom->column() );
    addText("Bits Allocated:    %d",    dicom->bitsAllocated() );
    addText("Bits Stored:       %d",    dicom->bitsStored() );
    addText("High Bit:          %d",    dicom->highBit() );
    addText("Pixel Spacing:     %.3lf %.3lf",
            dicom->pixelSpacing()[0],
            dicom->pixelSpacing()[1] );
    addText("Rescale Slope:     %.3lf", dicom->rescaleSlope() );
    addText("Rescale Intersept: %.3lf", dicom->rescaleIntersept() );
    addText("Min. raw value:    %d",    dicom->minRawValue() );
    addText("Max. raw value:    %d",    dicom->maxRawValue() );
    addText("Min. value:        %d",    dicom->minValue() );
    addText("Max. value:        %d",    dicom->maxValue() );
    addText("Window Level:      %d",    m_parameter->window_level );
    addText("Window Width:      %d",    m_parameter->window_width );
}

/*===========================================================================*/
/**
 *  @brief  Callback function when the screen is resized.
 */
/*===========================================================================*/
void ImageInfoLabel::screenResized( void )
{
    // Anchor at the bottom-left on the screen.
    const size_t character_height = 12;
    const size_t height = static_cast<kvs::glut::Screen*>(screen())->scene()->camera()->windowHeight();
    setX( 10 );
    setY( height - 19 * character_height - 10 );
}

} // end of namespace Widget
