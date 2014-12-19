/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::File class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1205 2012-06-15 07:16:47Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/File>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    kvs::File file( filename );

    // Check the existence of the given file.
    if ( !file.exists() )
    {
        std::cerr << "Error: " << filename << " is not existed." << std::endl;
        return 1;
    }

    // Output filename information.
    std::cout << "file path: " << file.filePath() << std::endl;
    std::cout << "path name: " << file.pathName() << std::endl;
    std::cout << "path name (absolute): " << file.pathName(true) << std::endl;
    std::cout << "file name: " << file.fileName() << std::endl;
    std::cout << "base name: " << file.baseName() << std::endl;
    std::cout << "extension: " << file.extension() << std::endl;
    std::cout << "extension (complete): " << file.extension(true) << std::endl;

    // Output the file size in byte.
    std::cout << "file size: " << file.byteSize() << " [bytes]" << std::endl;

    return 0;
}
