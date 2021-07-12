/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Directory class.
 *  @author Naohisa Sakamoto
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

    std::cout << "Root directory: " << kvs::Directory::RootPath() << std::endl;
    std::cout << "Home directory: " << kvs::Directory::HomePath() << std::endl;
    std::cout << "Temp directory: " << kvs::Directory::TempPath() << std::endl;
    std::cout << "Current directory: " << kvs::Directory::CurrentPath() << std::endl;

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
    std::cout << "Directory path: " << directory.path() << std::endl;
    std::cout << "Directory path (absolute): " << directory.path(true) << std::endl;
    std::cout << "Directory name: " << directory.name() << std::endl;

    // Output information of the number of files in the directory.
    kvs::FileList files = directory.fileList();
    std::cout << "Number of files in the directory: " << files.size() << std::endl;

    for ( const auto& file : files )
    {
        const auto name = file.fileName();
        const auto size = file.byteSize();
        std::cout << "\t" << name << " [" << size << " bytes]" << std::endl;
    }

    return 0;
}
