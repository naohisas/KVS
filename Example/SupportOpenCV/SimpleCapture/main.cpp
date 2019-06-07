/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of the OpenCV.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <string>
#include <kvs/Message>
#include <kvs/opencv/OpenCV>
#include <kvs/opencv/CaptureDevice>
#include <kvs/opencv/Window>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main()
{
    // Initialize capture device.
    kvs::opencv::CaptureDevice device;
    if ( !device.create( CV_CAP_ANY ) )
    {
        kvsMessageError("Capture device is NULL.");
        return 1;
    }

    // Create window.
    kvs::opencv::Window window;
    window.create("SimpleCapture");

    for ( ; ; )
    {
        // Capture frame.
        const IplImage* frame = device.queryFrame();
        if ( !frame )
        {
            kvsMessageError("Frame is NULL.");
            return 1;
        }

        window.show( frame );
        // Do not release the frame.

        /* If ESC key pressed, Key=0x10001B under OpenCV 0.9.7 (linux version),
         * remove higher bits using AND operator.
         */
        if ( ( kvs::opencv::Window::WaitKey(1) & 255 ) == 27 ) break;
    }

    return 0;
}
