#pragma once
#include <cstdlib>
#include <string>
#include <kvs/Program>
#include <kvs/Message>
#include <kvs/Directory>
#include <kvs/File>
#include <kvs/CommandLine>
#include "Constant.h"


namespace kvsmake
{

class Build : public kvs::Program
{
    int exec( int argc, char** argv );
};

inline int Build::exec( int argc, char** argv )
{
    if ( !kvs::File( kvsmake::MakefileName ).exists() )
    {
        kvsMessageError() << "Cannot find " << kvsmake::MakefileName << "." << std::endl;
        return 1;
    }

    std::string make_arguments;
    for ( int i = 1; i < argc; ++i )
    {
        make_arguments += std::string( " " ) + std::string( argv[i] );
    }

    const std::string command =
        kvsmake::MakeCommand + std::string( " -f " ) +
        kvsmake::MakefileName + make_arguments;

    return system( command.c_str() );
}

} // end of namespace kvsmake
