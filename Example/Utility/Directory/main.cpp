/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Directory class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1209 2012-06-15 07:57:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Directory>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        std::cerr << "Usage: " << argv[0] << " <directory name>" << std::endl;
        return 1;
    }

    const char* directory_name = argv[1];
    kvs::Directory directory( directory_name );

    // Check the existence of the given file.
    if ( !directory.exists() )
    {
        std::cerr << "Error: " << directory_name << " is not existed." << std::endl;
        return 1;
    }

    // Check whether the given directory is a directory.
    if ( !directory.isDirectory() )
    {
        std::cerr << "Error: " << directory_name << " is not directory." << std::endl;
        return 1;
    }

    // Output directory information.
    std::cout << "Directory path: " << directory.directoryPath() << std::endl;
    std::cout << "Directory path (absolute): " << directory.directoryPath(true) << std::endl;
    std::cout << "Directory name: " << directory.directoryName() << std::endl;

    // Output information of the number of files in the directory.
    std::cout << "Number of files in the directory: " << directory.fileList().size() << std::endl;

    kvs::FileList::iterator file = directory.fileList().begin();
    kvs::FileList::iterator end = directory.fileList().end();
    while ( file != end )
    {
        std::cout << "\t" << file->fileName() << " [" << file->byteSize() << " bytes]" << std::endl;
        ++file;
    }

    // Find "test.cpp" in the directory.
    kvs::File f( "test.cpp" );
    file = directory.find( f );
    if ( file != end )
    {
        std::cout << f.filePath() << " is found in ";
        std::cout << directory.directoryPath(true) << std::endl;
    }
    else
    {
        std::cout << f.filePath() << " is not found in ";
        std::cout << directory.directoryPath(true) << std::endl;
    }

    return 0;
}
