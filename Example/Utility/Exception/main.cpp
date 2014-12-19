/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Exception classes.
 *  @author Naoya Maeda
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
#include <sstream>
#include <kvs/Exception>


void throw_test()
{
    try
    {
        // KVS_TRHOW(exception_type, message) //
        KVS_THROW(kvs::NotSupportedException, "Sample message");
    }
    catch (kvs::Exception& exception)
    {
        std::cout << exception.what() << std::endl;
        exception.addMessage("Added message");
        throw;
    }
}


int main()
{
    try
    {
        throw_test();
        std::cout << "This line is not output." << std::endl;
        return 0;
    }
    catch (const kvs::Exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 0;
    }
}
